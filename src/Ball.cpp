//Ian Wright 9/15/18
//Ball.cpp: Class representing ball, starts in center of court and doesn't move until Player hits space

#include "Ball.h"

Ball::Ball(sf::Color leftCol, sf::Color rightCol)
{
	shape.setRadius(10);
	shape.setOrigin(sf::Vector2f(10, 10));

	_leftColor = leftCol;
	_rightColor = rightCol;

	//sets ball velocity
	_velocity = 160;

	//seed random number generator
	srand((int)time(0));

	//allow ball trail to be drawn
	trail.enabled = true;

	this->reset();
}

void Ball::initSounds()
{
	//loads bounce sounds
	if (!_bounceBuffer.loadFromFile("../src/assets/boop.wav"))
		throw;
	_bounce.setBuffer(_bounceBuffer);

}


/*resets ball's position attributes and sets the ball as out of play, side is randomly selected by default
when side = 0 the ball moves to right and when side = 1 the ball moves to the left.
side should correspond to the player that just scored if called after a point is scored*/
void Ball::reset(int side)
{
	shape.setPosition(sf::Vector2f(400, 300));
	_acceleration = 1.f;

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
	shape.setRotation(float(_direction));

	//ball is not in play until user hits enter
	isInPlay = false;

	//perform initial steps for ball trail
	if (trail.enabled)
	{
		for (int i = 0; i < 5; i++)
		{
			trail.coordinates[i] = sf::Vector2f(-10.0f, -10.0f);
			trail.shapes[i].setFillColor(sf::Color(255, 255, 255, 175 - 30 * i));
			trail.shapes[i].setRadius(10);
			trail.shapes[i].setOrigin(sf::Vector2f(10, 10));
			trail.shapes[i].setPosition(400.f, 300);
		}
	}

}

void Ball::draw(sf::RenderWindow &window)
{
	if (isInPlay && trail.enabled)
	{
		//draws all balls in trail except 0th since it is used to store main ball's location
		for (int i = 1; i < 5; i++)
			window.draw(trail.shapes[i]);
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
int Ball::update(float delta, sf::Vector2f left, sf::Vector2f right , float leftVelocity, float rightVelocity)
{
	if (isInPlay)
	{
		sf::Vector2f pos = shape.getPosition();

		if (pos.y <= 25 || pos.y >= 575)
			bounceY();

		if (_isDirectionLeft() && pos.x < 33 && pos.x > 25 && abs(left.y - pos.y) < 85.f) //paddle height is 70 from origin with a ball height of 10 so this is 85 just so the game is more fun
			bounceX(leftVelocity);
		
		else if (!_isDirectionLeft() && pos.x > 767 && pos.x < 773 && abs(right.y - pos.y) < 85.f)
			bounceX(rightVelocity);

		//now that color is updated through bounce, update trail if it's enabled and if the ball has moved far enough
		if (trail.enabled && _distance(pos, trail.coordinates[0]) > 4.f * _acceleration)
			_trailUpdate(pos);

		//move ball in new direction
		shape.move(float(cos(_directionAsRadians())  * delta * _velocity * _acceleration), float(sin(_directionAsRadians())  * delta * _velocity * _acceleration));
		pos = shape.getPosition();

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
removed randomness to avoid the ball going backwards*/
void Ball::bounceY()
{
	//_direction = ((720 - _direction) + (rand() % 11) - 5) % 360;
	_direction = (720 - _direction) % 360;

	//limits how close direction can be to 90 and 270
	if (_isDirectionLeft())
	{
		if (_direction < 100)
			_direction = 100;
		if (_direction > 260)
			_direction = 260;
	}
	else
	{
		if (_direction > 80 && _direction < 91)
			_direction = 80;
		if (_direction > 269 && _direction < 280)
			_direction = 280;
	}
	//avoids bugs with multiple bounces
	sf::Vector2f pos = shape.getPosition();
	if (pos.y < 25)
		shape.setPosition(pos.x, 25.f);
	else if (pos.y > 575)
		shape.setPosition(pos.x, 575.f);

	//rotate the shape so that any calls to shape.move are in the correct direction
	shape.setRotation(float(_direction));

	_bounceSound();
}

/*flips ball's direction over Y axis, changing whether the ball is moving left or right,
adds a small random perturbation (between -5 and +5 degrees) for an element of uncertainty*/
void Ball::bounceX(float velocity)
{
	//computes direction of ball without paddle velocity
	_direction = (180 - _direction + 360) + ((rand() % 11) - 5);
	//uses paddle speed/direction to affect angle 
	_direction = int(((velocity * maxBounceAngle) / (paddleSpeed * accelMax)) + _direction) % 360;

	//rotate the shape so that any calls to shape.move are in the correct direction
	shape.setRotation(float(_direction));

	//make ball color the same as the paddle it just bounced off of
	if (_isDirectionLeft())
		shape.setFillColor(_rightColor);
	else
		shape.setFillColor(_leftColor);

	//increases acceleration so that game speeds up with each successful paddle hit until it reaches 2.0
	if (_acceleration < 2.f) 
		_acceleration += 0.05f;

	_bounceSound();
}

//plays sound when ball bounces, cycles through a set of pitches so ijt isn't just a single repetitive sound
void Ball::_bounceSound()
{
	_bounce.setPitch(_pitches[_pitchIndex]);
	_bounce.play();
	_pitchIndex = (_pitchIndex == 7) ? 0 : _pitchIndex + 1;
}

//updates coordinates of the ball's trail and the color of each part
void Ball::_trailUpdate(sf::Vector2f ballPos)
{
	sf::Color ballCol = shape.getFillColor();

	for (int i = 4; i > -1; i--)
	{
		if (i == 0)
		{
			trail.coordinates[i] = ballPos;
			trail.shapes[i].setPosition(ballPos);
		}
		else
		{
			trail.coordinates[i] = trail.coordinates[i-1];
			trail.shapes[i].setPosition(trail.coordinates[i]);
			ballCol.a = 150 - 30 * i;
			trail.shapes[i].setFillColor(ballCol);
		}
	}
}

//allows you to turn the ball trail on/off but it should be on since the trail is cool
bool Ball::toggleTrail()
{
	trail.enabled = !(trail.enabled);
	return trail.enabled;
}

//converts _direction into radians for the <cmath> functions
double Ball::_directionAsRadians()
{
	return (_direction * M_PI / 180.0);
}

//returns true if the ball is going to the left and false otherwise
bool Ball::_isDirectionLeft()
{
	return (_direction >= 90 && _direction <= 270);
}

//returns euclidean distance between to points in the court, result is in pixels
float Ball::_distance(sf::Vector2f u, sf::Vector2f v)
{
	return sqrt(pow(u.x - v.x, 2) + pow(u.y - v.y, 2));
}