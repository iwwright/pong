//Ian Wright 9/9/18
#include "../include/Game.h"

void Game::gameLoop()
{
	sf::Event Event;
	while (_window.pollEvent(Event))
	{
		switch (_gameState)
		{
		case Game::InMenu:
		{
			if (Event.key.code == sf::Keyboard::Up && Event.type == sf::Event::KeyReleased)
				_mainMenu.moveUp();

			if (Event.key.code == sf::Keyboard::Down && Event.type == sf::Event::KeyReleased)
				_mainMenu.moveDown();

			if (Event.key.code == sf::Keyboard::Enter && Event.type == sf::Event::KeyReleased)
			{
				switch (_mainMenu.selectionIndex)
				{
				case 0:
				case 1:
					_gameState = Game::Playing;
					break;
				case 2:
					_gameState = Game::Exiting;
					break;
				}
			}

			_window.clear();
			_mainMenu.draw(_window);
			_window.display();
			break;
		}
	
		case Game::Playing:
		{
			_window.clear(sf::Color::Blue);
			_window.display();
			break;
		}
		}


		if (Event.type == sf::Event::Closed)
			_gameState = Game::Exiting;

	}
}

void Game::start(void)
{

	//continue only if the game has not been initialized
	if (_gameState != Uninitialized)
		return;

	//create main window
	_window.create(sf::VideoMode(800, 600, 32), "Pong", sf::Style::Close);
	_gameState = Game::InMenu;
	_mainMenu.initialSetup();

	//call game loop until the state is exiting, then close
	while (!isExiting())
	{
		gameLoop();
	}

	_window.close();
}

bool Game::isExiting()
{
	//determine if the gameState is exiting
	return (_gameState == Game::Exiting);
}



//initalizes the variables at runtime (since they are static)
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_window;
Menu Game::_mainMenu;
