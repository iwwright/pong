//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define paddleSpeed 150
#define accelMin 0.8f

class Player
{
public:
	Player() = default;
	Player(bool human, short side, sf::Color paddleColor, float difficulty = 1.f);
	~Player();

	bool addPoint(int scoreToWin);
	void updateText(sf::Font &font);
	void movePaddle(int direction, float delta, bool keyHeld);

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

	float _acceleration = accelMin;
	int _prevDirection = 0;
	//difficulty is a factor of the paddle speed, 1.0 by default
	float _difficulty;

};

