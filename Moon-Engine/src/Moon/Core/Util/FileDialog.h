#pragma once


namespace Moon {

	class FileDialog
	{
	public:

		/*
		 * @return Path to file.
		 * Empty string if canceled.
		 */
		static std::string OpenFile(const char* filter);

		/*
		 * @return Path to directory.
		 * Empty string if canceled.
		 */
		static std::string SaveFile(const char* filter);

	};

}
