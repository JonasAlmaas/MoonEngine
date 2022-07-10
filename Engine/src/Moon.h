#pragma once

// For use by Moon engine applications only

#include "Moon/Core/Base.h"

// ---- Application ----
#include "Moon/Core/Application/Application.h"

// ---- Event ----
#include "Moon/Core/Event/Event.h"
#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Event/KeyEvent.h"
#include "Moon/Core/Event/MouseEvent.h"
#include "Moon/Core/Event/WindowEvent.h"

// ---- Input ----
#include "Moon/Core/Input/Input.h"
#include "Moon/Core/Input/Code/KeyCode.h"
#include "Moon/Core/Input/Code/MouseCode.h"

// ---- Layer ----
#include "Moon/Core/Layer/Layer.h"

// ---- Renderer ----
#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Renderer/Renderer2D/Renderer2D.h"

#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"

#include "Moon/Core/Renderer/Buffer/Framebuffer/Framebuffer.h"
#include "Moon/Core/Renderer/Buffer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/Buffer/UniformBuffer/UniformBuffer.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer/Layout/VertexBufferLayout.h"

#include "Moon/Core/Renderer/Camera/Camera.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"

#include "Moon/Core/Renderer/Shader/Shader.h"
#include "Moon/Core/Renderer/Shader/ShaderLibrary/ShaderLibrary.h"

#include "Moon/Core/Renderer/Texture/Texture.h"
#include "Moon/Renderer/Texture/SubTexture2D.h"

#include "Moon/Core/Renderer/VertexArray/VertexArray.h"

// ---- Util ----
#include "Moon/Core/Util/Util.h"
#include "Moon/Core/Util/Math/Math.h"
#include "Moon/Core/Util/FileDialog.h"
#include "Moon/Core/Util/Random/Random.h"
#include "Moon/Debug/Instrumentor.h"

// ---- Window ----
#include "Moon/Core/Window/Window.h"

// ---- Scene ----
#include "Moon/Scene/Scene.h"
#include "Moon/Scene/Component/SceneComponents.h"
#include "Moon/Scene/Entity/Entity.h"
#include "Moon/Scene/Entity/Scriptable/ScriptableEntity.h"

// ---- Misc ----
#include "Moon/Core/Type/Types.h"
#include "Moon/Core/Log/Log.h"
