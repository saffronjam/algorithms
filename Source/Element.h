#pragma once

#include <SFML/Graphics/Color.hpp>

struct Element
{
	Element() = default;

	explicit Element(long value, sf::Color color = sf::Color::White) :
		Value(value),
		Color(color)
	{
	}

	long Value = 0;
	sf::Color Color = sf::Color::White;
};
