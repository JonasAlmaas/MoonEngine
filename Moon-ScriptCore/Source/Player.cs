using System;

using Moon;


namespace Sandbox
{

	public class Player : Entity
	{
		private TransformComponent m_Transform;
		private Rigidbody2DComponent m_Rigidbody;

		void OnCreate()
		{
			Console.WriteLine($"Player::OnCreate - {ID}");

			// Should check if we have the component before getting it
			m_Transform = GetComponent<TransformComponent>();
			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
		}

		void OnUpdate(float ts)
		{
			Console.WriteLine($"Player::OnUpdate: {ts}");

			float speed = 0.1f;
			Float3 velocity = Float3.Zero;

			if (Input.IsKeyPressed(KeyCode.W))
				velocity.y = 1.0f;
			else if (Input.IsKeyPressed(KeyCode.S))
				velocity.y = -1.0f;

			if (Input.IsKeyPressed(KeyCode.A))
				velocity.x = -1.0f;
			else if (Input.IsKeyPressed(KeyCode.D))
				velocity.x = 1.0f;

			velocity *= speed;

			m_Rigidbody.ApplyLinearImpulse(velocity.xy, true);
		}

		void OnDestroy()
		{
			Console.WriteLine("Player::OnDestroy");
		}

	}

}
