#pragma once

using namespace Moon;


namespace Asteroid {

	class SceneState
	{
	public:
		enum class State
		{
			Edit = 0,
			Play,
			Pause,
			Simulate,
		};

	public:
		static State& Get();

		static void OnScenePlay();
		static void OnSceneStop();

	};

}
