#include "mepch.h"
#include "Moon/Platform/OpenGL/VertexArray/OpenGLVertexArray.h"

#include <glad/glad.h>


namespace Moon {

	// TEMPORARY (maybe)
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Moon::ShaderDataType::Bool:		return GL_BOOL;
			case Moon::ShaderDataType::Int:			return GL_INT;
			case Moon::ShaderDataType::Int2:		return GL_INT;
			case Moon::ShaderDataType::Int3:		return GL_INT;
			case Moon::ShaderDataType::Int4:		return GL_INT;
			case Moon::ShaderDataType::Float:		return GL_FLOAT;
			case Moon::ShaderDataType::Float2:		return GL_FLOAT;
			case Moon::ShaderDataType::Float3:		return GL_FLOAT;
			case Moon::ShaderDataType::Float4:		return GL_FLOAT;
			case Moon::ShaderDataType::Mat3:		return GL_FLOAT;
			case Moon::ShaderDataType::Mat4:		return GL_FLOAT;
		}

		ME_CORE_ASSERT(false, "Unknows ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		ME_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ME_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		ME_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		ME_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		ME_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
