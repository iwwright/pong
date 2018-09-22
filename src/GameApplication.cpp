//Ian Wright 9/9/18
//GameApplication.cpp: defines static Game class for use as Game Application Layer
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

	//create main window and view for resizing, uses default viewport
	_window.create(sf::VideoMode(800, 600, 32), "Pong");//, sf::Style::Close);
	_view.reset(sf::FloatRect(0, 0, 800, 600));
	_window.setView(_view);

	//assigns value for _gameState and initializes _mainMenu
	_gameState = Game::InMenu;
	_mainMenu.init(Game::font);

	//initalize options screen
	_options.init(Game::font);

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
			case 0:
				//classic mode has been selected so start game with certain parameters
				_gameState = Game::InGame;
				gameTimeFactor = 2.f;

				_pong = PongGame(Game::font, -1, 0, sf::Color::Blue, sf::Color::Red, 1.0f);
				_pong.init();
				break;
			case 1:
				//special mode is selected so change state to display option select
				_gameState = Game::Options;
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
			case 0:
				//Enter has been hit so gather parameters and start game
				_gameState = Game::InGame;
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
		//pauses game if window is minimized or resized while in play
		if ((Event.type == sf::Event::LostFocus || Event.type == sf::Event::Resized) && _gameState == Game::InGame)
			_pong.pause();

		if (Event.type == sf::Event::Resized)
			preserveAspectRatio(Event.size.width, Event.size.height);


	}

	//outside of loop so that Keyboard state can immediately trigger a response
	if (_gameState == Game::InGame)
	{
		//returns elapsed time and resets the clock
		float delta = _clock.restart().asSeconds() * gameTimeFactor;
		int result = _pong.processInputs(delta);

		if (_pong.processInputs(delta))
			_gameState = Game::InMenu;
		else
			_pong.update(delta, 3);
		
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

	case Game::InGame:
		_pong.draw(_window);

		break;
	}
	_window.display();
}

void Game::preserveAspectRatio(int width, int height)
{
	sf::FloatRect viewport;
	viewport.width = 1.f;
	viewport.height = 1.f;

	//adjusts viewport so that 800x600 window is always centered and scaled properly
	if (width > height * aspectRatio)
	{
		viewport.width = (height * aspectRatio) / width;
		viewport.left = (1.f - viewport.width) / 2.f;

	}
	else if (width < height * aspectRatio)
	{
		viewport.height = width / (height * aspectRatio);
		viewport.top = (1.f - viewport.height) / 2.f;
	}
	
	_view.setViewport(viewport);
	_window.setView(_view);
}


//initalizes the variables at runtime (since they are static)
sf::Font Game::font;
float Game::gameTimeFactor = 2.f;

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_window;
sf::View Game::_view;
sf::Clock Game::_clock; // starts the clock
Menu Game::_mainMenu;
OptionSelect Game::_options;
PongGame Game::_pong;



