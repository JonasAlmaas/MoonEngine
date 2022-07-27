#include "mepch.h"
#include "Moon/Core/Math/Noise.h"

#include <FastNoise.h>


namespace Moon {

	Noise::Noise()
	{
		m_FastNoise = new FastNoise();
	}

	Noise::Noise(int seed)
	{
		m_FastNoise = new FastNoise(seed);
	}

	Noise::~Noise()
	{
		delete m_FastNoise;
	}

	// -- Get Noise --
	float Noise::Get(float x, float y)
	{
		return m_FastNoise->GetNoise(x, y);
	}

	float Noise::Get(float x, float y, float z)
	{
		return m_FastNoise->GetNoise(x, y, z);
	}

	// -- Noise Type --
	void Noise::SetNoiseType(Type noiseType)
	{
		m_FastNoise->SetNoiseType((FastNoise::NoiseType)noiseType);
	}

	Noise::Type Noise::GetNoiseType()
	{
		return (Type)m_FastNoise->GetNoiseType();
	}

	// -- Interpolation Method --
	void Noise::SetInterp(InterpMethod method)
	{
		m_FastNoise->SetInterp((FastNoise::Interp)method);
	}

	Noise::InterpMethod Noise::GetInterp()
	{
		return (InterpMethod)m_FastNoise->GetInterp();
	}

	// -- Frequency --
	void Noise::SetFrequency(float frequency)
	{
		m_FastNoise->SetFrequency(frequency);
	}

	float Noise::GetFrequency() const
	{
		return m_FastNoise->GetFrequency();
	}

	// -- Fractal Octaves --
	void Noise::SetFractalOctaves(int octaves)
	{
		m_FastNoise->SetFractalOctaves(octaves);
	}

	int Noise::GetFractalOctaves() const
	{
		return m_FastNoise->GetFractalOctaves();
	}

	// -- Fractal Lacunarity --
	void Noise::SetFractalLacunarity(float lacunarity)
	{
		return m_FastNoise->SetFractalLacunarity(lacunarity);
	}

	float Noise::GetFractalLacunarity() const
	{
		return m_FastNoise->GetFractalLacunarity();
	}

	// -- Fractal Gain --
	void Noise::SetFractalGain(float gain)
	{
		m_FastNoise->SetFractalGain(gain);
	}

	float Noise::GetFractalGain() const
	{
		return m_FastNoise->GetFractalGain();
	}

	// -- Fractal Type --
	void Noise::SetFractalType(FractalType fractalType)
	{
		m_FastNoise->SetFractalType((FastNoise::FractalType)fractalType);
	}

	Noise::FractalType Noise::GetFractalType() const
	{
		return (FractalType)m_FastNoise->GetFractalType();
	}

	// ----------------------------
	// -------- Static API --------
	// ----------------------------

	static FastNoise s_FastNoise;

	void Noise::SetSeed(int seed)
	{
		s_FastNoise.SetSeed(seed);
	}

	int Noise::GetSeed()
	{
		return s_FastNoise.GetSeed();
	}

	float Noise::PerlinNoise(float x, float y)
	{
		s_FastNoise.SetNoiseType(FastNoise::Perlin);
		return s_FastNoise.GetNoise(x, y);
	}

}
