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

        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void SceneHierarchyPanel::SetRegistry(entt::registry& registry)
    {
        m_Registry = &registry;
    }

    void SceneHierarchyPanel::RenderEntity(entt::entity entityHandle)
    {
        Entity entity{ m_Registry, entityHandle };

        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_;
        if (entity == m_SelectionContext)
            flags |= ImGuiTreeNodeFlags_Selected;

        // If the entity doesnt have any more children, add this flag
        //ImGuiTreeNodeFlags_Leaf

        bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        if (ImGui::IsItemClicked())
            m_SelectionContext = entity;

        if (open)
        {
            ImGui::TreePop();
        }
    }

}
