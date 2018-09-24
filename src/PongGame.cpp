//Ian Wright 9/11/18
//PongGame.cpp: defines class to represent game logic, stores all objects to be used in pong and updates AI

#include "PongGame.h"

PongGame::PongGame()
{
	//game should start at menu
	state = InMenu;
}

/*Assigns instance variables to passed arguments, has to be called before state becomes InGame */
void PongGame::updateParameters(float speed, short side, sf::Color humanColor, sf::Color aiColor, float difficulty)
{
	_gameTimeFactor = speed;
	//creates Player objects based on arguments
	objects.humanPlayer = Player(true, side, humanColor);
	objects.aiPlayer = Player(false, -1 * side, aiColor, difficulty);

	//make ai response distance scale with difficulty
	_aiResponseDistance = 1.25f * difficulty * 400;

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

/*Called in Game::_gameLoop(), updates relevant objects based on elapsed time (delta) and returns a boolean
indicating whether either player has scored so that _playerView can be updated */
bool PongGame::update(float delta, int scoreToWin)
{
	delta *= _gameTimeFactor;
	//none of the PongObjects exist until state == InGame
	if (state != InGame)
		return false;

	//uses ball.update() to update score and reset paddle position if necessary, state becomes GameOver when score == scoreToWin
	int result = objects.ball.update(delta, left->paddle.getPosition(), right->paddle.getPosition(), left->velocity, right->velocity);
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

//resets the game if the player decides to play another game
void PongGame::reset()
{
	state = InGame;

	objects.humanPlayer.score = 0;
	objects.aiPlayer.score = 0;

	objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 370, 300);
	objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 370, 300);

	objects.ball.reset();
}

//allows PlayerView and Game to change state
void PongGame::setState(GameState newState)
{
	state = newState;
}

/*Represents the AI view, only moves AI paddle when the ball is within the aiResponseDistance, note that
AI paddle cannot accelerate but wuhen difficulty > 1 the paddle has a higher constant velocity than the player.*/
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
		else
			objects.aiPlayer.velocity = 0.f;
	}
	else
		objects.aiPlayer.velocity = 0.f;
}