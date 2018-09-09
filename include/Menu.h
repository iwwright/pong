//Ian Wright 9/9/18
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Menu
{
public:
	enum MenuTask { PlayNormal, PlaySpecial, Exit, None };
	MenuTask draw(sf::RenderWindow &window);

	struct MenuButton
	{
		//represents menu button shape and its corresponding action
		sf::Rect<int> shape;
		MenuTask action;
	};


};