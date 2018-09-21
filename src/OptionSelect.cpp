//Ian Wright 9/18/18
#include "OptionSelect.h"

OptionSelect::~OptionSelect()
{
}

void OptionSelect::init(sf::Font menuFont)
{
	font = menuFont;
	_selectionIndex = 0;

	//prepares colors array
	_colors[0] = sf::Color::White;
	_colors[1] = sf::Color::Red;
	_colors[2] = sf::Color::Magenta;
	_colors[3] = sf::Color::Blue;
	_colors[4] = sf::Color::Green;
	_colors[5] = sf::Color::Yellow;

	//specific strings for each member of _allOptions, starting with the player's paddle color
	_allOptions[0].name.setString("Your Color:");
	_allOptions[0].choices.push_back("< White >");
	_allOptions[0].choices.push_back("< Red >");
	_allOptions[0].choices.push_back("< Magenta >");
	_allOptions[0].choices.push_back("< Blue >");
	_allOptions[0].choices.push_back("< Green >");
	_allOptions[0].choices.push_back("< Yellow >");
	//settings for opponent's paddle color
	_allOptions[1].name.setString("Opponent's Color:");
	_allOptions[1].choices.push_back("< White >");
	_allOptions[1].choices.push_back("< Red >");
	_allOptions[1].choices.push_back("< Magenta >");
	_allOptions[1].choices.push_back("< Blue >");
	_allOptions[1].choices.push_back("< Green >");
	_allOptions[1].choices.push_back("< Yellow >");
	//settings for difficulty
	_allOptions[2].name.setString("Difficulty:");
	_allOptions[2].choices.push_back("< Easy >");
	_allOptions[2].choices.push_back("< Medium >");
	_allOptions[2].choices.push_back("< Hard >");
	_allOptions[2].choices.push_back("< Impossible >");
	//settings for game speed
	_allOptions[3].name.setString("Game Speed:");
	_allOptions[3].choices.push_back("< 0.5 >");
	_allOptions[3].choices.push_back("< 1.0 >");
	_allOptions[3].choices.push_back("< 1.5 >");
	_allOptions[3].choices.push_back("< 2.0 >");
	//settings for which side of the screen is the player's
	_allOptions[4].name.setString("Your Side:");
	_allOptions[4].choices.push_back("< Left >");
	_allOptions[4].choices.push_back("< Right >");

	//performs all common steps for each member of _allOptions
	for (int i = 0; i < numOptions; i++)
	{
		_allOptions[i].height = 180.f + 50 * i;
		_allOptions[i].index = 0;

		_allOptions[i].name.setFont(font);
		_allOptions[i].name.setCharacterSize(20);
		_allOptions[i].name.setFillColor(sf::Color::White);
		_allOptions[i].name.setOrigin(_allOptions[i].name.getLocalBounds().width, 0);
		_allOptions[i].name.setPosition(350.f, _allOptions[i].height);

		_allOptions[i].selection.setFont(font);
		_allOptions[i].selection.setCharacterSize(20);
		_allOptions[i].selection.setFillColor(sf::Color::White);
		_allOptions[i].selection.setPosition(450.f, _allOptions[i].height);
		_allOptions[i].selection.setString(_allOptions[i].choices.at(0));

	}
	//set the first options to cyan so that it is clear the player starts at the top
	_allOptions[0].name.setFillColor(sf::Color::Cyan);
	_allOptions[0].selection.setFillColor(sf::Color::Cyan);

	//instructions on how to start the game or return to the menu
	_instructions.setFont(font);
	_instructions.setCharacterSize(30);
	_instructions.setFillColor(sf::Color::White);
	_instructions.setString("[ENTER] = Start\n[ESC] Back");
	_instructions.setPosition(400.f - (_instructions.getLocalBounds().width / 2.f), 50);

}



void OptionSelect::draw(sf::RenderWindow &window)
{
	//creates black background for menu
	window.clear(sf::Color::Black);

	//draws all text/buttons
	window.draw(_instructions);
	for (int i = 0; i < numOptions; i++)
	{
		window.draw(_allOptions[i].name);
		window.draw(_allOptions[i].selection);
	}

}

int OptionSelect::processInput(sf::Event curEvent)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveVertical(-1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveVertical(1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		moveHorizontal(1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		moveHorizontal(-1);

	if (curEvent.key.code == sf::Keyboard::Enter && curEvent.type == sf::Event::KeyPressed)
		return 0;

	if (curEvent.key.code == sf::Keyboard::Escape && curEvent.type == sf::Event::KeyPressed)
		return 1;

	return -1;
}

void OptionSelect::moveVertical(int direction)
{
	//resets color of selected option to white
	_allOptions[_selectionIndex].name.setFillColor(sf::Color::White);
	_allOptions[_selectionIndex].selection.setFillColor(sf::Color::White);

	//direction has to be -1 or +1
	_selectionIndex = (_selectionIndex + direction + numOptions) % numOptions;

	//sets the currently selected option to cyan
	_allOptions[_selectionIndex].name.setFillColor(sf::Color::Cyan);
	_allOptions[_selectionIndex].selection.setFillColor(sf::Color::Cyan);
}

void OptionSelect::moveHorizontal(int direction)
{
	//get number of choices as int
	int numChoices = static_cast<int>(_allOptions[_selectionIndex].choices.size());

	//direction has to be -1 or +1
	_allOptions[_selectionIndex].index = (_allOptions[_selectionIndex].index + direction + numChoices) % numChoices;

	//update the selection text to display the new value
	_allOptions[_selectionIndex].selection.setString(_allOptions[_selectionIndex].choices.at(_allOptions[_selectionIndex].index));
}

sf::Color OptionSelect::getHumanColor()
{
	return _colors[_allOptions[0].index];
}

sf::Color OptionSelect::getAIColor()
{
	return _colors[_allOptions[1].index];
}

float OptionSelect::getDifficulty()
{
	
	return (_allOptions[2].index * 0.5f + 0.5f);
}

float OptionSelect::getSpeed()
{
	return (_allOptions[3].index * 0.5f + 0.5f);
}

int OptionSelect::getSide()
{
	return (2 * _allOptions[4].index - 1);
}