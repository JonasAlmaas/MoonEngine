#pragma once

#include <string>

#include <glm/glm.hpp>


namespace Moon {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		/*
		 * Uploads a 4 * 4 matrix to the gpu.
		 * Binding the shader has to be done before uploading to it!
		 * 
		 * @param name of the uniform.
		 * @param matrix to upload.
		 */
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;

	};

}