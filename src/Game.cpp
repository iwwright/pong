//Ian Wright 9/9/18
#include "../include/Game.h"
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

void Game::start(void)
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
	_mainMenu.initialSetup(font);

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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				_mainMenu.moveUp();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				_mainMenu.moveDown();

			if (Event.key.code == sf::Keyboard::Enter && Event.type == sf::Event::KeyPressed)
			{
				switch (_mainMenu.selectionIndex)
				{
				case 0:
				case 1:
					_gameState = Game::Playing;
					_pong = PongGame(0, font);
					break;
				case 2:
					_gameState = Game::Exiting;
					break;
				}
			}

			break;
		}

		}


		if (Event.type == sf::Event::Closed)
			_gameState = Game::Exiting;

	}

	if (_gameState == Game::Playing)
	{
		//returns elapsed time and resets the clock
		float delta = _clock.restart().asMilliseconds();
		std::cout << delta << "\n";

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			_pong.objects.humanPlayer.movePaddle(-1, delta);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			_pong.objects.humanPlayer.movePaddle(1, delta);

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
PongGame Game::_pong;
sf::Clock Game::_clock; // starts the clock
int Game::_gameTimeFactor;

