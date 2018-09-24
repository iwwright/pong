//Ian Wright 9/9/18

#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <vector>

#define numButtons 3

class Menu
{
public:
	Menu() = default;
	~Menu() = default;

	void init(sf::Font menuFont);
	void draw(sf::RenderWindow &window);
	int processInput(sf::Event curEvent);
	void moveUp();
	void moveDown();
	
	sf::Font font;
private:
	int _selectionIndex;
	sf::Text _buttonText[numButtons];
	sf::Text _titleText;

	//sound variables for menu movement and selecting an option
	sf::SoundBuffer _moveBuffer;
	sf::Sound _move;
	sf::SoundBuffer _selectBuffer;
	sf::Sound _select;
};