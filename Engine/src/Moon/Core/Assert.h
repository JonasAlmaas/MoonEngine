#pragma once


#if ME_ENABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define ME_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { ME##type##ERROR(msg, __VA_ARGS__); ME_DEBUGBREAK(); } }
	#define ME_INTERNAL_ASSERT_WITH_MSG(type, check, ...) ME_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define ME_INTERNAL_ASSERT_NO_MSG(type, check) ME_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", ME_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define ME_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define ME_INTERNAL_ASSERT_GET_MACRO(...) ME_EXPAND_MACRO( ME_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ME_INTERNAL_ASSERT_WITH_MSG, ME_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define ME_ASSERT(...) ME_EXPAND_MACRO( ME_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define ME_CORE_ASSERT(...) ME_EXPAND_MACRO( ME_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define ME_ASSERT(...)
	#define ME_CORE_ASSERT(...)
#endif
