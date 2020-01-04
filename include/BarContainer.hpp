#pragma once

#include "memory"

#include "Camera.hpp"

class BarContainer
{
public:
    class Bar
    {
    public:
        Bar(double const &value);

        void SetColor(sf::Color const &color) { m_rectangleShape.setFillColor(color); }
        void SetRectangleShape(sf::RectangleShape const &rectangleShape) { m_rectangleShape = rectangleShape; }
        void SetPosition(sf::Vector2f const &position) { m_rectangleShape.setPosition(position); }
        void SetValue(double const &value) { m_value = value; }

        sf::RectangleShape GetRectangleShape() { return m_rectangleShape; }
        double GetValue() { return m_value; }

    private:
        sf::RectangleShape m_rectangleShape;

        double m_value;
    };

public:
    BarContainer(Graphics &gfx, std::vector<std::shared_ptr<BarContainer::Bar>> &allBars, sf::IntRect const &visualizationBox = sf::IntRect(-Camera::GetOffset(), Camera::GetOffset() * 2));

    void Update();
    void Draw();

    auto *GetAllBarsPtr() { return &m_allBars; }

    void SetLabel(std::string const &label) { m_label.setString(label); }
    void SetVisualizationBox(sf::IntRect const &visualizationBox) { m_visualizationBox = visualizationBox; }

    void Reset();
    void ComputeResetValues();
    void CorrectBarSizes();

private:
    Graphics &m_gfx;

    std::vector<std::shared_ptr<BarContainer::Bar>> &m_allBars;
    std::vector<double> m_allBarsResetValue;
    sf::IntRect m_visualizationBox;

    float m_barWidth;
    float m_maxBarHeight;

    sf::Text m_label;
};