#pragma once

#include <SFML/Graphics/Color.hpp>

struct Element
{
	Element() = default;

	Element(long value, sf::Color color) :
		Value(value),
		Color(color)
	{
	}

	long Value = 0;
	sf::Color Color = sf::Color::White;
};
