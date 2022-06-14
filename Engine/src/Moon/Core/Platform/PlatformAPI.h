#pragma once


namespace Moon {

	class PlatformAPI
	{
	public:
		virtual ~PlatformAPI() = default;

		virtual float GetTime() = 0;

		static Scope<PlatformAPI> Create();

	};

}
