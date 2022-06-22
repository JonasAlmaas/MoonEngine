#pragma once

using namespace Moon;


namespace Asteroid {

	class EditorState
	{
	public:
		static void Init();

		static Ref<Framebuffer> GetFramebuffer();

		static Entity GetSelectionContext();
		static void SetSelectionContext(Entity entity = {});

		static entt::registry* GetRegistry();
		static void SetRegistry(entt::registry& reg);

	};

}