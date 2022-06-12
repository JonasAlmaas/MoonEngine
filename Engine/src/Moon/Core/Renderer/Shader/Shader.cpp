#include "mepch.h"
#include "Moon/Core/Renderer/Shader/Shader.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/Shader/OpenGLShader.h"


namespace Moon {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		ME_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLShader>(filepath);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		ME_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
