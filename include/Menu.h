//Ian Wright 9/9/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define numButtons 3

class Menu
{
public:
	//Menu() = default;
	~Menu();

	void initialSetup(sf::Font menuFont);
	void draw(sf::RenderWindow &window);

	int selectionIndex;
	sf::Font font;

	void moveUp();
	void moveDown();

private:
	sf::Text _buttonText[numButtons];
	sf::Text _titleText;
};