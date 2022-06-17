#pragma once

#include <Moon.h>

using namespace Moon;


namespace Sandbox {

	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		Color ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticles = 100000);

		void OnUpdate(Timestep ts);
		void OnRender(OrthographicCamera& camera);

		void Emit(const ParticleProps& particleProps);

	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			Color ColorBegin, ColorEnd;

			float Rotation;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};

		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex;

	};

}
