//Ian Wright 9/9/18
//GameApplication.cpp: defines Game class for use as Game Application Layer
#include "GameApplication.h"

void Game::_gameLoop()
{
	_clock.restart(); // starts the clock

	//call game loop until the state is exiting, then close
	while (_logic.state != PongGame::Exiting)
	{
		_processEvents();

		//returns elapsed time and resets the clock
		float delta = _clock.restart().asSeconds();
		
		_playerView.processKeyboard(delta);

		if (_logic.update(delta, scoreToWin))
			_playerView.score();

		_playerView.draw(_window);
	}
	

}

void Game::init(void)
{

	//create main window and view for resizing, uses default viewport
	_window.create(sf::VideoMode(800, 600, 32), "Pong");//, sf::Style::Close);
	_view.reset(sf::FloatRect(0, 0, 800, 600));
	_window.setView(_view);


	//creates logic and initializes PlayerView with a pointer to logic
	_logic = PongGame();
	_playerView = PlayerView();
	_playerView.init(&_logic);

	//calls the game loop after initialization
	 _gameLoop();

	_window.close();
}


void Game::_processEvents()
{
	sf::Event Event;

	while (_window.pollEvent(Event))
	{
		_playerView.processEvents(Event);

		//makes sure game can shutdown properly
		if (Event.type == sf::Event::Closed)
			_logic.setState(PongGame::Exiting);
		
		//pauses game if window is minimized or resized while ball is in play
		if ((Event.type == sf::Event::LostFocus || Event.type == sf::Event::Resized) && _logic.state == PongGame::InGame && _logic.objects.ball.isInPlay)
			_logic.setState(PongGame::Paused);

		if (Event.type == sf::Event::Resized)
			_preserveAspectRatio(Event.size.width, Event.size.height);
	}
}


void Game::_preserveAspectRatio(int width, int height)
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

