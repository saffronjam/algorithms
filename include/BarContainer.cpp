#include "BarContainer.hpp"

BarContainer::Bar::Bar(double const &value)
    : m_value(value)
{
}

BarContainer::BarContainer(Graphics &gfx, std::vector<std::shared_ptr<BarContainer::Bar>> &allBars, sf::IntRect const &visualizationBox)
    : m_gfx(gfx),
      m_allBars(allBars),
      m_visualizationBox(visualizationBox),
      m_barWidth(0.0f),
      m_maxBarHeight(0.0f),
      m_label("", *gfx.allFonts[Graphics::FontMapping::FontArial], 24)
{
    CorrectBarSizes();
    ComputeResetValues();
}

void BarContainer::Update()
{
    m_label.setPosition(sf::Vector2f(m_visualizationBox.left + m_visualizationBox.width / 2 - m_label.getLocalBounds().width / 2, m_visualizationBox.top - 32));
    for (size_t i = 0; i < m_allBars.size(); i++)
    {
        m_allBars[i]->SetPosition(sf::Vector2f(i * m_barWidth + m_visualizationBox.left, m_visualizationBox.top + m_visualizationBox.height - (float)m_allBars[i]->GetRectangleShape().getSize().y));
    }
}

void BarContainer::Draw()
{
    m_gfx.Draw(m_label);
    for (auto &bar : m_allBars)
    {
        m_gfx.Draw(bar->GetRectangleShape());
    }
}

void BarContainer::Reset()
{
    for (size_t i = 0; i < m_allBars.size(); i++)
    {
        m_allBars[i]->SetValue(m_allBarsResetValue[i]);
    }
    for (auto &bar : m_allBars)
    {
        bar->SetColor(sf::Color::White);
    }
    CorrectBarSizes();
}

void BarContainer::ComputeResetValues()
{
    m_allBarsResetValue.clear();
    for (auto &bar : m_allBars)
    {
        m_allBarsResetValue.push_back(bar->GetValue());
    }
}

void BarContainer::CorrectBarSizes()
{
    m_barWidth = (float)m_visualizationBox.width / (float)m_allBars.size();
    m_maxBarHeight = 0.0f;
    for (auto bar : m_allBars)
    {
        if (bar->GetValue() > m_maxBarHeight)
        {
            m_maxBarHeight = bar->GetValue();
        }
    }

    for (auto &bar : m_allBars)
    {
        float offset;
        if (m_barWidth <= 1.0f)
        {
            offset = 0.0f;
        }
        else
        {
            offset = 1.0f;
        }

        bar->SetRectangleShape(sf::RectangleShape(sf::Vector2f(m_barWidth - offset, (bar->GetValue() / m_maxBarHeight) * m_visualizationBox.height)));
    }
}