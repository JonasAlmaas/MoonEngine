#pragma once


namespace Moon {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		EntityID CreateEntity();

		// TEMP!
		entt::registry& Reg() { return m_Registry; }

	private:
		entt::registry m_Registry;

	};

}
