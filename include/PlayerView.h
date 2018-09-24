//Ian Wright 9/22/18
#pragma once

#include "Menu.h"
#include "PongGame.h"
#include "OptionSelect.h"

#define numBackgroundShapes 62

class PlayerView
{
public:
	PlayerView() = default;
	~PlayerView() = default;

	void init(PongGame* gameLogic);
	void draw(sf::RenderWindow &window);
	void processEvents(sf::Event curEvent);
	void processKeyboard(float delta);
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

	//speed at which the game runs
	float _gameTimeFactor;
	//determines whether key is being held for acceleration or to avoid repeated inputs 
	bool _upHeld;
	bool _downHeld;
	bool _spaceHeld;
	//shapes that are not changed/moved but are displayed when in game
	std::vector<sf::RectangleShape> _backgroundShapes;

	//Text telling the player how to put the ball in play
	sf::Text _notInPlayText;
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