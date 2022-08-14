#pragma once

// For use by Moon engine applications only

#pragma region Core
	#include "Moon/Core/Base.h"
	#include "Moon/Core/Application.h"

	#pragma region Camera
	#include "Moon/Core/Camera/OrthographicCamera.h"
	#include "Moon/Core/Camera/PerspectiveCamera.h"
	#pragma endregion

	#pragma region Event
	#include "Moon/Core/Event.h"
	#include "Moon/Core/Event/ApplicationEvent.h"
	#include "Moon/Core/Event/KeyEvent.h"
	#include "Moon/Core/Event/MouseEvent.h"
	#include "Moon/Core/Event/WindowEvent.h"
	#pragma endregion

	#pragma region Input
	#include "Moon/Core/Input.h"
	#include "Moon/Core/Input/KeyCode.h"
	#include "Moon/Core/Input/MouseCode.h"
	#pragma endregion

	#pragma region Math
	#include "Moon/Core/Math.h"
	#include "Moon/Core/Math/Noise.h"
	#include "Moon/Core/Math/Spline.h"
	#pragma endregion

	#pragma region Renderer
		#pragma region Buffer
		#include "Moon/Core/Renderer/Buffer/Framebuffer.h"
		#include "Moon/Core/Renderer/Buffer/IndexBuffer.h"
		#include "Moon/Core/Renderer/Buffer/UniformBuffer.h"
		#include "Moon/Core/Renderer/Buffer/VertexBuffer.h"
		#pragma endregion

		#include "Moon/Core/Renderer.h"
		#include "Moon/Core/Renderer/RenderCommand.h"
		#include "Moon/Core/Renderer/Shader.h"

		#include "Moon/Core/Renderer/Texture.h"
		#include "Moon/Core/Renderer/VertexArray.h"
	#pragma endregion

	#pragma region Type
	#include "Moon/Core/Type/Base.h"
	#include "Moon/Core/Type/Color.h"
	#include "Moon/Core/Type/UUID.h"
	#pragma endregion

	#pragma region Utilities
	#include "Moon/Core/Utilities.h"
	#include "Moon/Core/Utilities/FileDialog.h"
	#include "Moon/Core/Utilities/FileSystem.h"
	#include "Moon/Core/Utilities/Timer.h"
	#include "Moon/Core/Utilities/Random.h"
	#include "Moon/Core/Utilities/Conversion/Hexadecimal.h"
	#pragma endregion

	#pragma region Window
	#include "Moon/Core/Window.h"
	#pragma endregion

	#pragma region Misc
	#include "Moon/Core/Layer.h"
	#pragma endregion
#pragma endregion

#pragma region Asset
#include "Moon/Asset/AssetImporter.h"
#pragma endregion

#pragma region Debug
#include "Moon/Debug/Instrumentor.h"
#pragma endregion

#pragma region Renderer
#include "Moon/Renderer/Renderer2D.h"
#include "Moon/Renderer/ShaderLibrary.h"
#include "Moon/Renderer/SubTexture2D.h"
#include "Moon/Renderer/TextSurface.h"
#pragma endregion

#pragma region Scene
#include "Moon/Scene.h"
#include "Moon/Scene/Components.h"
#include "Moon/Scene/Entity.h"
#include "Moon/Scene/Entity/ScriptableEntity.h"
#pragma endregion
