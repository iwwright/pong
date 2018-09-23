//Ian Wright 9/22/18
#pragma once
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "PongGame.h"
#include "Menu.h"
#include "OptionSelect.h"
#include <vector>

#define numBackgroundShapes 62


class PlayerView
{
public:
	PlayerView() = default;
	~PlayerView();

	void init(PongGame* gameLogic);

	void processEvents(sf::Event curEvent);
	void processKeyboard(float delta);
	void draw(sf::RenderWindow &window);
	void score();

	PongGame* logic;

	Menu menu;
	OptionSelect options;

	sf::Font font;

private:
	void _makeEndGameScreen();
	void _initShapes();
	void _initText();
	void _initSounds();

	float _gameTimeFactor;

	//shapes that are not changed/moved, displayed when in game
	std::vector<sf::RectangleShape> _backgroundShapes;

	sf::Text _notInPlayText;

	bool _upHeld;
	bool _downHeld;
	bool _spaceHeld;

	//variables used for game over
	sf::Text _endGameMessage;
	sf::Text _endGameHelp;
	sf::SoundBuffer _endBuffer;
	sf::Sound _endSound;

	//variables used for pausing
	sf::Text _pausedText;
	sf::Text _pausedHelp;
	sf::SoundBuffer _pauseBuffer;
	sf::Sound _pauseSound;

	//scoring sounds
	sf::SoundBuffer _scoreBuffer;
	sf::Sound _scoreSound;

};