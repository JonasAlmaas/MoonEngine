#pragma once

#include "Moon/Core/Platform/FileDialog/FileDialogAPI.h"


namespace Moon {

	class FileDialog
	{
	public:
		static void Init();

		/*
		 * @return Name of the file that was openend.
		 * Returns an empty string if canceled.
		 */
		inline static std::string OpenFile(const char* filter)
		{
			return s_FileDialogAPI->OpenFile(filter);
		}

		/*
		 * @return Name of the file that was saved.
		 * Returns an empty string if canceled.
		 */
		inline static std::string SaveFile(const char* filter)
		{
			return s_FileDialogAPI->SaveFile(filter);
		}

	private:
		static Scope<FileDialogAPI> s_FileDialogAPI;

	};

}
