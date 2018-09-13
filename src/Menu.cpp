//Ian Wright 9/9/18
#include "../include/Menu.h"

void Menu::draw(sf::RenderWindow &window)
{
	//creates black background for menu
	window.clear(sf::Color::Black);
	
	//draws all text/buttons
	window.draw(_titleText);
	for (int i = 0; i < numButtons; i++)
		window.draw(_buttonText[i]);

}

void Menu::initialSetup(sf::Font menuFont)
{
	font = menuFont;

	// select the font and sets many various attributes of text
	_titleText.setFont(font); // font is a sf::Font
	_titleText.setString("PONG");
	_titleText.setCharacterSize(100); // in pixels, not points!
	_titleText.setFillColor(sf::Color::White);
	_titleText.setStyle(sf::Text::Underlined);
	//centers text on screen, 800 should be replaced if window size is changed
	_titleText.setPosition((800 / 2.f) - (_titleText.getLocalBounds().width / 2.f), 50);

	//creates button for playing the normal version of the game (no bonus features)
	_buttonText[0].setFont(font);
	_buttonText[0].setString("Play Classic Mode");
	_buttonText[0].setCharacterSize(30);
	_buttonText[0].setFillColor(sf::Color::Cyan); //cyan since it is currently selected
	_buttonText[0].setPosition((800 / 2.f) - (_buttonText[0].getLocalBounds().width / 2.f), 240);

	//creates button for playing the bonus version of the game
	_buttonText[1].setFont(font);
	_buttonText[1].setString("Play Special Mode");
	_buttonText[1].setCharacterSize(30);
	_buttonText[1].setFillColor(sf::Color::White);
	_buttonText[1].setPosition((800 / 2.f) - (_buttonText[1].getLocalBounds().width / 2.f), 300);

	//creates buttonText[2] button
	_buttonText[2].setFont(font);
	_buttonText[2].setString("Exit");
	_buttonText[2].setCharacterSize(30);
	_buttonText[2].setFillColor(sf::Color::White);
	_buttonText[2].setPosition((800 / 2.f) - (_buttonText[2].getLocalBounds().width / 2.f), 360);

	selectionIndex = 0;
}



void Menu::moveUp()
{
	_buttonText[selectionIndex].setFillColor(sf::Color::White);
	selectionIndex = (selectionIndex ? selectionIndex : numButtons) - 1;

	_buttonText[selectionIndex].setFillColor(sf::Color::Cyan);
}

void Menu::moveDown()
{
	_buttonText[selectionIndex].setFillColor(sf::Color::White);
	selectionIndex = (selectionIndex == numButtons - 1 ? 0 : selectionIndex + 1);

	_buttonText[selectionIndex].setFillColor(sf::Color::Cyan);
}
