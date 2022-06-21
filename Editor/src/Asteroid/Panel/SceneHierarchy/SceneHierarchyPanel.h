#pragma once

using namespace Moon;

#include "Asteroid/Panel/Panel.h"


namespace Asteroid {

	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(entt::registry& registry);
		virtual ~SceneHierarchyPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

		void SetRegistry(entt::registry& registry);

	private:
		void RenderEntity(entt::entity ntityHandle);

	private:
		entt::registry* m_Registry = nullptr;
		Entity m_SelectionContext;
	};

}
