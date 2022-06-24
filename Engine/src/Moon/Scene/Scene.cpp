#include "mepch.h"
#include "Moon/Scene/Scene.h"

#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Renderer/Renderer2D/Renderer2D.h"
#include "Moon/Scene/Component/SceneComponents.h"


namespace Moon {
	
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// ---- Scripts ----
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// Move to scene begin play. But I don't have that yet.
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity(&m_Registry, entity);
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);

				// Call OnDestroy on scene stop.
			});
		}

		// ---- Renderer2D ----
		{
			Renderer2D::ResetStats();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();

			if (m_ActiveCamera == nullptr)
				return;

			if (!m_ActiveCamera->HasComponent<CameraComponent>())
				return;

			auto& camera = m_ActiveCamera->GetComponent<CameraComponent>().Camera;

			if (m_ActiveCamera->HasComponent<TransformComponent>())
			{
				auto& cameraTransComp = m_ActiveCamera->GetComponent<TransformComponent>();
				Renderer2D::BeginScene(camera.GetProjection(), cameraTransComp.GetTransform());
			}
			else {
				Renderer2D::BeginScene(camera.GetProjection(), glm::mat4(1.0f));
			}

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transformComp, spriteComp] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transformComp.GetTransform(), spriteComp.Color);
			}

			Renderer2D::EndScene();
		}
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
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::SetActiveCamera(Entity& camera)
	{
		m_ActiveCamera = &camera;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(&m_Registry);

		entity.AddComponent<TransformComponent>();

		auto& tagComp = entity.AddComponent<TagComponent>(name);
		tagComp.Tag = name.empty() ? "Unnamed Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

}
