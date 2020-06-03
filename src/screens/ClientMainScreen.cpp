#include "ClientMainScreen.h"
#include "AppClient.h"

ClientMainScreen::ClientMainScreen(AppClient &parent)
    : m_parent(parent)
{
}

ClientMainScreen::~ClientMainScreen()
{
}

void ClientMainScreen::Build()
{
}

void ClientMainScreen::Destroy()
{
}

void ClientMainScreen::OnEntry()
{
    FPSLimiter::SetDesiredFPS(60.0f);
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    if (Keyboard::IsPressed(sf::Keyboard::Num1))
        m_algorithmMgr.Start();
    if (Keyboard::IsPressed(sf::Keyboard::Num2))
        m_algorithmMgr.Pause();
    if (Keyboard::IsPressed(sf::Keyboard::Num3))
        m_algorithmMgr.Resume();
    if (Keyboard::IsPressed(sf::Keyboard::Num4))
        m_algorithmMgr.Reset();
    if (Keyboard::IsPressed(sf::Keyboard::Num5))
        m_algorithmMgr.Shuffle();
}

void ClientMainScreen::Draw()
{
    m_algorithmMgr.Draw();
}

int ClientMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ClientMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}