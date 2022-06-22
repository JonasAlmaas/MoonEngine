#include "aopch.h"
#include "Asteroid/Panel/SceneHierarchy/SceneHierarchyPanel.h"

#include "Asteroid/State/EditorState.h"


namespace Asteroid {

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

        EditorState::GetRegistry()->each(ME_BIND_EVENT_FN(SceneHierarchyPanel::RenderEntity));

        ImGui::End();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void SceneHierarchyPanel::RenderEntity(entt::entity entityHandle)
    {
        Entity entity{ EditorState::GetRegistry(), entityHandle };

        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (entity == EditorState::GetSelectionContext())
            flags |= ImGuiTreeNodeFlags_Selected;

        // If the entity doesnt have any more children, add this flag
        //ImGuiTreeNodeFlags_Leaf

        bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        if (ImGui::IsItemClicked())
            EditorState::SetSelectionContext(entity);

        if (open)
        {
            ImGui::TreePop();
        }
    }

}
