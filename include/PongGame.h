//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "Player.h"
#include <vector>

#define numPassiveShapes 20
#define numActiveShapes 2

class PongGame
{
public:
	PongGame() = default;
	PongGame(sf::Font scoreFont, short side, short type, sf::Color humanColor, sf::Color aiColor);
	~PongGame();

	void draw(sf::RenderWindow &window);
	void update(float delta, float gameTimeFactor);
	void processInputs(float delta, float gameTimeFactor);

	struct PongObjects 
	{
		Ball ball;

		//shapes to be ignored by the ball
		std::vector<sf::RectangleShape> passiveShapes;

		//shapes the ball bounces off of
		std::vector<sf::RectangleShape> activeShapes;

		Player humanPlayer;
		Player aiPlayer;
	} objects;

	sf::Font font;

	Player *left;
	Player *right;
	

private:
	//true if human player's paddle is on the left side of the screen
	bool _isHumanLeft;
	//true if the pointers *left and *right have been set
	bool _objsSet;
	//void normalGameSetup();
	//void specialGameSetup();

};