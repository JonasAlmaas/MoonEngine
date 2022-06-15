#include "mepch.h"
#include "Moon/Core/Util/Platform/FileDialog/FileDialog.h"


namespace Moon {

	Scope<FileDialogAPI> FileDialog::s_FileDialogAPI = nullptr;

	void FileDialog::Init()
	{
		ME_PROFILE_FUNCTION();

		s_FileDialogAPI = FileDialogAPI::Create();
	}

}
