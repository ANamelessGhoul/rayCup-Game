#include "../include/Game.h"
#include <math.h>

namespace CupGame
{
	Game* Game::Instance = nullptr;
	GameState Game::GameState = Start;
	
	Game::Game()
	{
		Instance = this;

		ScreenSize.x = GetScreenWidth();
		ScreenSize.y = GetScreenHeight();

		_background = LoadTexture("resources/Background.png");
		Cup::LoadTextures();

		Vector2 position = {ScreenSize.x * 0.2f, ScreenSize.y * 2 / 3.0f};
		for (int i = 0; i < CUP_COUNT; i++)
		{
			_startingPositions[i] = Vector2(position);
			_cups[i].Init(position, false);
			position.x += ScreenSize.x * 0.3f;

			_cupRenderOrder[i] = &_cups[i];

			#ifdef DEBUG_TEXT
			_cups[i].SetIndex(i);
			#endif
		}

		int randomCupIndex = GetRandomValue(0, CUP_COUNT - 1);
		_cups[randomCupIndex].SetBall(true);

		_timerLimit = 3;



	}

	Game::~Game()
	{
		UnloadTexture(_background);
		Cup::UnloadTextures();
	}

	void Game::Update(float deltaTime)
	{
		ScreenSize.x = GetScreenWidth();
		ScreenSize.y = GetScreenHeight();

		_mousePosition = GetMousePosition();

		for (int i = 0; i < CUP_COUNT; i++)
		{
			_cups[i].Update(deltaTime);
		}

		bool isDoneSwapping = true;

		switch (GameState)
		{
		case Start:
			_timer += deltaTime;
			if(_timer > _timerLimit)
			{
				_timer = 0;
				SwapBalls();
				GameState = Swapping;
			}
			break;
		case Swapping:
			
			for (int i = 0; i < CUP_COUNT; i++)
			{
				if(_cups[i].IsSwapping)
				{
					isDoneSwapping = false;
				}
			}
			
			if(isDoneSwapping)
			{
				_timer += deltaTime;
				if(_timer > _timerLimit)
				{
					_timer = 0;
					_swapCount--;

					if(_swapCount != 0)
						SwapBalls();
					else
						GameState = Selecting;
					
				}
			}

			break;	
		case Selecting:
			_selectedCup = -1;
			for (int i = 0; i < CUP_COUNT; i++)
			{
				if(_cups[i].CheckPointCollision(_mousePosition))
				{
					_selectedCup = i;
				}
			}

			if(_selectedCup != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				_win = _cups[_selectedCup].GetBall();
				GameState = GameOver;
				_timer = 0;
			}
			break;
		case GameOver:
			_timer += deltaTime;
			if(_timer > 2.5f)
			{
				if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					ResetGame(_win);
				}
			}

			break;
		default:
			break;
		}

	}

	void Game::Draw()
	{
		DrawTextureEx(_background, {ScreenSize.x/2 - _background.width * 2, ScreenSize.y/2 - _background.height * 2}, 0, 4, WHITE);

		for (int i = 0; i < CUP_COUNT; i++)
		{
			_cupRenderOrder[i]->Draw();
		}
		
		#ifdef DEBUG_TEXT
        DrawText(TextFormat("Mouse position Y: %f %f", _mousePosition.x, _mousePosition.y), 190, 200, 20, LIGHTGRAY);
		#endif
		
		switch (GameState)
		{
		case Swapping:
			#ifdef DEBUG_TEXT
			DrawText(TextFormat("Timer: %f", _timer), 190, 100, 20, LIGHTGRAY);	
			DrawText(TextFormat("Swapping: %i and %i", _first, _second), 190, 150, 20, LIGHTGRAY);
			#endif
			break;
		case Selecting:
			#ifdef DEBUG_TEXT
			if(_selectedCup != -1)
				DrawText(TextFormat("Cup Selected: %i", _selectedCup), 190, 150, 20, LIGHTGRAY);
			#endif
			break;
		case GameOver:
			DrawText(_win ? "You Win!" : "You Lose!", 190, 130, 20, _win ? GREEN : RED);
			if(_timer > 2.5f)
				DrawText("Click to continue...", 190, 160, 20, GRAY);
		default:
			break;
		}
	}

	void Game::SwapBalls()
	{
		int firstIndex = GetRandomValue(0, CUP_COUNT - 1);
		int secondIndex = GetRandomValue(0, CUP_COUNT - 2);
		
		if(secondIndex >= firstIndex)
		{
			secondIndex++;
		}

		_first = firstIndex;
		_second = secondIndex;

		SwapBalls(firstIndex, secondIndex);
		_timerLimit = 0.1f * GetRandomValue(0,5) / Cup::Speed;
	}

	void Game::SwapBalls(int firstIndex, int secondIndex)
	{
		int thirdIndex = 3 - (firstIndex + secondIndex);

		_cups[firstIndex].SetTargetPosition(_cups[secondIndex].GetPosition(), true);
		_cups[secondIndex].SetTargetPosition(_cups[firstIndex].GetPosition(), false);
		//_cups[thirdIndex].SetTargetPosition(_cups[firstIndex].GetPosition(), !firstInFront);

		#ifdef DEBUG_TEXT
		_cups[firstIndex].SetIndex(secondIndex);
		_cups[secondIndex].SetIndex(firstIndex);
		#endif

		_cupRenderOrder[0] = &_cups[secondIndex];
		_cupRenderOrder[1] = &_cups[thirdIndex];
		_cupRenderOrder[2] = &_cups[firstIndex];

		auto temp = _cups[firstIndex];
		_cups[firstIndex] = _cups[secondIndex];
		_cups[secondIndex] = temp;

		

	}

	void Game::ResetGame(bool isWin)
	{



		if(isWin)
		{
			_totalSwaps += 2;
			Cup::Speed *= 1.2f;	

			for (int i = 0; i < CUP_COUNT; i++)
			{
				_cups[i].Reset();
			}	
		}
		else
		{
			_totalSwaps = 3;
			Cup::Speed = 1;

			for (int i = 0; i < CUP_COUNT; i++)
			{
				_cups[i].Init(_startingPositions[i], false);
			}
			_cups[GetRandomValue(0, CUP_COUNT - 1)].SetBall(true);
		}

		_swapCount = _totalSwaps;
		_timer = 0;
		_timerLimit = 3; 
		GameState = Start;
	}
} // namespace CupGame