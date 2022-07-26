using System;
using System.Runtime.CompilerServices;


namespace Moon
{

	public struct Float2
	{
		public float x, y;

		public Float2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}
	}

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

		public static Float3 operator+(Float3 a, Float3 b)
		{
			return new Float3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		public static Float3 operator*(Float3 vector, float scalar)
		{
			return new Float3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}
	}

	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyPressed(KeyCode keyCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Entity_GetTranslation(ulong entityID, out Float3 translation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Entity_SetTranslation(ulong entityID, ref Float3 translation);
	}

	public class Entity
	{
		protected Entity() { ID = 0; }

		internal Entity(ulong id)
		{
			ID = id;
		}

		public readonly ulong ID;

		public Float3 Translation
		{
			get
			{
				InternalCalls.Entity_GetTranslation(ID, out Float3 translation);
				return translation;
			}
			set
			{
				InternalCalls.Entity_SetTranslation(ID, ref value);
			}
		}

	}

}
