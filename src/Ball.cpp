//Ian Wright 9/15/18
#include "../include/Ball.h"
#include <iostream>

Ball::Ball(sf::Color leftCol, sf::Color rightCol)
{
	shape.setRadius(10.f);
	shape.setOrigin(sf::Vector2f(10, 10));

	_leftColor = leftCol;
	_rightColor = rightCol;

	//seed random number generator
	srand(time(0));

	this->reset();
}


Ball::~Ball()
{
}


/*resets ball's position attributes and sets the ball as out of play, side is randomly selected by default
when side = 0 the ball moves to right and when side = 1 the ball moves to the left*/
void Ball::reset(int side)
{
	shape.setPosition(sf::Vector2f(400, 300));
	_velocity = 0;

	//generates random angle in degrees between (360 - maxInitialAngle) and (360 + maxinitialAngle) 
	_direction = 360 + ((rand() % (maxInitialAngle*2 + 1)) - maxInitialAngle);
	//multiplies 180 degree offset by 0 or 1 to determine ball's direction, 0 for right 1 for left
	_direction += side * 180;
	//ensure _direction is between 0 and 359
	_direction %= 360;

	//ball is set to the same color as the paddle it is moving away from
	if (_isDirectionLeft())
		shape.setFillColor(_rightColor);
	else
		shape.setFillColor(_leftColor);

	//relatively rotate ball for easy movement
	shape.setRotation(_direction);

	//ball is not in play until user hits enter
	isInPlay = false;

}


void Ball::draw(sf::RenderWindow &window)
{
	if (isInPlay)
	{
		//draw ball's trail
	}

	//draw ball
	window.draw(shape);

}

/*Uses ball's position to determine whether a bounce should occur and then moves the ball after updating _direction,
returns -1 (add point to left player), 0 (do nothing), and 1 (add point to right player)
parameters:
delta - product of delta between gameLoop calls and gameTimeFactor
left  - position of left paddle
right - position of right paddle
*/
int Ball::update(float delta, sf::Vector2f left, sf::Vector2f right)
{
	if (isInPlay)
	{
		sf::Vector2f pos = shape.getPosition();
		if (int(pos.y) <= 25 || int(pos.y) >= 575)
			bounceY();

		if (_isDirectionLeft() && pos.x < 51 && pos.x > 49 && abs(left.y - pos.y) < 70)
			bounceX();
		else if (!_isDirectionLeft() && pos.x > 749 && pos.x < 751 && abs(right.y - pos.y) < 70)
			bounceX();

		shape.move(sf::Vector2f(cos(_directionAsRadians())  * delta * ballSpeed, sin(_directionAsRadians())  * delta * ballSpeed));

		//return value indicating what to do if someone has scored
		if (pos.x <= 2)
		{
			reset(0);
			return 1;
		}
		else if (pos.x >= 798)
		{
			reset(1);
			return -1;
		}
		else
			return 0;
	}
	else
		return 0;
}


/*flips ball's direction over X axis, changing whether the ball is moving up or down,
adds a small random perturbation (between -2 and +2 degrees) for an element of uncertainty*/
void Ball::bounceY()
{
	_direction = ((360 - _direction) + (rand() % 6) - 3) % 360;
	//rotate the shape so that any calls to shape.move are in the correct direction
	shape.setRotation(_direction);
}

/*flips ball's direction over Y axis, changing whether the ball is moving left or right,
adds a small random perturbation (between -2 and +2 degrees) for an element of uncertainty*/
void Ball::bounceX()
{
	_direction = ((180 - _direction + 360) + ((rand() % 6) - 3)) % 360;
	//rotate the shape so that any calls to shape.move are in the correct direction
	shape.setRotation(_direction);

	//make ball color the same as the paddle it just bounced off of
	if (_isDirectionLeft())
		shape.setFillColor(_rightColor);
	else
		shape.setFillColor(_leftColor);
}



double Ball::_directionAsRadians()
{
	return (_direction * M_PI / 180.0);
}

bool Ball::_isDirectionLeft()
{
	return (_direction >= 90 && _direction <= 270);
}