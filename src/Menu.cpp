//Ian Wright 9/9/18
#include "Menu.h"

Menu::~Menu()
{
}

void Menu::init(sf::Font menuFont)
{
	font = menuFont;

	// select the font and sets many various attributes of text
	_titleText.setFont(font); // font is a sf::Font
	_titleText.setString("PONG");
	_titleText.setCharacterSize(100); // in pixels, not points!
	_titleText.setFillColor(sf::Color::White);
	_titleText.setStyle(sf::Text::Underlined);
	//centers text on screen, 800 should be replaced if window size is changed
	_titleText.setPosition(400.f - (_titleText.getLocalBounds().width / 2.f), 50);

	//creates button for playing the normal version of the game (no bonus features)
	_buttonText[0].setFont(font);
	_buttonText[0].setString("Play Classic Mode");
	_buttonText[0].setCharacterSize(30);
	_buttonText[0].setFillColor(sf::Color::Cyan); //cyan since it is currently selected
	_buttonText[0].setPosition(400.f - (_buttonText[0].getLocalBounds().width / 2.f), 240);

	//creates button for playing the bonus version of the game
	_buttonText[1].setFont(font);
	_buttonText[1].setString("Play Special Mode");
	_buttonText[1].setCharacterSize(30);
	_buttonText[1].setFillColor(sf::Color::White);
	_buttonText[1].setPosition(400.f - (_buttonText[1].getLocalBounds().width / 2.f), 300);

	//creates buttonText[2] button
	_buttonText[2].setFont(font);
	_buttonText[2].setString("Exit");
	_buttonText[2].setCharacterSize(30);
	_buttonText[2].setFillColor(sf::Color::White);
	_buttonText[2].setPosition(400.f - (_buttonText[2].getLocalBounds().width / 2.f), 360);

	_selectionIndex = 0;
}

void Menu::draw(sf::RenderWindow &window)
{
	//creates black background for menu
	window.clear(sf::Color::Black);
	
	//draws all text/buttons
	window.draw(_titleText);
	for (int i = 0; i < numButtons; i++)
		window.draw(_buttonText[i]);

}


void Menu::moveUp()
{
	_buttonText[_selectionIndex].setFillColor(sf::Color::White);
	_selectionIndex = (_selectionIndex ? _selectionIndex : numButtons) - 1;

	_buttonText[_selectionIndex].setFillColor(sf::Color::Cyan);
}

void Menu::moveDown()
{
	_buttonText[_selectionIndex].setFillColor(sf::Color::White);
	_selectionIndex = (_selectionIndex == numButtons - 1 ? 0 : _selectionIndex + 1);

	_buttonText[_selectionIndex].setFillColor(sf::Color::Cyan);
}

int Menu::processInput(sf::Event curEvent)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveUp();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveDown();

	if (curEvent.key.code == sf::Keyboard::Enter && curEvent.type == sf::Event::KeyReleased)
		return _selectionIndex;

	return -1;

}
