#pragma once


namespace Moon {

	struct Timestep
	{
		float Time;	// Time in seconds

		Timestep(float time = 0.0f)
			: Time(time) {}

		operator float() const { return Time; }

		float GetSeconds() const { return Time; }
		float GetMilliseconds() const { return Time * 1000.0f; }
	};

}
