#include "aopch.h"
#include "Asteroid/Panel/SceneHierarchy/SceneHierarchyPanel.h"


namespace Asteroid {

    SceneHierarchyPanel::SceneHierarchyPanel(entt::registry& registry)
        : m_Registry(&registry)
    {
    }

    void SceneHierarchyPanel::OnAttach()
    {
    }

    void SceneHierarchyPanel::OnDetach()
    {
    }

    void SceneHierarchyPanel::OnUpdate(Timestep ts)
    {
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Registry->each(ME_BIND_EVENT_FN(SceneHierarchyPanel::RenderEntity));

        ImGui::End();
    }

    void SceneHierarchyPanel::SetRegistry(entt::registry& registry)
    {
        m_Registry = &registry;
    }

    void SceneHierarchyPanel::RenderEntity(auto entity)
    {
        auto& tag = m_Registry->get<TagComponent>(entity).Tag;
        ImGui::Text("%s", tag.c_str());
    }

}
