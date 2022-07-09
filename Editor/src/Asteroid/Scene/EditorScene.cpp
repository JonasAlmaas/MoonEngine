#include "aopch.h"
#include "EditorScene.h"

#include <unordered_map>


namespace Asteroid {

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).ID;
			ME_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
			entt::entity dstEnttID = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(dstEnttID, component);
		}
	}

	Ref<EditorScene> EditorScene::Copy()
	{
		Ref<EditorScene> newScene = CreateRef<EditorScene>();

		newScene->m_ViewportWidth = m_ViewportWidth;
		newScene->m_ViewportHeight = m_ViewportHeight;

		newScene->SetActiveCamera(m_ActiveCamera);

		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = m_Registry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = m_Registry.get<IDComponent>(e).ID;
			const auto& name = m_Registry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent<TransformComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<CircleRendererComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, m_Registry, enttMap);
		CopyComponent<CircleCollider2DComponent>(dstSceneRegistry, m_Registry, enttMap);

		return newScene;
	}

	void EditorScene::OnUpdateEditor(Timestep ts, const Ref<EditorCamera>& camera)
	{
		Renderer2D::BeginScene(camera->GetViewProjection());

		// Draw sprites
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

}
