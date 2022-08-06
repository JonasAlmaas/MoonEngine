#include "mepch.h"
#include "Moon/Core/Util/FileSystem.h"


namespace Moon {

	bool FileSystem::CreateDirectory(const std::filesystem::path& directory)
	{
		return std::filesystem::create_directories(directory);
	}

	bool FileSystem::Exists(const std::filesystem::path& filepath)
	{
		return std::filesystem::exists(filepath);
	}

	bool FileSystem::DeleteFile(const std::filesystem::path& filepath)
	{
		if (!FileSystem::Exists(filepath))
			return false;

		if (std::filesystem::is_directory(filepath))
			return std::filesystem::remove_all(filepath) > 0;

		return std::filesystem::remove(filepath);
	}

	bool FileSystem::MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
	{
		return Move(filepath, dest / filepath.filename());
	}

	bool FileSystem::CopyFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
	{
		return Copy(filepath, dest / filepath.filename());
	}

	bool FileSystem::IsDirectory(const std::filesystem::path& filepath)
	{
		return std::filesystem::is_directory(filepath);
	}

	bool FileSystem::Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
	{
		if (FileSystem::Exists(newFilepath))
			return false;

		std::filesystem::rename(oldFilepath, newFilepath);
		return true;
	}

	bool FileSystem::Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
	{
		if (FileSystem::Exists(newFilepath))
			return false;

		std::filesystem::copy(oldFilepath, newFilepath);
		return true;
	}

	bool FileSystem::Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
	{
		return Move(oldFilepath, newFilepath);
	}

	bool FileSystem::RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName)
	{
		std::filesystem::path newPath = fmt::format("{0}\\{1}{2}", oldFilepath.parent_path().string(), newName, oldFilepath.extension().string());
		return Rename(oldFilepath, newPath);
	}

	bool FileSystem::ShowFileInExplorer(const std::filesystem::path& path)
	{
		auto absolutePath = std::filesystem::canonical(path);
		if (!Exists(absolutePath))
			return false;

		std::string cmd = fmt::format("explorer.exe /select,\"{0}\"", absolutePath.string());
		system(cmd.c_str());
		return true;
	}

	bool FileSystem::WriteBytes(const std::filesystem::path& filepath, const Buffer& buffer)
	{
		std::ofstream stream(filepath, std::ios::binary | std::ios::trunc);

		if (!stream)
		{
			stream.close();
			return false;
		}

		stream.write((char*)buffer.Data, buffer.Size);
		stream.close();

		return true;
	}

	Buffer FileSystem::ReadBytes(const std::filesystem::path& filepath)
	{
		Buffer buffer;

		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		ME_CORE_ASSERT(stream);

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();
		ME_CORE_ASSERT(size != 0);

		buffer.Allocate(size);
		stream.read((char*)buffer.Data, buffer.Size);
		stream.close();

		return buffer;
	}

}
