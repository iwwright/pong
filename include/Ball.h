//Ian Wright 9/15/18
#pragma once

//includes math constants
#define _USE_MATH_DEFINES

#include "SFML/Audio.hpp"
#include "Player.h" //just for paddle speed and acceleration constants
#include <cmath>
#include <random>

#define maxInitialAngle 25
#define maxBounceAngle 70 //maximum possible value to add/subtract to balls reflected angle based on paddle velocity

class Ball
{
public:
	Ball() = default;
	~Ball() = default;
	Ball(sf::Color leftCol, sf::Color rightCol);

	void reset(int side = (rand() % 2));
	void draw(sf::RenderWindow &window);
	int update(float delta, sf::Vector2f left, sf::Vector2f right, float leftVelocity, float rightVelocity);
	void bounceY();
	void bounceX(float velocity);
	bool toggleTrail();

	void initSounds();

	bool isInPlay;
	sf::CircleShape shape;
	//trail behind the ball because it looks cool and also helps show ball speed
	struct BallTrail
	{
		bool enabled;
		sf::Vector2f coordinates[5];
		sf::CircleShape shapes[5];

	} trail;

	void _bounceSound();

private:
	sf::Color _leftColor;
	sf::Color _rightColor;

	int _velocity;
	float _acceleration;
	int _direction;

	//different pitched sounds to be played when the ball bounces
	sf::SoundBuffer _bounceBuffer;
	sf::Sound _bounce;
	//pitches make a Cmaj7 arpeggio
	float _pitches[8] = { 0.50f, 0.62f, 0.74f, 0.93f, 1.0f, 0.93f, 0.74f, 0.62f};
	int _pitchIndex = 0;

	double _directionAsRadians();
	bool _isDirectionLeft();
	void _trailUpdate(sf::Vector2f ballPos);
	float _distance(sf::Vector2f u, sf::Vector2f v);
};