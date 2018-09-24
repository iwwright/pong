//Ian Wright 9/11/18
//Player.cpp: defines class to represent each player, has score and paddle

#include "Player.h"

Player::Player(bool human, short argSide, sf::Color argColor, float difficulty)
{
	//assigns passed in values to instance variables for object
	_human = human;
	side = argSide;
	_difficulty = difficulty;
	paddleColor = argColor;
	velocity = 0.f;

	//initializes score and scoreText, always uses a light grey for visibility
	score = 0;
	scoreText.setString("0");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(sf::Color(185, 185, 185));
	//paddle settings
	paddle.setFillColor(paddleColor);
	paddle.setSize(sf::Vector2f(15, 140));
	paddle.setOrigin(sf::Vector2f(7.5f, 70));
	paddle.setPosition(400.f + side * 385, 300);
}

//increments player's score, updates the text to display, and returns whether score == scoreToWin
bool Player::addPoint(int scoreToWin)
{
	score = score + 1;
	return (score == scoreToWin);
}

//allows score text to be updated from PlayerView, helps avoid issues with the scope of the font
void Player::updateText(sf::Font &font)
{
	scoreText.setFont(font);
	scoreText.setString(std::to_string(score));
	//position can't be centered until font is set, for whatever reason the bounding box is off center so 1.5 fixes it
	scoreText.setPosition((400.f - (scoreText.getLocalBounds().width / 1.5f)) + side*140, 30);
}

/*moves the player's paddle up (direction == -1) or down (direction == +1)
*/
void Player::movePaddle(int direction, float delta, bool keyHeld)
{
	sf::Vector2f pos = paddle.getPosition();
	//edges of court are 15px and from paddle origin to edge is 70px so only move paddle if it is in between 85 and 515
	if (pos.y >= 85 && pos.y <= 515)
	{
		if (keyHeld)
			_acceleration = (_acceleration < accelMax) ? _acceleration + 0.001f: accelMax;
		else
			_acceleration = accelMin;
		//set velocity so that ball trajectory can be calculated based on paddle speed/direction
		velocity = direction * paddleSpeed  * _acceleration;
		float newPos = pos.y + velocity * delta * _difficulty;

		if (newPos < 85)
			newPos = 85;
		else if (newPos > 515)
			newPos = 515;

		paddle.setPosition(pos.x, newPos);
	}
}