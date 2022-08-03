
namespace Moon
{
	public struct Float2
	{
		public float x, y;

		public static Float2 Zero => new Float2(0.0f);

		public Float2(float scalar)
		{
			x = scalar;
			y = scalar;
		}

		public Float2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		public static Float2 operator +(Float2 a, Float2 b)
		{
			return new Float2(a.x + b.x, a.y + b.y);
		}

		public static Float2 operator *(Float2 vector, float factor)
		{
			return new Float2(vector.x * factor, vector.y * factor);
		}
	}
}
