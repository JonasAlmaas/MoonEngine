#pragma once


class FastNoise;

namespace Moon {

	class Noise
	{
	public:
		enum class Type
		{
			Value,
			ValueFractal,
			Perlin,
			PerlinFractal,
			Simplex,
			SimplexFractal,
			Cellular,
			WhiteNoise,
			Cubic
		};

		enum class InterpMethod
		{
			Linear,
			Hermite,
			Quintic
		};

		enum class FractalType
		{
			FBM,
			Billow,
			RigidMulti
		};

	public:
		Noise();
		Noise(int seed);
		~Noise();

		/**
		 * @return A float between -1 and 1.
		 * Value is based on all other paramaters.
		 */
		float Get(float x, float y);
		float Get(float x, float y, float z);

		/**
		 * Sets noise return type of GetNoise(...)
		 * Default: Simplex
		 */
		void SetNoiseType(Type type);
		Type GetNoiseType();

		/**
		 * Changes the interpolation method used to smooth between noise values
		 * Possible interpolation methods (lowest to highest quality) :
		 * - Linear
		 * - Hermite
		 * - Quintic
		 * Used in Value, Perlin Noise and Position Warping
		 * Default: Quintic
		 */
		void SetInterp(InterpMethod method);
		InterpMethod GetInterp();

		/**
		 * Sets frequency for all noise types.
		 * Default: 0.01
		 */
		void SetFrequency(float frequency);
		float GetFrequency() const;

		/**
		* Sets octave count for all fractal noise types.
		* Default: 3
		*/
		void SetFractalOctaves(int octaves);
		int GetFractalOctaves() const;

		/**
		 * Sets octave lacunarity for all fractal noise types.
		 * Default: 2.0
		 */
		void SetFractalLacunarity(float lacunarity);
		float GetFractalLacunarity() const;

		/**
		 * Sets octave gain for all fractal noise types.
		 * Default: 0.5
		*/
		void SetFractalGain(float gain);
		float GetFractalGain() const;

		/**
		 * Sets method for combining octaves in all fractal noise types.
		 * Default: FBM
		*/
		void SetFractalType(FractalType fractalType);
		FractalType GetFractalType() const;

	public:
		// -- Static API --
		static void SetSeed(int seed);
		static int GetSeed();

		static float PerlinNoise(float x, float y);

	private:
		FastNoise* m_FastNoise = nullptr;

	};

}
