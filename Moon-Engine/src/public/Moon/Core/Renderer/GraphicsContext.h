#pragma once

#include "Moon/Core/Base.h"


namespace Moon {

	class GraphicsContext
	{
	public:
		~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);

	};

}
