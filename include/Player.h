//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define scoreOffset 100
#define paddleSpeed 2

class Player
{
public:
	Player() = default;
	Player(bool human);
	~Player();

	bool addPoint(int scoreToWin);
	void updateText(sf::Font &font);
	void movePaddle(int direction, float delta);

	sf::RectangleShape paddle;
	sf::Text scoreText;
	int score;
	short side;

private:

};

