//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "Player.h"
#include <vector>

#define numPassiveShapes 20
#define numActiveShapes 2
#define aiResponseDistance 150

class PongGame
{
public:
	PongGame() = default;
	PongGame(sf::Font scoreFont, short side, short type, sf::Color humanColor, sf::Color aiColor, float difficulty=1.f);
	~PongGame();

	void initVars();

	void draw(sf::RenderWindow &window);
	void update(float delta, int scoreToWin);
	bool processInputs(float delta);

	sf::Font font;
	
	//pointers to the Player object on the corresponding side of the court, assigned in initVars() since Game::_pong is static
	Player *left;
	Player *right;
	
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

private:
	//various strings of text that show up at different points during the game, initialized in initVars()
	sf::Text _notInPlayText;
	sf::Text _endGameMessage;
	sf::Text _endGameHelp;

	//stores whether game has ended
	bool _gameOver = false;

	void _makeEndGameText();
	void _reset();
	void _updateAI(float delta);

	//void normalGameSetup();
	//void specialGameSetup();

};