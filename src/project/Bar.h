#pragma once

#include <SFML/Graphics/Color.hpp>

struct Bar
{
    Bar() = default;
    Bar(long value, sf::Color color = sf::Color::White)
        : value(value), color(color) {}
    long value = 0;
    sf::Color color = sf::Color::White;
};