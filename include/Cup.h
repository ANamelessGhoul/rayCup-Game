#ifndef CUP_H
#define CUP_H

#include "Debug.h"
#include "raylib.h"


namespace CupGame
{
	class Cup
	{
	public:
		static void LoadTextures();
		static void UnloadTextures();

		static float Speed;

		Cup();
		~Cup();
		void Init(const Vector2& position, bool hasBall);
		void Reset();
		void Update(float deltaTime);
		void Draw();
		bool CheckPointCollision(const Vector2& point);
		

		void SetTargetPosition(const Vector2&, bool Infront);

		Vector2& GetPosition();
		void SetBall(bool);
		bool GetBall();

		bool MousedOver = false;
		bool IsSwapping;

		#ifdef DEBUG_TEXT
		void SetIndex(int);
		#endif
	private:
		static Texture2D _mainTexture;
		static Texture2D _outlineTexture;
		static Texture2D _ballTexture;

		void DrawTexture(const Texture2D& texture, const Color& tint = WHITE);
		void DrawTexture(const Texture2D& texture, const Vector2&, const Color& tint = WHITE);

		float _time;



		Vector2 _colliderSize;
		Vector2 _cornerOffset;

		Vector2 _position;
		bool _hasBall;

		Vector2 _startPosition;
		Vector2 _midPosition;
		Vector2 _targetPosition;

		#ifdef DEBUG_TEXT
		int _index;
		#endif
	};

	
} // namespace CupGame


#endif //CUP_H