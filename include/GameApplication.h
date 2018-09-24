//Ian Wright 9/9/18
#pragma once

#include "PlayerView.h"

#define aspectRatio 1.25f
#define scoreToWin 11

class Game
{
public:
	Game() = default;
	~Game() = default;

	void init();

private:
	void _gameLoop();
	void _processEvents();

	void _preserveAspectRatio(int width, int height);

	sf::RenderWindow _window;
	sf::View _view;
	sf::Clock _clock;

	PongGame _logic;
	PlayerView _playerView;
};