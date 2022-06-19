#include "mepch.h"
#include "Moon/Scene/Scene.h"

#include "Moon/Renderer/Renderer2D/Renderer2D.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"


namespace Moon {
	
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(&m_Registry);

		entity.AddComponent<TransformComponent>();

		auto& tagComp = entity.AddComponent<TagComponent>(name);
		tagComp.Tag = name.empty() ? "Unnamed Entity" : name;

		return entity;
	}

#include "Moon/Scene/Component/NativeScriptComponent.h"

	void Scene::OnUpdate(Timestep ts)
	{
		// ---- Scripts ----
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.InstantiateFn();
					nsc.Instance->m_Entity = Entity(&m_Registry, entity);

					if (nsc.OnCreateFn)
						nsc.OnCreateFn(nsc.Instance);
				}

				if (nsc.OnUpdateFn)
					nsc.OnUpdateFn(nsc.Instance, ts);
			});
		}

		// ---- Renderer2D ----
		{
			if (m_ActiveCamera == nullptr)
				return;

			Renderer2D::ResetStats();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();

			auto& camera = m_ActiveCamera->GetComponent<CameraComponent>();
			auto& cameraTransform = m_ActiveCamera->GetComponent<TransformComponent>();
			Renderer2D::BeginScene(camera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform, sprite);
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

}
