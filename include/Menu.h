//Ian Wright 9/9/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <vector>

#define numButtons 3

class Menu
{
public:
	//Menu() = default;
	~Menu();

	void init(sf::Font menuFont);
	void draw(sf::RenderWindow &window);
	int processInput(sf::Event curEvent);
	
	sf::Font font;

	void moveUp();
	void moveDown();

private:
	int _selectionIndex;
	sf::Text _buttonText[numButtons];
	sf::Text _titleText;


};