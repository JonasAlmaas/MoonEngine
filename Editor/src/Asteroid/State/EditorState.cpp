#include "aopch.h"
#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	struct EditorStateData
	{
		Ref<Framebuffer> Framebuffer;
		Entity SelectionContext;
		entt::registry* Registry;
	};

	static EditorStateData s_Data;

	void EditorState::Init()
	{
		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		s_Data.Framebuffer = Framebuffer::Create(spec);
	}

	Ref<Framebuffer> EditorState::GetFramebuffer()
	{
		return s_Data.Framebuffer;
	}

	Entity EditorState::GetSelectionContext()
	{
		return s_Data.SelectionContext;
	}

	void EditorState::SetSelectionContext(Entity entity)
	{
		s_Data.SelectionContext = entity;
	}

	entt::registry* EditorState::GetRegistry()
	{
		return s_Data.Registry;
	}

	void EditorState::SetRegistry(entt::registry& reg)
	{
		s_Data.Registry = &reg;
	}

}
