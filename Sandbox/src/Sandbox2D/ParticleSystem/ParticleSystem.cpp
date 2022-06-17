#include "Sandbox2D/ParticleSystem/ParticleSystem.h"


namespace Sandbox {

	ParticleSystem::ParticleSystem(uint32_t maxParticles)
		: m_PoolIndex(maxParticles - 1)
	{
		m_ParticlePool.resize(maxParticles);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::OnRender(OrthographicCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float lifePercent = particle.LifeRemaining / particle.LifeTime;
			float size = Math::Lerp(particle.SizeBegin, particle.SizeEnd, lifePercent);
			Color color = particle.ColorBegin.Lerp(particle.ColorEnd, lifePercent);
			Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y, 0.2f }, particle.Rotation, size, color);
		}

		Renderer2D::EndScene();
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 360.0f;

		// Size
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5);
		particle.SizeEnd = particleProps.SizeEnd;

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		// Life
		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

}
