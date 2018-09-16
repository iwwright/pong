//Ian Wright 9/11/18
#include "../include/PongGame.h"
#include <iostream>


//creates pong game of appropriate type, parameters should be 0 for a normal game and 1 for special 
PongGame::PongGame(sf::Font scoreFont, short side, short type, sf::Color humanColor, sf::Color aiColor, float difficulty)
{
	//initializes text to be displayed when the ball is out of play
	font = scoreFont;

	objects.humanPlayer = Player(true, side, type, humanColor);
	objects.aiPlayer = Player(false, -1 * side, type, aiColor, difficulty);

	//creates passive shapes, which are currently just rectangles that divide the window in half
	for (int i = 0; i < numPassiveShapes; i++)
	{
		sf::RectangleShape tmp;
		tmp.setFillColor(sf::Color(185, 185, 185));
		tmp.setSize(sf::Vector2f(10, 15));
		tmp.setPosition((800 / 2.0f) - (tmp.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		objects.passiveShapes.push_back(tmp);
	}

	//creates active shapes, currently just border of the game
	for (int i = 0; i < numActiveShapes; i++)
	{
		sf::RectangleShape tmp;
		tmp.setFillColor(sf::Color(185, 185, 185));
		tmp.setSize(sf::Vector2f(800, 15));
		tmp.setPosition((800 / 2.0f) - (tmp.getLocalBounds().width / 2.0f), (600 - 15.f)*i);
		objects.activeShapes.push_back(tmp);
	}
}

PongGame::~PongGame()
{
}

void PongGame::initVars()
{
	//for whatever reason text has to be setup outside of constructor so that the font doesn't cause problems
	objects.humanPlayer.updateText(font);
	objects.aiPlayer.updateText(font);

	_notInPlayText.setFont(font);
	_notInPlayText.setString("Press Space");
	_notInPlayText.setCharacterSize(45);
	_notInPlayText.setFillColor(sf::Color::White);
	_notInPlayText.setPosition((800 / 2.f) - (_notInPlayText.getLocalBounds().width / 2.f), 150);

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
	if (_gameOver) return;

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
		objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 370, 300.f);
		objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 370, 300.f);
	}
	
	if (_gameOver)
		_makeEndGameText();

	_updateAI(delta);
}

void PongGame::draw(sf::RenderWindow &window)
{
	window.clear(sf::Color::Black);


	for (int i = 0; i < numActiveShapes; i++)
		window.draw(objects.activeShapes[i]);

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

		objects.ball.draw(window);

		if (!objects.ball.isInPlay)
			window.draw(_notInPlayText);
	}
}

bool PongGame::processInputs(float delta)
{
	if (!_gameOver)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			objects.humanPlayer.movePaddle(-1, delta);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			objects.humanPlayer.movePaddle(1, delta);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			objects.ball.isInPlay = true;
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			_reset();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return true;
	}

	return false;
	
}

void PongGame::_makeEndGameText()
{

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
	_endGameMessage.setPosition((800 / 2.f) - (_endGameMessage.getLocalBounds().width / 1.92f), 180);

	//instructions on how to reset the game
	_endGameHelp.setFont(font);
	_endGameHelp.setCharacterSize(30);
	_endGameHelp.setString("[ENTER]  - Try Again\n[ESCAPE] - Main Menu");
	_endGameHelp.setFillColor(sf::Color::White);
	_endGameHelp.setPosition((800 / 2.f) - (_endGameHelp.getLocalBounds().width / 1.9f), 315);

}

void PongGame::_reset()
{
	objects.humanPlayer.score = 0;
	objects.aiPlayer.score = 0;
	objects.humanPlayer.updateText(font);
	objects.aiPlayer.updateText(font);

	objects.humanPlayer.paddle.setPosition(400.f + objects.humanPlayer.side * 370, 300.f);
	objects.aiPlayer.paddle.setPosition(400.f + objects.aiPlayer.side * 370, 300.f);

	_gameOver = false;
	objects.ball.reset();
}

void PongGame::_updateAI(float delta)
{
	sf::Vector2f ballPos = objects.ball.shape.getPosition();
	sf::Vector2f paddlePos = objects.aiPlayer.paddle.getPosition();

	//only allow ai to move paddle after ball gets close enough to make winning possible
	if (abs(paddlePos.x - ballPos.x) < aiResponseDistance)
	{
		if (paddlePos.y < ballPos.y)
			objects.aiPlayer.movePaddle(1, delta);
		else if (paddlePos.y > ballPos.y)
			objects.aiPlayer.movePaddle(-1, delta);
	}
}