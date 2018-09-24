//Ian Wright 9/22/18
//PlayerView.cpp: Processes user input and draws all visuals to the screen

#include "PlayerView.h"

/*Initializes PlayerView object by preparing  all shapes, text, and sounds*/
void PlayerView::init(PongGame* gameLogic)
{
	//load font
	if (!font.loadFromFile("../src/assets/AtariClassic-Regular.ttf"))
		throw;

	logic = gameLogic;
	menu.init(font);
	options.init(font);

	_initShapes();
	_initText();
	_initSounds();
}

/*Draws all necessary shapes/text to the window that is passed in*/
void PlayerView::draw(sf::RenderWindow &window)
{
	//always a black background
	window.clear(sf::Color::Black);
	
	switch (logic->state)
	{
	case PongGame::InMenu:
		menu.draw(window);
		break;
	case PongGame::Options:
		options.draw(window);
		break;
	case PongGame::Exiting:
		break;
	default:
		//InGame || GameOver || Paused
		window.draw(logic->objects.humanPlayer.scoreText);
		window.draw(logic->objects.aiPlayer.scoreText);

		if (logic->state == PongGame::GameOver)
		{
			window.draw(_endGameHelp);
			window.draw(_endGameMessage);
		}
		else
		{
			window.draw(logic->objects.humanPlayer.paddle);
			window.draw(logic->objects.aiPlayer.paddle);

			for (int i = 0; i < numBackgroundShapes; i++)
				window.draw(_backgroundShapes[i]);

			logic->objects.ball.draw(window);

			if (!logic->objects.ball.isInPlay)
				window.draw(_notInPlayText);

			if (logic->state == PongGame::Paused)
			{
				window.draw(_pausedText);
				window.draw(_pausedHelp);
			}
		}
		break;
	}
	window.display();
}

/*Both the Menu and Option Select require enter to be released so this method is called in Game::_processEvents()
since sf::Events are unions. Handles input for Both Menu and OptionSelect*/
void PlayerView::processEvents(sf::Event curEvent)
{
	//get current game state
	switch (logic->state)
	{

	case PongGame::InMenu:
		switch (menu.processInput(curEvent))
		{
		case 0:
			//classic mode has been selected so start game with certain parameters
			_gameTimeFactor = 2.f;
			logic->setState(PongGame::InGame);
			logic->updateParameters(_gameTimeFactor, -1, sf::Color::Blue, sf::Color::Red, 1.0f);
			break;
		case 1:
			//custom mode is selected so change state to display option select
			logic->setState(PongGame::Options);
			break;
		case 2:
			//exit has been selected so quit the game
			logic->setState(PongGame::Exiting);
			break;
		}
		break;

	case PongGame::Options:
		switch (options.processInput(curEvent))
		{
		case 0:
			//Enter has been hit so gather parameters and start game
			_gameTimeFactor = 2.f * options.getSpeed();
			logic->setState(PongGame::InGame);
			logic->updateParameters(_gameTimeFactor, options.getSide(), options.getHumanColor(), options.getAIColor(), options.getDifficulty());
			break;
		case 1:
			//Escape has been hit so return to main menu
			logic->setState(PongGame::InMenu);
			break;
		}
		break;
	}
	//update the text now that Player objects have been created
	if (logic->state == PongGame::InGame)
	{
		logic->objects.humanPlayer.updateText(font);
		logic->objects.aiPlayer.updateText(font);
	}
}

/*Called in Game::_gameLoop() so it can directly reads keyboard states and respond to user input in a smooth 
manner. Updates logic->state appropriately*/
void PlayerView::processKeyboard(float delta)
{
	//get current game state
	switch (logic->state)
	{
	
	//pausing stops any moving objects
	case PongGame::Paused:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!_spaceHeld)
				logic->setState(PongGame::InGame);
			_spaceHeld = true;
		}
		else
			_spaceHeld = false;

		//allow player to quit if game is paused
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			logic->setState(PongGame::InMenu);
		break;

	case PongGame::InGame:
		delta *= _gameTimeFactor;
		//set velocity to zero so that way if move is not called, it remains 0 when calculating ball angle
		logic->objects.humanPlayer.velocity = 0.f;

		//moves paddle up, keeps track of if the key is held for paddle acceleration
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			logic->objects.humanPlayer.movePaddle(-1, delta, _upHeld);
			_upHeld = true;
		}
		else
			_upHeld = false;

		//moves paddle down, keeps track of if the key is held for paddle acceleration
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			logic->objects.humanPlayer.movePaddle(1, delta, _downHeld);
			_downHeld = true;
		}
		else
			_downHeld = false;

		//Pauses game if ball is in play and space is not held, sets ball in play if it isn't
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (logic->objects.ball.isInPlay && !_spaceHeld)
			{
				_pauseSound.play();
				logic->setState(PongGame::Paused);
			}
			else
				logic->objects.ball.isInPlay = true;

			_spaceHeld = true;
		}
		else
			_spaceHeld = false;

		break;

	case PongGame::GameOver:
		//restart the game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			logic->reset();
			logic->objects.humanPlayer.updateText(font);
			logic->objects.aiPlayer.updateText(font);
		}

		//return true to signal that gameState needs to be changed to inMenu
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			logic->setState(PongGame::InMenu);
		break;
	}
}

