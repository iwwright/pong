//Ian Wright 9/9/18
#include "Menu.h"

#include <iostream>

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

	//load all sounds into buffers
	if (!_moveBuffer.loadFromFile("../src/assets/menuMove.wav"))
		throw;
	if (!_selectBuffer.loadFromFile("../src/assets/menuSelect.wav"))
		throw;

	//set buffer for each sound object
	_move.setBuffer(_moveBuffer);
	_select.setBuffer(_selectBuffer);
	_select.setVolume(70.f);

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
	_move.play();
}

void Menu::moveDown()
{
	_buttonText[_selectionIndex].setFillColor(sf::Color::White);
	_selectionIndex = (_selectionIndex == numButtons - 1 ? 0 : _selectionIndex + 1);

	_buttonText[_selectionIndex].setFillColor(sf::Color::Cyan);
	_move.play();
}

int Menu::processInput(sf::Event curEvent)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveUp();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveDown();

	if (curEvent.key.code == sf::Keyboard::Enter && curEvent.type == sf::Event::KeyReleased)
	{
		_select.play();
		return _selectionIndex;
	}
	//default return value to do nothing
	return -1;
}
