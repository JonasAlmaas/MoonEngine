#include "mepch.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"

#include "Moon/Platform/OpenGL/RendererAPI/OpenGLRendererAPI.h"


namespace Moon {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

}
