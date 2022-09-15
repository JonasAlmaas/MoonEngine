#include "mepch.h"
#include "Moon/Scene.h"

#include "Moon/Core/Renderer/RenderCommand.h"
#include "Moon/Renderer/Renderer2D.h"
#include "Moon/Scene/Components.h"
#include "Moon/Scene/Entity/ScriptableEntity.h"
#include "Moon/Scripting/ScriptEngine.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>


namespace Moon {

	static b2BodyType RigidBody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:		return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:		return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic:		return b2_kinematicBody;
		}

		ME_CORE_ASSERT(false, "Unknown body type!");
		return b2_staticBody;
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;

		OnPhysics2DStart();

		// ---- Native Scripts ----
		{
			auto view = m_Registry.view<NativeScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };
				auto& nsc = entity.GetComponent<NativeScriptComponent>();

				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = entity;
					nsc.Instance->OnCreate();
				}
			}
		}

		// C# Scripting
		{
			ScriptEngine::OnRuntimeStart(this);

			// Instantiate all script entities
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;

		// ---- Native Scripts ----
		{
			auto view = m_Registry.view<NativeScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };
				entity.GetComponent<NativeScriptComponent>().Instance->OnDestroy();
			}
		}

		// ---- C# Scripts ----
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };
				ScriptEngine::OnDestroyEntity(entity);
			}

			ScriptEngine::OnRuntimeStop();
		}

		OnPhysics2DStop();
	}

	void Scene::OnRuntimeUpdate(Timestep ts)
	{
		// ---- Native Scripts ----
		{
			auto view = m_Registry.view<NativeScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };
				entity.GetComponent<NativeScriptComponent>().Instance->OnUpdate(ts);
			}
		}

		// ---- C# Scripts ----
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };
				ScriptEngine::OnUpdate(entity, ts);
			}
		}

		// ---- Physics 2D ----
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			// Update renderable entities
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;

				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		if (!m_ActiveCamera || !m_ActiveCamera.HasComponent<TransformComponent>() || !m_ActiveCamera.HasComponent<CameraComponent>())
			return;

		auto& camera = m_ActiveCamera.GetComponent<CameraComponent>().Camera;
		auto& cameraTransComp = m_ActiveCamera.GetComponent<TransformComponent>();
		camera->SetViewWithTransform(cameraTransComp.GetTransform());
		RenderScene(camera);
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnUpdateSimulation(Timestep ts, const Ref<RenderCamera>& camera)
	{
		// ---- Physics 2D ----
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			// Update renderable entities
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { &m_Registry, e };

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;

				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		RenderScene(camera);
	}

	void Scene::OnPhysics2DStart()
	{
		// Create phisics world
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { &m_Registry, e };

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = RigidBody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::RenderScene(const Ref<RenderCamera>& camera)
	{
		Renderer2D::BeginScene(camera);

		// Draw sprite
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transformComp, spriteComp] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSpriteRendererComponent(transformComp.GetTransform(), spriteComp, (int)entity);
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transformComp, circleComp] = view.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircleRendererComponent(transformComp.GetTransform(), circleComp, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize non fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera->SetViewportSize(width, height);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = Entity(&m_Registry);

		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();

		auto& tagComp = entity.AddComponent<TagComponent>(name);
		tagComp.Tag = name.empty() ? "Unnamed Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists(AllComponents{}, newEntity, entity);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::SetActiveCamera(Entity camera)
	{
		m_ActiveCamera = camera;

		if (m_ActiveCamera)
		{
			// TODO: If you have a fixed aspect ratio this might be an issue. It kinda just updates the aspect ratio when setting it as active.
			if (m_ViewportWidth != 0 && m_ViewportHeight != 0)
				camera.GetComponent<CameraComponent>().Camera->SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	Entity Scene::GetEntityByUUID(UUID entityID)
	{
		ME_CORE_ASSERT(m_EntityMap.find(entityID) != m_EntityMap.end(), "Entity does not exist!");
		return { &m_Registry, m_EntityMap.at(entityID) };
	}

}
