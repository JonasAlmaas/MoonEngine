#include "mepch.h"
#include "Moon/Scripting/ScriptGlue.h"

#include "Moon/Core/Base.h"
#include "Moon/Core/Input.h"
#include "Moon/Core/Input/KeyCode.h"
#include "Moon/Core/Type/UUID.h"
#include "Moon/Scene/Components.h"
#include "Moon/Scripting/ScriptEngine.h"

#include <box2d/b2_body.h>

#include <mono/metadata/object.h>
#include "mono/metadata/reflection.h"


namespace Moon {

	#define ME_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Moon.InternalCalls::" #Name, Name)

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	// ------------------------
	// -------- Entity --------
	// ------------------------

	static Entity GetEntityFromID(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		ME_CORE_ASSERT(scene, "Script Engine does not currently have a scene context!");
		Entity entity = scene->GetEntityByUUID(entityID);
		ME_CORE_ASSERT(entity, "Scene does not have an entity with the given ID!");

		return entity;
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Entity entity = GetEntityFromID(entityID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ME_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static UUID Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		ME_CORE_ASSERT(scene, "Script Engine does not currently have a scene context!");
		Entity entity = scene->FindEntityByName(nameCStr);

		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	// ----------------------------
	// -------- Components --------
	// ----------------------------

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Entity entity = GetEntityFromID(entityID);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Entity entity = GetEntityFromID(entityID);
		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* worldPosition, bool wake)
	{
		Entity entity = GetEntityFromID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(worldPosition->x, worldPosition->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Entity entity = GetEntityFromID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	// -----------------------
	// -------- Input --------
	// -----------------------

	static bool Input_IsKeyPressed(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("Moon.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				ME_CORE_LOG_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
    {
		ME_ADD_INTERNAL_CALL(Entity_HasComponent);

		ME_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		ME_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		ME_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		ME_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		ME_ADD_INTERNAL_CALL(Input_IsKeyPressed);
    }

}
