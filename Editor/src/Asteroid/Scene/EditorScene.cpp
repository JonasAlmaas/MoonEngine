#include "aopch.h"
#include "EditorScene.h"


namespace Asteroid {

	void EditorScene::OnUpdateEditor(Timestep ts, const Ref<EditorCamera>& camera)
	{
		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		Renderer2D::BeginScene(camera->GetViewProjection());

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transformComp, spriteComp] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transformComp.GetTransform(), spriteComp.Color);
		}

		Renderer2D::EndScene();
	}

}
