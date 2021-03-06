//Ian Wright 9/18/18
#pragma once

#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define numOptions 5

class OptionSelect
{
public:
	OptionSelect() = default;
	~OptionSelect() = default;

	void init(sf::Font menuFont);
	void draw(sf::RenderWindow &window);
	int processInput(sf::Event curEvent);
	void moveVertical(int direction);
	void moveHorizontal(int direction);
	//get functions to retrieve selected options in PlayerView.cpp
	sf::Color getHumanColor();
	sf::Color getAIColor();
	float getDifficulty();
	float getSpeed();
	int getSide();

	sf::Font font;

	//used to represent each user changeable parameter for the PongGame
	struct Option
	{
		sf::Text name;
		sf::Text selection;
		std::vector<std::string> choices;

		int index;
		float height;
	};

private:
	int _selectionIndex;
	sf::Text _instructions;
	sf::Color _colors[6];
	Option _allOptions[numOptions];

	//Sound/Buffer for menu movement
	sf::SoundBuffer _moveBuffer;
	sf::Sound _move;
	//Sound/Buffer for selecting an option
	sf::SoundBuffer _selectBuffer;
	sf::Sound _select;
	//Sound/Buffer for going back in the menu
	sf::SoundBuffer _backBuffer;
	sf::Sound _back;
};