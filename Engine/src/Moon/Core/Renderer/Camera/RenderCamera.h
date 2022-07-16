#pragma once


namespace Moon {

	class RenderCamera
	{
	public:
		RenderCamera() = default;
		RenderCamera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~RenderCamera() = default;

		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetProjection() const { return m_Projection; }		
		const glm::mat4 GetViewProjection() const { return m_Projection * m_View; };

	protected:
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);

	};
}
