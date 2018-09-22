//Ian Wright 9/11/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "Player.h"
#include <vector>

#define numPassiveShapes 60
#define numActiveShapes 2

class PongGame
{
public:
	PongGame() = default;
	PongGame(sf::Font scoreFont, short side, short type, sf::Color humanColor, sf::Color aiColor, float difficulty=1);
	~PongGame();

	void init();

	void draw(sf::RenderWindow &window);
	void update(float delta, int scoreToWin);
	bool processInputs(float delta);

	void pause();
	sf::Font font;
	
	//pointers to the Player object on the corresponding side of the court, assigned in initVars() since Game::_pong is static
	Player *left;
	Player *right;
	
	struct PongObjects
	{
		Ball ball;

		//shapes to be ignored by the ball
		std::vector<sf::RectangleShape> passiveShapes;

		//shapes the ball bounces off of
		std::vector<sf::RectangleShape> activeShapes;

		Player humanPlayer;
		Player aiPlayer;
	} objects;

private:
	sf::Text _notInPlayText;

	bool _upHeld;
	bool _downHeld;

	//variables used for game over
	bool _gameOver = false;
	sf::Text _endGameMessage;
	sf::Text _endGameHelp;
	sf::SoundBuffer _endBuffer;
	sf::Sound _endSound;

	//variables used for pausing
	bool _paused = false;
	bool _spaceHeld;
	sf::Text _pausedText;
	sf::Text _pausedHelp;
	sf::SoundBuffer _pauseBuffer;
	sf::Sound _pauseSound;

	//scoring sounds
	sf::SoundBuffer _scoreBuffer;
	sf::Sound _scoreSound;

	void _makeEndGameScreen();
	void _reset();
	void _updateAI(float delta);

	//AI can move paddle when the distance between its paddle and the ball is less than the below value
	float _aiResponseDistance;
};