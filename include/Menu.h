//Ian Wright 9/9/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <vector>

#define numButtons 3

class Menu
{
public:
	void draw(sf::RenderWindow &window);
	void initialSetup();
	int selectionIndex;

	void moveUp();
	void moveDown();

private:
	sf::Font _font;
	sf::Text _buttonText[numButtons];
	sf::Text _titleText;
};