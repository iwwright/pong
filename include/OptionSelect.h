//Ian Wright 9/18/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define numOptions 5

class OptionSelect
{
public:
	OptionSelect() = default;
	~OptionSelect();

	void init(sf::Font menuFont);
	void draw(sf::RenderWindow &window);
	int processInput(sf::Event curEvent);

	sf::Font font;

	void moveVertical(int direction);
	void moveHorizontal(int direction);

	//get functions to retrieve selected options in GameApplication.cpp
	sf::Color getHumanColor();
	sf::Color getAIColor();
	float getDifficulty();
	float getSpeed();
	int getSide();


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

};