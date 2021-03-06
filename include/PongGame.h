//Ian Wright 9/11/18

#pragma once
#include "Ball.h"
#include <vector>

#define numPassiveShapes 60
#define numActiveShapes 2

class PongGame
{
public:
	PongGame();
	~PongGame() = default;

	void updateParameters(float speed, short side, sf::Color humanColor, sf::Color aiColor, float difficulty = 1);
	bool update(float delta, int scoreToWin);
	void reset();

	//defines states of the game 
	enum GameState
	{
		InMenu, Paused, GameOver, Options, InGame, Exiting
	};
	GameState state;

	void setState(GameState newState);

	//pointers to the Player object on the corresponding side of the court, assigned in initVars() since Game::_pong is static
	Player *left;
	Player *right;

	//Stores relevant pong objects that have an sf::shape
	struct PongObjects
	{
		Ball ball;
		Player humanPlayer;
		Player aiPlayer;
	} objects;

private:
	void _updateAI(float delta);
	//AI can move paddle when the distance between its paddle and the ball is less than the below value
	float _aiResponseDistance;

	sf::Text _notInPlayText;
	float _gameTimeFactor;

	//scoring sounds
	sf::SoundBuffer _scoreBuffer;
	sf::Sound _scoreSound;
};