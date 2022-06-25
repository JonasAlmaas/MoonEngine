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

        EditorState::GetSceneRegistry()->each([&](auto entityID)
        {   
            Entity entity{ EditorState::GetSceneRegistry(), entityID };
            DrawEntityNode(entity);
        });

        // Deselect by clicking the background
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            EditorState::SetSelectionContext();

		// Right click blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				EditorState::GetActiveScene()->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (entity == EditorState::GetSelectionContext())
            flags |= ImGuiTreeNodeFlags_Selected;

        // If the entity doesnt have any more children, add this flag
        //ImGuiTreeNodeFlags_Leaf

        bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        if (ImGui::IsItemClicked())
			EditorState::SetSelectionContext(entity);

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

        if (open)
            ImGui::TreePop();

		if (entityDeleted)
		{
			if (EditorState::GetSelectionContext() == entity)
				EditorState::SetSelectionContext({});

			Entity camera = EditorState::GetActiveScene()->GetActiveCamera();
			if (camera && camera == entity)
				EditorState::GetActiveScene()->SetActiveCamera({});

			EditorState::GetActiveScene()->DestroyEntity(entity);
		}
    }

}
