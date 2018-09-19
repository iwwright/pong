//Ian Wright 9/9/18
#pragma once	//better way of writing include guards
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Menu.h"
#include "OptionSelect.h"
#include "PongGame.h"
#include "Player.h"

//header file defines static class to act as global interface for the game
//only one instance of this class
class Game
{
public:
	static void init();
	static sf::Font font;
	static float gameTimeFactor;

private:
	static bool isExiting();
	static void gameLoop();
	static void processInput();
	static void updateView();

	//defines states of the game 
	enum GameState
	{
		Uninitialized, InMenu, Options, Paused, Playing, Finished, Exiting
	};

	static GameState _gameState;
	static sf::RenderWindow _window;

	static Menu _mainMenu;
	static OptionSelect _options;

	static PongGame _pong;
	static sf::Clock _clock;
};