#include "../include/Cup.h"
#include "../include/Game.h"
#include "raymath.h"

namespace CupGame
{
	float Cup::Speed = 1;
	Texture2D Cup::_mainTexture = (Texture2D){0}; 
	Texture2D Cup::_outlineTexture = (Texture2D){0}; 
	Texture2D Cup::_ballTexture = (Texture2D){0};

	void Cup::LoadTextures()
	{
		_mainTexture = LoadTexture("resources/Cup.png");
		_outlineTexture = LoadTexture("resources/CupOutline.png");
		_ballTexture = LoadTexture("resources/Ball.png");
	}

	Cup::Cup()
	{
	}
	
	Cup::~Cup()
	{
	}

	void Cup::Init(const Vector2& position, bool hasBall)
	{
		_colliderSize = (Vector2){(float)(_mainTexture.width) * 4, (float)(_mainTexture.height) * 4};
		_cornerOffset = {(float)-_mainTexture.width * 2, (float)-_mainTexture.height * 2};
		_time = 0;

		_position = Vector2(position);
		_startPosition = Vector2(position);
		_midPosition = Vector2(position);
		_targetPosition = Vector2(position);
		_hasBall = hasBall;
	}

	void Cup::Reset()
	{
		_time = 0;

		_startPosition = Vector2(_position);
		_midPosition = Vector2(_position);
		_targetPosition = Vector2(_position);
	}

	void Cup::Update(float deltaTime)
	{
		switch (Game::GameState)
		{
		case Start:
			_time += deltaTime;
			if(_time < 1)
				_position = Vector2Lerp(_startPosition, (Vector2){_startPosition.x , _startPosition.y - 100}, _time);
			else if(_time < 2);
			else if(_time < 3)
				_position = Vector2Lerp((Vector2){_startPosition.x , _startPosition.y - 100}, _startPosition, _time - 2);
			else
				_position = _startPosition;
			break;
		case Swapping:
			if(IsSwapping)
			{
				_time += deltaTime * Speed;
				_time = Clamp(_time, 0, 1);
				if(_time <= 1)
				{
					Vector2 startMid = Vector2Lerp(_startPosition, _midPosition, _time);
					Vector2 midTarget = Vector2Lerp(_midPosition, _targetPosition, _time);

					_position = Vector2Lerp(startMid, midTarget, _time);
				}
				if(_time == 1)
				{
					IsSwapping = false;
					_time = 0;
					_startPosition = Vector2(_position);
				}
			}
			break;
		case GameOver:
			if(_hasBall)
			{
				_time += deltaTime;
				if(_time < 1)
					_position = Vector2Lerp(_startPosition, (Vector2){_startPosition.x , _startPosition.y - 100}, _time);
				else if(_time < 1.5f);
				else if(_time < 2.5f)
					_position = Vector2Lerp((Vector2){_startPosition.x , _startPosition.y - 100}, _startPosition, _time - 1.5f);
				else
					_position = _startPosition;
				break;
			}
		default:
			break;
		}
		

	}

	void Cup::Draw()
	{
		switch (Game::GameState)
		{
		case Start:
		case GameOver:
			if(_hasBall)
				DrawTexture(_ballTexture, (Vector2){_startPosition.x, _startPosition.y + 20});
			break;
		case Selecting:
			if(MousedOver)
				DrawTexture(_outlineTexture, DARKGRAY);
			break;
		default:
			break;
		}
		
		//if(Game::GameState == GameOver && _hasBall)
			//DrawTexture(_outlineTexture, GREEN);
		
		DrawTexture(_mainTexture);

		#ifdef SHOW_COLLIDERS
		DrawRectangleV(Vector2Add(_position, _cornerOffset), _colliderSize, COLLIDER_COLOR);
		#endif
	}
	void Cup::DrawTexture(const Texture2D& texture, const Color& tint)
	{
		DrawTexture(texture, _position, tint);
	}

	void Cup::DrawTexture(const Texture2D& texture, const Vector2& position, const Color& tint)
	{
		DrawTextureEx(texture, (Vector2){position.x - texture.width * 2, position.y - texture.height * 2}, 0, 4, tint);
	}

	bool Cup::CheckPointCollision(const Vector2& point)
	{
		if (Game::GameState == Selecting)
		{
			MousedOver = CheckCollisionPointRec(point, {_position.x + _cornerOffset.x,_position.y + _cornerOffset.y, _colliderSize.x,_colliderSize.y});
			return MousedOver;
		}
		else
		{
			return MousedOver = false;
		}
	}

	void Cup::SetTargetPosition(const Vector2& targetPosition, bool Infront)
	{
		_startPosition = Vector2(_position);
		_targetPosition = Vector2(targetPosition);
		_midPosition = (Vector2){(_targetPosition.x + _startPosition.x)/2, _targetPosition.y + (Infront ? -50 : 50)};

		_time = 0;
		IsSwapping = true;
	}

	Vector2& Cup::GetPosition()
	{
		return _position;
	}

	void Cup::SetBall(bool hasBall)
	{
		_hasBall = hasBall;
	}

	bool Cup::GetBall()
	{
		return _hasBall;
	}
} // namespace CupGame
