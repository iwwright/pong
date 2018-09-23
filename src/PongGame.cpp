//Ian Wright 9/11/18
#include "PongGame.h"
#include <iostream>


PongGame::PongGame()
{
	//game should start at menu
	state = InMenu;
}

PongGame::~PongGame()
{
}

//creates pong game with passed paramters
void PongGame::updateParameters(float speed, short side, sf::Color humanColor, sf::Color aiColor, float difficulty)
{
	_gameTimeFactor = speed;

	objects.humanPlayer = Player(true, side, humanColor);
	objects.aiPlayer = Player(false, -1 * side, aiColor, difficulty);

	//make ai response distance scale with difficulty
	_aiResponseDistance = 1.25f * difficulty * 300;

	//so that left/right players can be determined easily for update()
	if (objects.humanPlayer.side == -1)
	{
		left = &objects.humanPlayer;
		right = &objects.aiPlayer;
	}
	else
	{
		left = &objects.aiPlayer;
		right = &objects.humanPlayer;
	}

	objects.ball = Ball(left->paddleColor, right->paddleColor);
	objects.ball.initSounds();
}




bool PongGame::update(float delta, int scoreToWin)
{
	delta *= _gameTimeFactor;

	if (state != InGame)
		return 0;

	sf::Vector2f leftPos = left->paddle.getPosition();
	sf::Vector2f rightPos = right->paddle.getPosition();


	//uses ball.update() to update score and reset paddle position if necessary, state becomes GameOver when score == scoreToWin
	int result = objects.ball.update(delta, leftPos, rightPos);
	if (result != 0)
	{
		bool endGame = false;
		if (result == -1)
			endGame = left->addPoint(scoreToWin);
		else if (result == 1)
			endGame = right->addPoint(scoreToWin);

		if (endGame)
			state = GameOver;
		
		//reset paddle locations
		objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 385, 300);
		objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 385, 300);
	}
	
	_updateAI(delta);

	return (result != 0);
}

void PongGame::setState(GameState newState)
{
	state = newState;
}



void PongGame::reset()
{
	state = InGame;

	objects.humanPlayer.score = 0;
	objects.aiPlayer.score = 0;

	objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 370, 300);
	objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 370, 300);

	objects.ball.reset();
}

void PongGame::_updateAI(float delta)
{
	sf::Vector2f ballPos = objects.ball.shape.getPosition();
	sf::Vector2f paddlePos = objects.aiPlayer.paddle.getPosition();

	//only allow ai to move paddle after ball gets close enough to make winning possible
	if (abs(paddlePos.x - ballPos.x) < _aiResponseDistance)
	{
		if (paddlePos.y < ballPos.y)
			objects.aiPlayer.movePaddle(1, delta, false);
		else if (paddlePos.y > ballPos.y)
			objects.aiPlayer.movePaddle(-1, delta, false);
	}
}