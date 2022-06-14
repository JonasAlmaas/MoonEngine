#pragma once


namespace Moon {

	class FileDialogAPI
	{
	public:
		virtual ~FileDialogAPI() = default;

		virtual std::string OpenFile(const char* filter) = 0;
		virtual std::string SaveFile(const char* filter) = 0;

		static Scope<FileDialogAPI> Create();

	};

}
