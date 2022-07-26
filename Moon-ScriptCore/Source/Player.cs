using System;

using Moon;


namespace Sandbox
{

	public class Player : Entity
	{

		void OnCreate()
		{
			Console.WriteLine($"Player::OnCreate - {ID}");
		}

		void OnUpdate(float ts)
		{
			Console.WriteLine($"Player::OnUpdate: {ts}");

			float speed = 5.0f;
			Float3 velocity = new Float3();

			if (Input.IsKeyPressed(KeyCode.W))
				velocity.y = 1.0f;
			else if (Input.IsKeyPressed(KeyCode.S))
				velocity.y = -1.0f;

			if (Input.IsKeyPressed(KeyCode.A))
				velocity.x = -1.0f;
			else if (Input.IsKeyPressed(KeyCode.D))
				velocity.x = 1.0f;

			velocity *= speed;

			Float3 translation = Translation;
			translation += velocity * ts;
			Translation = translation;
		}

		void OnDestroy()
		{
			Console.WriteLine("Player::OnDestroy");
		}

	}

}
