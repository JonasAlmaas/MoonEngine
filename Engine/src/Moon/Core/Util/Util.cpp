#include "mepch.h"
#include "Moon/Core/Util/Util.h"

#include "Moon/Core/Util/Platform/FileDialog/FileDialog.h"
#include "Moon/Core/Util/Random/Random.h"


namespace Moon {
	
	Scope<PlatformUtilAPI> Util::s_PlatformUtilAPI = nullptr;

	void Util::Init()
	{
		ME_PROFILE_FUNCTION();

		s_PlatformUtilAPI = PlatformUtilAPI::Create();

		FileDialog::Init();
		Random::Init();
	}

}
