//Ian Wright 9/11/18
#include "../include/PongGame.h"
#include <iostream>


//creates pong game of appropriate type, parameters should be 0 for a normal game and 1 for special 
PongGame::PongGame(sf::Font scoreFont, short side, short type, sf::Color humanColor, sf::Color aiColor)
{
	font = scoreFont;
	
	objects.humanPlayer = Player(true, side, type, humanColor);
	objects.aiPlayer = Player(false, -1 * side, type, aiColor);


	//so that left player can be determined easily for update
	if (side == -1)
	{
		_isHumanLeft = true;
		objects.ball = Ball(humanColor, aiColor);
	}
	else
	{
		_isHumanLeft = false;
		objects.ball = Ball(aiColor, humanColor);
	}

	//rather than setting *left and *right above, we wait until update to set them to avoid memory changes
	_objsSet = false;


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
		tmp.setPosition((800 / 2.0f) - (tmp.getLocalBounds().width / 2.0f), (600-15.f)*i);
		objects.activeShapes.push_back(tmp);
	}
	

	if (type == 0)
	{

	}
}

PongGame::~PongGame()
{
}

void PongGame::update(float delta, float gameTimeFactor)
{
	//if the pointers *left and *right have not been set, they are assigned the appropriate address
	if (!_objsSet)
	{
		if (_isHumanLeft)
		{
			left = &objects.humanPlayer;
			right = &objects.aiPlayer;
		}
		else
		{
			left = &objects.aiPlayer;
			right = &objects.humanPlayer;
		}

		_objsSet = true;
	}
	
	sf::Vector2f leftPos = left->paddle.getPosition();
	sf::Vector2f rightPos = right->paddle.getPosition();


	int result = objects.ball.update(delta * gameTimeFactor, leftPos, rightPos);
	if (result == -1)
		left->addPoint(11);
	else if (result  == 1)
		right->addPoint(11);
	

}

void PongGame::draw(sf::RenderWindow &window)
{
	window.clear(sf::Color::Black);

	for (int i = 0; i < numPassiveShapes; i++)
		window.draw(objects.passiveShapes[i]);

	for (int i = 0; i < numActiveShapes; i++)
		window.draw(objects.activeShapes[i]);

	window.draw(objects.humanPlayer.paddle);
	window.draw(objects.aiPlayer.paddle);
	objects.humanPlayer.updateText(font);
	objects.aiPlayer.updateText(font);

	window.draw(objects.humanPlayer.scoreText);
	window.draw(objects.aiPlayer.scoreText);

	objects.ball.draw(window);

}

void PongGame::processInputs(float delta, float gameTimeFactor)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		objects.humanPlayer.movePaddle(-1, delta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		objects.humanPlayer.movePaddle(1, delta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		objects.ball.isInPlay = true;
}