#pragma once

#include "Moon/Core/Buffer.h"

// This is really just a workaround Microsoft being extremely dumb and thinking no one would ever use the name CreateDirectory, etc... for a method...
#ifdef CreateDirectory
	#undef CreateDirectory
	#undef DeleteFile
	#undef MoveFile
	#undef CopyFile
	#undef SetEnvironmentVariable
	#undef GetEnvironmentVariable
#endif

namespace Moon {

	class FileSystem
	{
	public:
		static bool CreateDirectory(const std::filesystem::path& directory);
		static bool Exists(const std::filesystem::path& filepath);
		static bool DeleteFile(const std::filesystem::path& filepath);
		static bool MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
		static bool CopyFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
		static bool IsDirectory(const std::filesystem::path& filepath);

		static bool Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
		static bool Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
		static bool Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
		static bool RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName);

		static bool ShowFileInExplorer(const std::filesystem::path& path);

		static bool WriteBytes(const std::filesystem::path& filepath, const Buffer& buffer);
		static Buffer ReadBytes(const std::filesystem::path& filepath);

	};

}
