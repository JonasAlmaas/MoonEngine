#pragma once

#include "Moon/Core/Base.h"


namespace Moon {
	
	class MOON_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

}
