//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include <vector>

#define scoreToWin 11
#define numPassiveShapes 20
#define numActiveShapes 2

class PongGame
{
public:
	PongGame() = default;
	PongGame(int type, sf::Font scoreFont);
	void draw(sf::RenderWindow &window);
	sf::Font font;

	struct PongObjects 
	{
		//shapes to be ignored by the ball
		std::vector<sf::RectangleShape> passiveShapes;

		//shapes the ball bounces off of
		std::vector<sf::RectangleShape> activeShapes;

		Player humanPlayer;
		Player aiPlayer;

	} objects;


private:
	//void normalGameSetup();
	//void specialGameSetup();

};