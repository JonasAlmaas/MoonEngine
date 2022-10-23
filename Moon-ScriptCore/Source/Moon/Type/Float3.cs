
namespace Moon
{
	public struct Float3
	{
		public float x, y, z;

		public static Float3 Zero => new Float3(0.0f);

		public Float3(float scalar)
		{
			x = scalar;
			y = scalar;
			z = scalar;
		}

		public Float3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		public Float3(Float2 xy, float _z)
		{
			x = xy.x;
			y = xy.y;
			z = _z;
		}

		public Float3(float _x, Float2 yz)
		{
			x = _x;
			y = yz.x;
			z = yz.y;
		}

		public Float2 xy
		{
			get => new Float2(x, y);
			set
			{
				x = value.x;
				y = value.y;
			}
		}

		public static Float3 operator +(Float3 a, Float3 b)
		{
			return new Float3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		public static Float3 operator *(Float3 vector, float factor)
		{
			return new Float3(vector.x * factor, vector.y * factor, vector.z * factor);
		}
	}
}
