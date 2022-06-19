#pragma once

// For use by Moon engine applications only

// ---- Application ----
#include "Moon/Core/Application/Application.h"

// ---- Input ----
#include "Moon/Core/Input/Input.h"
#include "Moon/Core/Input/Code/KeyCode.h"
#include "Moon/Core/Input/Code/MouseCode.h"

// ---- Layer ----
#include "Moon/Core/Layer/Layer.h"

// ---- Core Renderer ----
#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"

#include "Moon/Core/Renderer/Buffer/Framebuffer/Framebuffer.h"
#include "Moon/Core/Renderer/Buffer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBuffer.h"

#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"

#include "Moon/Core/Renderer/Shader/Shader.h"
#include "Moon/Core/Renderer/Shader/ShaderLibrary/ShaderLibrary.h"

#include "Moon/Core/Renderer/Texture/Texture.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"

// ---- Renderer ----
#include "Moon/Renderer/Renderer2D/Renderer2D.h"
#include "Moon/Renderer/Texture/SubTexture2D.h"

// ---- Util ----
#include "Moon/Core/Util/Util.h"
#include "Moon/Core/Util/Math.h"
#include "Moon/Core/Util/Platform/FileDialog/FileDialog.h"
#include "Moon/Core/Util/Random/Random.h"

// ---- Scene ----
#include "Moon/Scene/Scene.h"
#include "Moon/Scene/Component/Components.h"
#include "Moon/Scene/Entity/Entity.h"
#include "Moon/Scene/Entity/Scriptable/ScriptableEntity.h"

// ---- Misc ----
#include "Moon/CameraController/OrthographicCameraController/OrthographicCameraController.h"
#include "Moon/Core/Type/Type.h"
#include "Moon/Core/Log/Log.h"
