#pragma once


namespace Moon {

	class PlatformUtilAPI
	{
	public:
		virtual ~PlatformUtilAPI() = default;

		virtual float GetTime() = 0;

		static Scope<PlatformUtilAPI> Create();

	};

}
