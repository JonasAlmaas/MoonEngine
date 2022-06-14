#include "mepch.h"
#include "Moon/Core/Platform/Platform.h"

#include "Moon/Core/Platform/FileDialog/FileDialog.h"


namespace Moon {

	Scope<PlatformAPI> Platform::s_PlatformAPI = nullptr;
	
	void Platform::Init()
	{
		ME_PROFILE_FUNCTION();

		s_PlatformAPI = PlatformAPI::Create();

		FileDialog::Init();
	}

}
