#pragma once

#include "Moon/Scene/Entity/Scriptable/ScriptableEntity.h"


namespace Moon {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		//void 

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}
