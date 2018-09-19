//Ian Wright 9/9/18
#include "GameApplication.h"
#include <iostream>

void Game::gameLoop()
{
	_clock.restart(); // starts the clock

	//call game loop until the state is exiting, then close
	while (!isExiting())
	{
		processInput();
		updateView();
	}
	

}

void Game::init(void)
{

	//continue only if the game has not been initialized
	if (_gameState != Uninitialized)
		return;

	//load font
	if (!font.loadFromFile("../src/assets/AtariClassic-Regular.ttf"))
		throw;

	//create main window
	_window.create(sf::VideoMode(800, 600, 32), "Pong", sf::Style::Close);
	_gameState = Game::InMenu;
	_mainMenu.init(Game::font);

	//calls the game loop after initialization
	 gameLoop();

	_window.close();
}

bool Game::isExiting()
{
	//determine if the gameState is exiting
	return (_gameState == Game::Exiting);
}

void Game::processInput()
{
	sf::Event Event;

	while (_window.pollEvent(Event))
	{
		switch (_gameState)
		{

		case Game::InMenu:
		{
			switch (_mainMenu.processInput(Event))
			{
			case -1:
				//do nothing in this case
				break;
			case 0:
				//classic mode has been selected so start game with certain parameters
				_gameState = Game::Playing;
				gameTimeFactor = 2.f;

				_pong = PongGame(Game::font, -1, 0, sf::Color::Blue, sf::Color::Red, 0.5f);
				_pong.init();
				break;
			case 1:
				//special mode is selected so change state to display option select
				_gameState = Game::Options;
				_options.init(Game::font);
				break;
			case 2:
				//exit has been selected so quit the game
				_gameState = Game::Exiting;
				break;
			}
			break;
		}
		case Game::Options:
		{
			switch (_options.processInput(Event))
			{
			case -1:
				//do nothing in this case
				break;
			case 0:
				//Enter has been hit so gather parameters and start game
				_gameState = Game::Playing;
				//update gameTimeFactor with speed parameter
				gameTimeFactor = 2.f * _options.getSpeed();

				_pong = PongGame(Game::font, _options.getSide(), 0, _options.getHumanColor(), _options.getAIColor(), _options.getDifficulty());
				_pong.init();
				break;
			case 1:
				//Escape has been hit so return to main menu
				_gameState = Game::InMenu;
				break;
			}
			break;
		}
		}


		if (Event.type == sf::Event::Closed)
			_gameState = Game::Exiting;

	}

	//outside of loop so that Keyboard state can immediately trigger a response
	if (_gameState == Game::Playing)
	{
		//returns elapsed time and resets the clock
		float delta = _clock.restart().asSeconds() * gameTimeFactor;

		if (_pong.processInputs(delta))
			_gameState = Game::InMenu;
		else
			_pong.update(delta, 11);
		
	}
		
}

void Game::updateView()
{
	//_window.clear();
	
	switch (_gameState)
	{
	case Game::InMenu:
		_mainMenu.draw(_window);
		break;

	case Game::Options:
		_options.draw(_window);
		break;

	case Game::Playing:
		_pong.draw(_window);

		break;
	}
	_window.display();
}


//initalizes the variables at runtime (since they are static)
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_window;
sf::Font Game::font;
Menu Game::_mainMenu;
OptionSelect Game::_options;
PongGame Game::_pong;
sf::Clock Game::_clock; // starts the clock
float Game::gameTimeFactor = 2.f;

