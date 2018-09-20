//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define paddleSpeed 200

class Player
{
public:
	Player() = default;
	Player(bool human, short side, short paddleType, sf::Color paddleColor, float difficulty = 1.f);
	~Player();

	bool addPoint(int scoreToWin);
	void updateText(sf::Font &font);
	void movePaddle(int direction, float delta);

	sf::RectangleShape paddle;
	sf::Color paddleColor;
	float velocity;

	//int to store the player's score and Text object to display on the screen
	int score;
	sf::Text scoreText;
	sf::Font scoreFont;

	//represents which side of the court the belongs to the player, -1 = left and 1 = right
	short side;


private:
	//whether played is human controlled or not
	bool _human;
	//represents type of paddle, 0 = constant velocity paddle and 1 = paddle with acceleration
	short _paddleType;
	//difficulty is a factor of the paddle speed, 1.0 by default
	float _difficulty;

};

