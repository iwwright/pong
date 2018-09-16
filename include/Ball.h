//Ian Wright 9/15/18
#pragma once

//includes math constants
#define _USE_MATH_DEFINES

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <cmath>
#include <random>

#define maxInitialAngle 30
#define ballSpeed 200

class Ball
{
public:
	Ball() = default;
	Ball(sf::Color leftCol, sf::Color rightCol);
	~Ball();

	void reset(int side = (rand() % 2));
	void draw(sf::RenderWindow &window);
	int update(float delta, sf::Vector2f left, sf::Vector2f right);
	void bounceY();
	void bounceX();


	bool isInPlay;
	sf::CircleShape shape;

private:
	sf::Color _leftColor;
	sf::Color _rightColor;

	float _velocity;
	int _direction;


	double _directionAsRadians();
	bool _isDirectionLeft();

};