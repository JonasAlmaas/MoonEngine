#pragma once

// For use by Moon engine applications only

#pragma region Core
	#include "Moon/Core/Base.h"

	#pragma region Application
	#include "Moon/Core/Application/Application.h"
	#pragma endregion

	#pragma region Camera
	#include "Moon/Core/Camera/Orthographic/OrthographicCamera.h"
	#include "Moon/Core/Camera/Perspective/PerspectiveCamera.h"
	#pragma endregion

	#pragma region Event
	#include "Moon/Core/Event/Event.h"
	#include "Moon/Core/Event/ApplicationEvent.h"
	#include "Moon/Core/Event/KeyEvent.h"
	#include "Moon/Core/Event/MouseEvent.h"
	#include "Moon/Core/Event/WindowEvent.h"
	#pragma endregion

	#pragma region Input
	#include "Moon/Core/Input/Input.h"
	#include "Moon/Core/Input/Code/KeyCode.h"
	#include "Moon/Core/Input/Code/MouseCode.h"
	#pragma endregion

	#pragma region Renderer
		#pragma region Buffer
		#include "Moon/Core/Renderer/Buffer/Framebuffer/Framebuffer.h"
		#include "Moon/Core/Renderer/Buffer/IndexBuffer/IndexBuffer.h"
		#include "Moon/Core/Renderer/Buffer/UniformBuffer/UniformBuffer.h"
		#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBuffer.h"
		#pragma endregion

		#include "Moon/Core/Renderer/Renderer.h"
		#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"

		#pragma region Shader
		#include "Moon/Core/Renderer/Shader/Shader.h"
		#include "Moon/Core/Renderer/Shader/ShaderLibrary/ShaderLibrary.h"
		#pragma endregion

		#include "Moon/Core/Renderer/Texture/Texture.h"
		#include "Moon/Core/Renderer/VertexArray/VertexArray.h"
	#pragma endregion

	#pragma region Type
	#include "Moon/Core/Type/Types.h"
	#include "Moon/Core/Type/UUID/UUID.h"
	#pragma endregion

	#pragma region Util
	#include "Moon/Core/Util/Util.h"
	#include "Moon/Core/Util/FileDialog.h"
	#include "Moon/Core/Util/Timer.h"
	#include "Moon/Core/Util/Math/Math.h"
	#include "Moon/Core/Util/Random/Random.h"
	#include "Moon/Core/Util/Conversion/Hexadecimal/Hexadecimal.h"
	#pragma endregion

	#pragma region Window
	#include "Moon/Core/Window/Window.h"
	#pragma endregion

	#pragma region Misc
	#include "Moon/Core/Layer/Layer.h"
	#pragma endregion
#pragma endregion

#pragma region Debug
#include "Moon/Debug/Instrumentor.h"
#pragma endregion

#pragma region Renderer
#include "Moon/Renderer/Renderer2D/Renderer2D.h"
#include "Moon/Renderer/Texture/SubTexture2D.h"
#pragma endregion

#pragma region Scene
#include "Moon/Scene/Scene.h"
#include "Moon/Scene/Component/SceneComponents.h"
#include "Moon/Scene/Entity/Entity.h"
#include "Moon/Scene/Entity/Scriptable/ScriptableEntity.h"
#pragma endregion
