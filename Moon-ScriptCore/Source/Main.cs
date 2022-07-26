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

		public Float3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
	}

	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog(string text, int parameter);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog_Float3(ref Float3 parameter);
	}

	public class Entity
	{

		public float FloatVar { get; set; }

		public Entity()
		{
			Console.WriteLine("Main constructor!");

			Log("Native Log", 34251);

			Float3 pos = new Float3(1, 2.5f, 3);
			Log(pos);
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello World from C#!");
		}

		public void PrintInt(int value)
		{
			Console.WriteLine($"C# says: {value}");
		}

		public void PrintInts(int value1, int value2)
		{
			Console.WriteLine($"C# says: {value1} and {value2}");
		}

		public void PrintCustomMessage(string message)
		{
			Console.WriteLine($"C# says: {message}");
		}

		private void Log(string text, int parameter)
		{
			InternalCalls.NativeLog(text, parameter);
		}

		private void Log(Float3 parameter)
		{
			InternalCalls.NativeLog_Float3(ref parameter);
		}

	}

}
