#include "mepch.h"
#include "Moon/Core/Math/Noise.h"

#include <FastNoise.h>


namespace Moon {

	Noise::Noise(const Specification& spec)
		: m_Specification(spec)
	{
		m_FastNoise = new FastNoise(m_Specification.Seed);
		m_FastNoise->SetFrequency(m_Specification.Frequency);
		m_FastNoise->SetInterp((FastNoise::Interp)m_Specification.InterpMethod);
		m_FastNoise->SetNoiseType((FastNoise::NoiseType)m_Specification.Type);
		m_FastNoise->SetFractalType((FastNoise::FractalType)m_Specification.FractalType);
		m_FastNoise->SetFractalOctaves(m_Specification.FractalOctaves);
		m_FastNoise->SetFractalLacunarity(m_Specification.FractalLacunarity);
		m_FastNoise->SetFractalGain(m_Specification.FractalGain);

		if (m_Specification.NoiseLookup != nullptr)
		{
			m_FastNoise->SetCellularNoiseLookup(m_Specification.NoiseLookup->m_FastNoise);
			m_FastNoise->SetCellularDistanceFunction((FastNoise::CellularDistanceFunction)m_Specification.CellularDistanceFunction);
			m_FastNoise->SetCellularReturnType((FastNoise::CellularReturnType)m_Specification.CellularReturnType);
		}
		
		m_FastNoise->SetCellularDistance2Indices(m_Specification.CellularDistance2Indices.x, m_Specification.CellularDistance2Indices.y);
		m_FastNoise->SetCellularJitter(m_Specification.CellularJitter);
	}

	Noise::~Noise()
	{
		delete m_FastNoise;
	}

	float Noise::Get(float x, float y)
	{
		return m_FastNoise->GetNoise(x, y);
	}

	float Noise::Get(float x, float y, float z)
	{
		return m_FastNoise->GetNoise(x, y, z);
	}

}
