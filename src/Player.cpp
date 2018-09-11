#include "../include/Player.h"



Player::Player(bool human)
{
	//side determines which side of the playing field the player is on, -1 = left and +1 = right
	side = (human ? -1 : 1);
	
	score = 0;
	scoreText.setString("0");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(sf::Color(185, 185, 185));


}


Player::~Player()
{
}

//increments player's score, updates the text to display, and returns whether score < scoreToWin
bool Player::addPoint(int scoreToWin)
{
	score++;
	scoreText.setString(std::to_string(score));

	return (score < scoreToWin);
}

void Player::updateText(sf::Font &font)
{
	scoreText.setFont(font);
	//position can't be centered until font is set, for whatever reason the bounding box is off center so 1.5 fixes it
	scoreText.setPosition(((800 / 2.0) - (scoreText.getLocalBounds().width / 1.5)) + side*140, 30);
}