/*Purpose is to play the score sound and update the text for each player*/
void PlayerView::score()
{
	_scoreSound.play();
	logic->objects.humanPlayer.updateText(font);
	logic->objects.aiPlayer.updateText(font);
	//prepare gameover screen before drawing it
	if (logic->state == PongGame::GameOver)
		_makeEndGameScreen();
}


void PlayerView::_initShapes()
{
	//creates passive shapes (don't affect ball) 
	for (int i = 0; i < 20; i++)
	{
		//dotted lines to divide court in half
		sf::RectangleShape center;
		center.setFillColor(sf::Color(185, 185, 185));
		center.setSize(sf::Vector2f(10, 15));
		center.setPosition(400.f - (center.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		_backgroundShapes.push_back(center);
		//dotted lines to show left and right edges of court (useful when window size is changed)
		sf::RectangleShape left;
		left.setFillColor(sf::Color(60, 60, 60));
		left.setSize(sf::Vector2f(5, 15));
		left.setPosition(0.f - (left.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		_backgroundShapes.push_back(left);
		sf::RectangleShape right;
		right.setFillColor(sf::Color(60, 60, 60));
		right.setSize(sf::Vector2f(5, 15));
		right.setPosition(800.f - (right.getLocalBounds().width / 2.0f), 30 * i + 7.5f);
		_backgroundShapes.push_back(right);
	}
	//creates edges of court
	for (int i = 0; i < 2; i++)
	{
		//makes top and bottom borders for court
		sf::RectangleShape tmp;
		tmp.setFillColor(sf::Color(185, 185, 185));
		tmp.setSize(sf::Vector2f(800, 15));
		tmp.setPosition(400.f - (tmp.getLocalBounds().width / 2.0f), (600 - 15.f)*i);
		_backgroundShapes.push_back(tmp);
	}
}

void PlayerView::_initText()
{
	//text to let player know how to make the ball start moving before each point
	_notInPlayText.setFont(font);
	_notInPlayText.setString("Press Space");
	_notInPlayText.setCharacterSize(45);
	_notInPlayText.setFillColor(sf::Color::White);
	_notInPlayText.setPosition(390.f - (_notInPlayText.getLocalBounds().width / 2.f), 150);

	//text to let player know the game is paused
	_pausedText.setFont(font);
	_pausedText.setString("GAME PAUSED");
	_pausedText.setCharacterSize(45);
	_pausedText.setFillColor(sf::Color::Cyan);
	_pausedText.setPosition(390.f - (_pausedText.getLocalBounds().width / 2.f), 150);
	//possible actions while paused
	_pausedHelp.setFont(font);
	_pausedHelp.setString("[SPACE]-Resume\n[ESC]-Quit");
	_pausedHelp.setCharacterSize(30);
	_pausedHelp.setFillColor(sf::Color::Cyan);
	_pausedHelp.setPosition(390.f - (_pausedHelp.getLocalBounds().width / 2.f), 210);
}

void PlayerView::_initSounds()
{
	//prepares pause and scoring sounds 
	if (!_pauseBuffer.loadFromFile("../src/assets/pause.wav"))
		throw;
	if (!_scoreBuffer.loadFromFile("../src/assets/ding.wav"))
		throw;
	_pauseSound.setBuffer(_pauseBuffer);
	_scoreSound.setBuffer(_scoreBuffer);
	_scoreSound.setVolume(60.f);

	//waits to load buffer for end game sound until the winner is determined
	_endSound.setVolume(60.f);
}

/*Performs necessary prepartions for the GameOver screen*/
void PlayerView::_makeEndGameScreen()
{
	//determine winner and set endGameMessage string/color/sound appropriately
	if (logic->objects.humanPlayer.score > logic->objects.aiPlayer.score)
	{
		if (!_endBuffer.loadFromFile("../src/assets/wowIncredible.wav"))
			throw;
		_endGameMessage.setString("YOU WIN!");
		_endGameMessage.setFillColor(sf::Color::Green);
	}
	else
	{
		if (!_endBuffer.loadFromFile("../src/assets/failure.wav"))
			throw;
		_endGameMessage.setString("YOU LOSE!");
		_endGameMessage.setFillColor(sf::Color::Red);
	}
	_endGameMessage.setFont(font);
	_endGameMessage.setCharacterSize(80);
	_endGameMessage.setPosition(400.f - (_endGameMessage.getLocalBounds().width / 1.92f), 180);

	//instructions on how to reset the game
	_endGameHelp.setFont(font);
	_endGameHelp.setCharacterSize(30);
	_endGameHelp.setString("[ENTER]  - Try Again\n[ESCAPE] - Main Menu");
	_endGameHelp.setFillColor(sf::Color::White);
	_endGameHelp.setPosition(400.f - (_endGameHelp.getLocalBounds().width / 1.9f), 315);

	_endSound.setBuffer(_endBuffer);
	_endSound.play();
}