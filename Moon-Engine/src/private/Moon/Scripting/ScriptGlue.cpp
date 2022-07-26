#include "mepch.h"
#include "Moon/Scripting/ScriptGlue.h"

#include "Moon/Core/Input.h"
#include "Moon/Core/Type/UUID.h"
#include "Moon/Scene/Component/SceneComponents.h"
#include "Moon/Scripting/ScriptEngine.h"

#include <mono/metadata/object.h>


namespace Moon {

	#define ME_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Moon.InternalCalls::" #Name, Name)

	// -----------------------
	// -------- Input --------
	// -----------------------

	static bool Input_IsKeyPressed(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

	// ------------------------
	// -------- Entity --------
	// ------------------------

	static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	} 

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

    void ScriptGlue::RegisterFunctions()
    {
		ME_ADD_INTERNAL_CALL(Input_IsKeyPressed);
		ME_ADD_INTERNAL_CALL(Entity_GetTranslation);
		ME_ADD_INTERNAL_CALL(Entity_SetTranslation);
    }

}
