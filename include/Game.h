#ifndef GAME_H
#define GAME_H

#include "Cup.h"
#include "raylib.h"

//#define SHOW_COLLIDERS
#define COLLIDER_COLOR CLITERAL(Color){ 0, 228, 48, 63}
#define CUP_COUNT 3

namespace CupGame
{
	enum GameState
	{
		Start,
		Showing,
		Swapping,
		Selecting,
		GameOver
	};

	class Game
	{
	public:
		static Game* Instance;
		static GameState GameState;
		Vector2 ScreenSize;

		Game(/* args */);
		~Game();
		void Update(float deltaTime);
		void Draw();
	private:
		Vector2 _mousePosition;
		bool _win;

		float _timer;
		float _timerLimit;

		int _selectedCup = -1;
		Cup _cups[CUP_COUNT];
		Vector2 _startingPositions[CUP_COUNT];

		int _first;
		int _second;

		int _totalSwaps = 3;
		int _swapCount = 3;

		Texture2D _background;


		void SwapBalls();
		void SwapBalls(int, int);

		void ResetGame(bool isWin);
	};



} // namespace CupGame

#endif // GAME_H