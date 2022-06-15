#pragma once

#include "Moon/Core/Util/Platform/FileDialog/FileDialogAPI.h"


namespace Moon {

	class WindowsFileDialogAPI : public FileDialogAPI
	{
	public:
		WindowsFileDialogAPI();
		virtual ~WindowsFileDialogAPI();

		virtual std::string OpenFile(const char* filter) override;
		virtual std::string SaveFile(const char* filter) override;

	};

}
