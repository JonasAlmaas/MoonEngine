#include "mepch.h"
#include "Moon/Scripting/ScriptGlue.h"

#include <mono/metadata/object.h>


namespace Moon {

	#define ME_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Moon.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		ME_CORE_LOG("{}, {}", cStr, parameter);
		mono_free(cStr);
	}

	static void NativeLog_Float3(glm::vec3* parameter, glm::vec3* outResult)
	{
		ME_CORE_LOG("Value: {}", *parameter);
	}

    void ScriptGlue::RegisterFunctions()
    {
		ME_ADD_INTERNAL_CALL(NativeLog);
		ME_ADD_INTERNAL_CALL(NativeLog_Float3);
    }

}
