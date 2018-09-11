//Ian Wright 9/11/18
#include "../include/PongGame.h"
#include <iostream>


//creates pong game of appropriate type, parameters should be 0 for a normal game and 1 for special 
PongGame::PongGame(int type, sf::Font scoreFont)
{
	font = scoreFont;

	objects.humanPlayer = Player(true);
	objects.aiPlayer = Player(false);

	for (int i = 0; i < numPassiveShapes; i++)
	{
		sf::RectangleShape tmp;
		tmp.setFillColor(sf::Color(185, 185, 185));
		tmp.setSize(sf::Vector2f(10, 15));
		tmp.setPosition((800 / 2.0) - (tmp.getLocalBounds().width / 2.0), 30 * i + 7.5);
		objects.passiveShapes.push_back(tmp);
	}

	for (int i = 0; i < numActiveShapes; i++)
	{
		sf::RectangleShape tmp;
		tmp.setFillColor(sf::Color(185, 185, 185));
		tmp.setSize(sf::Vector2f(800, 15));
		tmp.setPosition((800 / 2.0) - (tmp.getLocalBounds().width / 2.0), (600-15)*i);
		objects.activeShapes.push_back(tmp);
	}
	

	if (type == 0)
	{

	}
}

void PongGame::draw(sf::RenderWindow &window)
{
	window.clear(sf::Color::Black);

	for (int i = 0; i < numPassiveShapes; i++)
		window.draw(objects.passiveShapes[i]);

	for (int i = 0; i < numActiveShapes; i++)
		window.draw(objects.activeShapes[i]);

	//window.draw(objects.humanPlayer.paddle);
	//window.draw(objects.aiPlayer.paddle);
	objects.humanPlayer.updateText(font);
	objects.aiPlayer.updateText(font);

	window.draw(objects.humanPlayer.scoreText);
	window.draw(objects.aiPlayer.scoreText);

}