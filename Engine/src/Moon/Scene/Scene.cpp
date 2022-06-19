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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(&m_Registry);

		entity.AddComponent<TransformComponent>();

		auto& tagComp = entity.AddComponent<TagComponent>(name);
		tagComp.Tag = name.empty() ? "Unnamed Entity" : name;

		return entity;
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
