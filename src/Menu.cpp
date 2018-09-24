//Ian Wright 9/9/18
//Menu.cpp: Defines class to represent the main menu screen and handle player input

#include "Menu.h"

void Menu::init(sf::Font menuFont)
{
	font = menuFont;
	_selectionIndex = 0;
	// select the font and sets many various attributes of text
	_titleText.setFont(font); // font is a sf::Font
	_titleText.setString("PONG");
	_titleText.setCharacterSize(100); // in pixels, not points!
	_titleText.setFillColor(sf::Color::White);
	_titleText.setStyle(sf::Text::Underlined);
	_titleText.setPosition(400.f - (_titleText.getLocalBounds().width / 2.f), 50);
	//creates button for playing the normal version of the game (no bonus features)
	_buttonText[0].setFont(font);
	_buttonText[0].setString("Play Classic Mode");
	_buttonText[0].setCharacterSize(30);
	_buttonText[0].setFillColor(sf::Color::Cyan); //cyan since it is currently selected
	_buttonText[0].setPosition(400.f - (_buttonText[0].getLocalBounds().width / 2.f), 240);
	//creates button for playing the bonus version of the game
	_buttonText[1].setFont(font);
	_buttonText[1].setString("Play Custom Mode");
	_buttonText[1].setCharacterSize(30);
	_buttonText[1].setFillColor(sf::Color::White);
	_buttonText[1].setPosition(400.f - (_buttonText[1].getLocalBounds().width / 2.f), 300);
	//creates buttonText[2] button
	_buttonText[2].setFont(font);
	_buttonText[2].setString("Exit");
	_buttonText[2].setCharacterSize(30);
	_buttonText[2].setFillColor(sf::Color::White);
	_buttonText[2].setPosition(400.f - (_buttonText[2].getLocalBounds().width / 2.f), 360);

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
	//Cyan represents currently selected option
	_buttonText[_selectionIndex].setFillColor(sf::Color::Cyan);
	_move.play();
}

void Menu::moveDown()
{
	_buttonText[_selectionIndex].setFillColor(sf::Color::White);
	_selectionIndex = (_selectionIndex == numButtons - 1 ? 0 : _selectionIndex + 1);
	//Cyan represents currently selected option
	_buttonText[_selectionIndex].setFillColor(sf::Color::Cyan);
	_move.play();
}

/*Processes user input, returns an int representing the selected menu option:
-1 : do nothing
 0 : Play with default parameters
 1 : Go to option select screen
 2 : Exit the game*/
int Menu::processInput(sf::Event curEvent)
{
	/*since this function is called inside Game::_processEvents(), there is an OS delay which produces the 
	desired cursor movement on the menu*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveUp();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveDown();
	//KeyReleased so that OptionSelect screen doesn't immediately think enter has been hit
	if (curEvent.key.code == sf::Keyboard::Return && curEvent.type == sf::Event::KeyReleased)
	{
		_select.play();
		return _selectionIndex;
	}
	//default return value to do nothing
	return -1;
}
