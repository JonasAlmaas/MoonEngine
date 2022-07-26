#include "mepch.h"
#include "Moon/Core/Renderer/RenderCommand.h"

#include "Moon/Platform/OpenGL/OpenGLRendererAPI.h"


namespace Moon {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

}
