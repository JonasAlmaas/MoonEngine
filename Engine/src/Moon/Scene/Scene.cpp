#include "mepch.h"
#include "Moon/Scene/Scene.h"

#include "Moon/Renderer/Renderer2D/Renderer2D.h"


namespace Moon {
	
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	EntityID Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite);
		}
	}

}
