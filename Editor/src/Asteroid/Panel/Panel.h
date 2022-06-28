#pragma once

#include "aopch.h"

using namespace Moon;


namespace Asteroid {

	class Panel
	{
	public:
		virtual ~Panel() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

	};

}
