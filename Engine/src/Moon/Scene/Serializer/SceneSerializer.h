#pragma once

#include "Moon/Scene/Scene.h"


namespace Moon {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::filesystem::path& filepath);
		void SerializeRuntime(const std::filesystem::path& filepath);

		bool Deserialize(const std::filesystem::path& filepath);
		bool DeserializeRuntime(const std::filesystem::path& filepath);

	private:
		Ref<Scene> m_Scene;

	};

}
