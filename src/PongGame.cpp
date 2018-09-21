//Ian Wright 9/11/18
#include "PongGame.h"
#include <iostream>


//creates pong game of appropriate type, parameters should be 0 for a normal game and 1 for special 
PongGame::PongGame(sf::Font scoreFont, short side, short type, sf::Color humanColor, sf::Color aiColor, float difficulty)
{
	//initializes text to be displayed when the ball is out of play
	font = scoreFont;

	objects.humanPlayer = Player(true, side, type, humanColor);
	objects.aiPlayer = Player(false, -1 * side, type, aiColor, difficulty);

	//make ai response distance scale with difficulty
	_aiResponseDistance = 1.25f * difficulty * 300;

	//creates passive shapes (don't affect ball) 
	for (int i = 0; i < 20; i++)
	{
		//dotted lines to divide court in half
		sf::RectangleShape center;
		center.setFillColor(sf::Color(185, 185, 185));
		center.setSize(sf::Vector2f(10, 15));
		center.setPosition(400.f - (center.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		objects.passiveShapes.push_back(center);
		//dotted lines to show left and right edges of court (useful when window size is changed)
		sf::RectangleShape left;
		left.setFillColor(sf::Color(60, 60, 60));
		left.setSize(sf::Vector2f(5, 15));
		left.setPosition(0.f - (left.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		objects.passiveShapes.push_back(left);
		sf::RectangleShape right;
		right.setFillColor(sf::Color(60, 60, 60));
		right.setSize(sf::Vector2f(5, 15));
		right.setPosition(800.f - (right.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		objects.passiveShapes.push_back(right);
	}

	//creates active shapes (ball bounces off shape)
	for (int i = 0; i < numActiveShapes; i++)
	{
		//makes top and bottom borders for court
		sf::RectangleShape tmp;
		tmp.setFillColor(sf::Color(185, 185, 185));
		tmp.setSize(sf::Vector2f(800, 15));
		tmp.setPosition(400.f - (tmp.getLocalBounds().width / 2.0f), (600 - 15.f)*i);
		objects.activeShapes.push_back(tmp);
	}
}

PongGame::~PongGame()
{
}

void PongGame::init()
{
	//for whatever reason text has to be setup outside of constructor so that the font doesn't cause problems
	objects.humanPlayer.updateText(font);
	objects.aiPlayer.updateText(font);

	//text to let player know how to make the ball start moving before each point
	_notInPlayText.setFont(font);
	_notInPlayText.setString("Press Space");
	_notInPlayText.setCharacterSize(45);
	_notInPlayText.setFillColor(sf::Color::White);
	_notInPlayText.setPosition(390.f - (_notInPlayText.getLocalBounds().width / 2.f), 150);

	//text to let player know the game is paused
	_pausedText.setFont(font);
	_pausedText.setString("GAME PAUSED");
	_pausedText.setCharacterSize(45);
	_pausedText.setFillColor(sf::Color::Cyan);
	_pausedText.setPosition(390.f - (_pausedText.getLocalBounds().width / 2.f), 150);
	//possible actions while paused
	_pausedHelp.setFont(font);
	_pausedHelp.setString("[SPACE]-Resume\n[ESC]-Quit");
	_pausedHelp.setCharacterSize(30);
	_pausedHelp.setFillColor(sf::Color::Cyan);
	_pausedHelp.setPosition(390.f - (_pausedHelp.getLocalBounds().width / 2.f), 210);

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
}


void PongGame::update(float delta, int scoreToWin)
{
	if (_gameOver || _paused)
		return;

	sf::Vector2f leftPos = left->paddle.getPosition();
	sf::Vector2f rightPos = right->paddle.getPosition();

	//uses ball.update() to update score and reset paddle position if necessary, _gameOver becomes true when score == scoreToWin
	int result = objects.ball.update(delta, leftPos, rightPos);
	if (result != 0)
	{
		if (result == -1)
			_gameOver = left->addPoint(scoreToWin);
		else if (result == 1)
			_gameOver = right->addPoint(scoreToWin);
		
		//reset paddle locations
		objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 385, 300);
		objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 385, 300);
	}
	
	if (_gameOver)
		_makeEndGameText();

	_updateAI(delta);
}

void PongGame::draw(sf::RenderWindow &window)
{
	window.clear(sf::Color::Black);
	//draw paddles and scores
	window.draw(objects.humanPlayer.paddle);
	window.draw(objects.aiPlayer.paddle);
	window.draw(objects.humanPlayer.scoreText);
	window.draw(objects.aiPlayer.scoreText);
	
	if (_gameOver)
	{
		window.draw(_endGameHelp);
		window.draw(_endGameMessage);
	}
	else
	{
		for (int i = 0; i < numPassiveShapes; i++)
			window.draw(objects.passiveShapes[i]);
		
		for (int i = 0; i < numActiveShapes; i++)
			window.draw(objects.activeShapes[i]);

		objects.ball.draw(window);

		if (!objects.ball.isInPlay && ! _paused)
			window.draw(_notInPlayText);

		if (_paused)
		{
			window.draw(_pausedText);
			window.draw(_pausedHelp);
		}
			
	}
}

bool PongGame::processInputs(float delta)
{
	//pausing stops any moving objects
	if (_paused)
		delta = 0;

	if (!_gameOver)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			objects.humanPlayer.movePaddle(-1, delta);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			objects.humanPlayer.movePaddle(1, delta);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !objects.ball.isInPlay)
		{
			objects.ball.isInPlay = true;
			_spaceHeld = true;
		}

		//only lets player toggle pause if the ball is in play
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && objects.ball.isInPlay)
		{
			//escape has to be released before toggling pause again
			if (!_spaceHeld)
			{
				_spaceHeld = true;
				_paused = !_paused;
			}
		}
		else
			_spaceHeld = false;

		//allow player to quit if game is paused
		if (_paused && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return true;
	}
	else
	{
		//restart the game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			_reset();

		//return true to signal that gameState needs to be changed to inMenu
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return true;
	}

	return false;	
}

void PongGame::pause()
{
	//only pauses game when ball is in play
	if (objects.ball.isInPlay)
		_paused = true;
}

void PongGame::_makeEndGameText()
{
	//determine winner and set endGameMessage string/color appropriately
	if (objects.humanPlayer.score > objects.aiPlayer.score) 
	{
		_endGameMessage.setString("YOU WIN!");
		_endGameMessage.setFillColor(sf::Color::Green);
	}
	else
	{
		_endGameMessage.setString("YOU LOSE!");
		_endGameMessage.setFillColor(sf::Color::Red);
	}
	_endGameMessage.setFont(font);
	_endGameMessage.setCharacterSize(80);
	_endGameMessage.setPosition(400.f - (_endGameMessage.getLocalBounds().width / 1.92f), 180);

	//instructions on how to reset the game
	_endGameHelp.setFont(font);
	_endGameHelp.setCharacterSize(30);
	_endGameHelp.setString("[ENTER]  - Try Again\n[ESCAPE] - Main Menu");
	_endGameHelp.setFillColor(sf::Color::White);
	_endGameHelp.setPosition(400.f - (_endGameHelp.getLocalBounds().width / 1.9f), 315);

}

void PongGame::_reset()
{
	objects.humanPlayer.score = 0;
	objects.aiPlayer.score = 0;
	objects.humanPlayer.updateText(font);
	objects.aiPlayer.updateText(font);

	objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 370, 300);
	objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 370, 300);

	_gameOver = false;
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
			objects.aiPlayer.movePaddle(1, delta);
		else if (paddlePos.y > ballPos.y)
			objects.aiPlayer.movePaddle(-1, delta);
	}
}