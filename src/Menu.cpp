//Ian Wright 9/9/18
#include "../include/Menu.h"

Menu::MenuTask Menu::draw(sf::RenderWindow &window)
{
	//creates black background for menu
	window.clear(sf::Color::Black);

	//load font
	sf::Font font;
	if (!font.loadFromFile("../src/assets/AtariClassic-Regular.ttf"))
	{
		// TODO add error...
		throw;
	}

	sf::Text text;
	// select the font
	text.setFont(font); // font is a sf::Font
	// set the string to display
	text.setString("PONG");
	// set the character size
	text.setCharacterSize(48); // in pixels, not points!
	// set the color
	text.setFillColor(sf::Color::White);
	// set the text style
	text.setStyle(sf::Text::Underlined);

	window.draw(text);

	//TODO change to function call for menu interaction
	return None;
}