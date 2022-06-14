#include "mepch.h"
#include "Moon/Core/Renderer/Renderer2D/Renderer2D.h"

#include "Moon/Core/Renderer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"
#include "Moon/Core/Renderer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/Shader/Shader.h"
#include "Moon/Core/Renderer/Texture/Texture.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Moon {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> Shader;
		Ref<Texture2D> WhiteTexture;

		Renderer2DStorage()
		{
			ME_PROFILE_FUNCTION();

			WhiteTexture = Texture2D::Create(1, 1);
			uint32_t textureData = 0xffffffff;
			WhiteTexture->SetData(&textureData, sizeof(uint32_t));
		}
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		ME_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		Ref<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "Position XYZ" },
			{ ShaderDataType::Float2, "UV" },
		};
		VB->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(VB);

		uint32_t indices[3 * 2] = {
			0, 1, 2,
			0, 2, 3
		};
		Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		s_Data->QuadVertexArray->SetIndexBuffer(IB);

		s_Data->Shader = Shader::Create("assets/shaders/2D.glsl");
		s_Data->Shader->Bind();
		s_Data->Shader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ME_PROFILE_FUNCTION();

		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		ME_PROFILE_FUNCTION();
	}

	// ---- Primitives ----

	void Renderer2D::Super_DrawQuad(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		s_Data->Shader->SetFloat4("u_Color", tint);
		s_Data->Shader->SetFloat2("u_TileFactor", tileFactor);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * rotation * scale;
		s_Data->Shader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

		texture->Unbind();
	}

	// -- Draw Quad --

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Color& color)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size }, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Color& color)
	{
		Super_DrawQuad(position, 0.0f, { size, size }, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, 0.0f, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, 0.0f, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, 0.0f, { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, 0.0f, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, 0.0f, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, 0.0f, { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, 0.0f, { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawQuad(position, 0.0f, size, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawQuad(position, 0.0f, size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawQuad(position, 0.0f, size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawQuad(position, 0.0f, size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawQuad(position, 0.0f, size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawQuad(position, 0.0f, size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, 0.0f, size, texture, tileFactor, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawQuad(position, 0.0f, size, texture, tileFactor, tint);
	}

	// -- Draw Rotated Quad --

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size }, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size }, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, s_Data->WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawQuad(position, glm::radians(rotationDegrees), size, texture, tileFactor, tint);
	}

}
