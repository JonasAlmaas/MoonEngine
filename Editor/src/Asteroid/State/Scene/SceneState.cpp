#include "aopch.h"
#include "Asteroid/State/Scene/SceneState.h"


namespace Asteroid {

	struct SceneStateData
	{
		SceneState::State SceneState = SceneState::State::Edit;
	};

	static SceneStateData s_Data;

	void SceneState::OnScenePlay()
	{
		s_Data.SceneState = SceneState::State::Play;
	}

	void SceneState::OnSceneStop()
	{
		s_Data.SceneState = SceneState::State::Edit;
	}

	SceneState::State& SceneState::Get()
	{
		return s_Data.SceneState;
	}

}
