#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#define GET_VARIABLE_NAME(Variable) (#Variable)

class Lib
{
public:
    template <typename T, typename U>
    static sf::Vector2<T> ConvertTo(const sf::Vector2<U> &vec) noexcept
    {
        return sf::Vector2<T>((T)vec.x, (T)vec.y);
    }
    template <typename T, typename U>
    static sf::Rect<T> ConvertTo(const sf::Rect<U> &rect) noexcept
    {
        return sf::Rect<T>((T)rect.left, (T)rect.top, (T)rect.width, (T)rect.height);
    }
    template <typename T>
    static sf::Vector2<T> Mid(sf::Rect<T> rect) noexcept
    {
        return sf::Vector2<T>(rect.left + rect.width / (T)2, rect.top + rect.height / (T)2);
    }

    template <typename T>
    static sf::Vector2<T> MapPoint(const sf::Vector2<T> &point, sf::Rect<T> from, sf::Rect<T> to) noexcept
    {
        sf::Vector2<T> _point = point;
        float x_diff = _point.x - from.left;
        float y_diff = _point.y - from.top;

        float x_percent_diff = x_diff / from.width;
        float y_percent_diff = y_diff / from.height;

        _point.x = to.left + to.width * x_percent_diff;
        _point.y = to.top + to.height * y_percent_diff;

        return _point;
    }

    template <typename T>
    static T Constrain(const T &value, T from, T to)
    {
        if (value < from)
            return from;
        else if (value > to)
            return to;
        else
            return value;
    }
};