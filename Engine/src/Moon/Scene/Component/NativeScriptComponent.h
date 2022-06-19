#pragma once

#include "Moon/Scene/Entity/Scriptable/ScriptableEntity.h"


namespace Moon {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFn;
		std::function<void()> DestroyInstanceFn;

		std::function<void(ScriptableEntity*)> OnCreateFn;
		std::function<void(ScriptableEntity*)> OnDestroyFn;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFn;

		template<typename T>
		void Bind()
		{
			InstantiateFn = [&]() { Instance = new T(); };
			DestroyInstanceFn = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFn = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFn = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFn = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		}
	};

}
