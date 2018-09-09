//Ian Wright 9/9/18
#include "../include/Game.h"
#include "../include/Menu.h"

void Game::gameLoop()
{
	sf::Event Event;
	while(_window.pollEvent(Event))
	{
  
		switch(_gameState)
		{
		case Game::InMenu:
		{
			seeMenu();
			break;
		}
	
		case Game::Playing:
		{
			_window.clear(sf::Color::Blue);
			break;
		}
		}

		_window.display();

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
	_window.create(sf::VideoMode(800, 600, 32), "Pong");
	_gameState = Game::InMenu;

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

void Game::seeMenu()
{
	Menu mainMenu;
	Menu::MenuTask task = mainMenu.draw(_window);
}

//initalizes the variables at runtime (since they are static)
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_window;

