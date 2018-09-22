//Ian Wright 9/11/18
#include "Player.h"

Player::Player(bool human, short argSide, short paddleType, sf::Color argColor, float difficulty)
{
	//assigns passed in values to instance variables for object
	_human = human;
	side = argSide;
	_paddleType = paddleType;
	_difficulty = difficulty;
	paddleColor = argColor;

	//initializes score and scoreText, always uses a light grey for visibility
	score = 0;
	scoreText.setString("0");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(sf::Color(185, 185, 185));

	paddle.setFillColor(paddleColor);
	paddle.setSize(sf::Vector2f(15, 140));
	paddle.setOrigin(sf::Vector2f(7.5f, 70));
	paddle.setPosition(400.f + side * 385, 300);

}


Player::~Player()
{
}

//increments player's score, updates the text to display, and returns whether score == scoreToWin
bool Player::addPoint(int scoreToWin)
{
	score = score + 1;
	scoreText.setString(std::to_string(score));
	//position can't be centered until font is set, for whatever reason the bounding box is off center so 1.5 fixes it
	scoreText.setPosition((400.f - (scoreText.getLocalBounds().width / 1.5f)) + side * 140, 30);

	return (score == scoreToWin);
}

void Player::updateText(sf::Font &font)
{
	scoreText.setFont(font);
	scoreText.setString(std::to_string(score));
	//position can't be centered until font is set, for whatever reason the bounding box is off center so 1.5 fixes it
	scoreText.setPosition((400.f - (scoreText.getLocalBounds().width / 1.5f)) + side*140, 30);
}

void Player::movePaddle(int direction, float delta, bool keyHeld)
{
	sf::Vector2f pos = paddle.getPosition();
	if (pos.y >= 85 && pos.y <= 515)
	{
		if (keyHeld)
			_acceleration = (_acceleration < 3.f) ? _acceleration + 0.001f: 3.f;
		else
			_acceleration = 0.8f;
	

		float newPos = pos.y + direction * paddleSpeed * delta * _difficulty * _acceleration;

	
		if (newPos < 85)
			newPos = 85;
		else if (newPos > 515)
			newPos = 515;

		paddle.setPosition(pos.x, newPos);
	}
}