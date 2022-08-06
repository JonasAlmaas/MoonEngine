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

		enum class CellularDistanceFunction
		{
			Euclidean,
			Manhattan,
			Natural
		};

		enum class CellularReturnType
		{
			CellValue,
			NoiseLookup,
			Distance,
			Distance2,
			Distance2Add,
			Distance2Sub,
			Distance2Mul,
			Distance2Div
		};

		struct Specification
		{
			int Seed = 1337;

			// Sets frequency for all noise types
			float Frequency = 0.01f;

			// Changes the interpolation method used to smooth between noise values
			// Possible interpolation methods (lowest to highest quality) :
			// - Linear
			// - Hermite
			// - Quintic
			// Used in Value, Perlin Noise and Position Warping
			InterpMethod InterpMethod = InterpMethod::Quintic;

			// Sets noise return type of Get(...)
			Type Type = Type::Simplex;

			// Sets method for combining octaves in all fractal noise types
			FractalType FractalType = FractalType::FBM;

			// Sets octave count for all fractal noise types
			uint32_t FractalOctaves = 3;

			// Sets octave lacunarity for all fractal noise types
			float FractalLacunarity = 2.0f;
			 
			// Sets octave gain for all fractal noise types
			float FractalGain = 0.5f;

			// Sets distance function used in cellular noise calculations
			CellularDistanceFunction CellularDistanceFunction = CellularDistanceFunction::Euclidean;

			// Note: NoiseLookup requires another Noise object be set with SetCellularNoiseLookup() to function
			CellularReturnType CellularReturnType = CellularReturnType::CellValue;

			// Noise used to calculate a cell value if cellular return type is NoiseLookup
			Noise* NoiseLookup = nullptr;

			// Sets the 2 distance indices used for distance2 return types
			// Note: index0 should be lower than index1
			// Both indices must be >= 0, index1 must be < 4
			glm::uvec2 CellularDistance2Indices = { 0, 1 };

			// Sets the maximum distance a cellular point can move from its grid position
			// Setting this high will make artifacts more common
			float CellularJitter = 0.45f;
		};

	public:
		Noise(const Specification& spec = Specification());
		~Noise();

		/**
		 * @return A float between -1 and 1.
		 * Value is based on all other paramaters.
		 */
		float Get(float x, float y);
		float Get(float x, float y, float z);

		const Specification& GetSpecification() const { return m_Specification; }

	private:
		FastNoise* m_FastNoise = nullptr;
		Specification m_Specification;

	};

}
