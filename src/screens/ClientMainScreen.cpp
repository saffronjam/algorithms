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

    // -------------- NUMBER OF BARS ------------------
    auto barsLabel = sfg::Label::Create();
    barsLabel->SetText("Bars: 500");
    auto barsScale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);

    auto barsAdjustment = barsScale->GetAdjustment();

    barsAdjustment->SetLower(4.64159f);
    barsAdjustment->SetUpper(464.159f);
    barsAdjustment->SetValue(100.0f);

    barsAdjustment->SetMinorStep(2.0f);
    barsAdjustment->SetMajorStep(100.0f);

    barsAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([barsAdjustment, barsLabel, this] {
        auto valMul = std::pow(barsAdjustment->GetValue(), 1.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Bars: " << valMul;
        barsLabel->SetText(oss.str());
        m_algorithmMgr.PopPushUntil(valMul);
    });
    {
        auto valMul = std::pow(barsAdjustment->GetValue(), 1.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Bars: " << valMul;
        barsLabel->SetText(oss.str());
        m_algorithmMgr.PopPushUntil(valMul);
    }

    barsScale->SetRequisition(sf::Vector2f(80.f, 20.f));

    auto barsScalebox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    barsScalebox->Pack(barsLabel, false, false);
    barsScalebox->Pack(barsScale, false, false);

    // -------------- SLEEP DELAY ------------------
    auto sleepDelayLabel = sfg::Label::Create();
    auto sleepDelayScale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);

    auto sleepDelayAdjustment = sleepDelayScale->GetAdjustment();

    sleepDelayAdjustment->SetLower(1.0f);
    sleepDelayAdjustment->SetUpper(1357.21f);
    sleepDelayAdjustment->SetValue(464.159f);

    sleepDelayAdjustment->SetMinorStep(1.0f);
    sleepDelayAdjustment->SetMajorStep(5.0f);

    sleepDelayAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([sleepDelayAdjustment, sleepDelayLabel, this] {
        auto valMul = std::pow(sleepDelayAdjustment->GetValue(), 1.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Sleep delay " << valMul << " us";
        sleepDelayLabel->SetText(oss.str());
        m_algorithmMgr.SetSleepDelay(sf::microseconds(valMul));
    });
    {
        auto valMul = std::pow(sleepDelayAdjustment->GetValue(), 1.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Sleep delay " << valMul << " us";
        sleepDelayLabel->SetText(oss.str());
        m_algorithmMgr.SetSleepDelay(sf::microseconds(valMul));
    }

    sleepDelayScale->SetRequisition(sf::Vector2f(80.f, 20.f));

    auto sleepDelayScalebox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    sleepDelayScalebox->Pack(sleepDelayLabel, false, false);
    sleepDelayScalebox->Pack(sleepDelayScale, false, false);

    // -------------- PACK ALL SCALES INTO ONE BOX ------------------
    auto scaleBoxes = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
    scaleBoxes->Pack(barsScalebox);
    scaleBoxes->Pack(sleepDelayScalebox);

    // -------------- ALL CHECK BOXES ------------------
    std::vector<sfg::CheckButton::Ptr> checkButtons;
    for (auto &algorithm : m_algorithmMgr.GetAlgorithms())
        checkButtons.push_back(sfg::CheckButton::Create(algorithm->GetName()));

    auto checkButtonsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 3.0f);
    for (auto &checkButton : checkButtons)
    {
        checkButton->GetSignal(sfg::ToggleButton::OnToggle).Connect([checkButton, this] {
            checkButton->IsActive() ? m_algorithmMgr.Activate(checkButton->GetLabel()) : m_algorithmMgr.Deactivate(checkButton->GetLabel());
        });
        checkButton->SetActive(true);
        checkButtonsBox->Pack(checkButton);
    }

    auto tmp = std::async(std::launch::async, ClientMainScreen::Build, this);
    tmp.get();
    // -------------- ALL BUTTONS ------------------
    auto startButton = sfg::Button::Create("Start");
    auto restartButton = sfg::Button::Create("Restart");
    auto resetButton = sfg::Button::Create("Reset");
    auto pauseButton = sfg::Button::Create("Pause");
    auto resumeButton = sfg::Button::Create("Resume");
    auto shuffleButton = sfg::Button::Create("Shuffle");

    startButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, barsScale, this] {
        m_algorithmMgr.Start();
        for (auto &child : checkButtonsBox->GetChildren())
            child->SetState(sfg::Widget::State::INSENSITIVE);
        barsScale->SetState(sfg::Widget::State::INSENSITIVE);
    });

    restartButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, barsScale, this] {
        m_algorithmMgr.Restart();
        for (auto &child : checkButtonsBox->GetChildren())
            child->SetState(sfg::Widget::State::NORMAL);
        barsScale->SetState(sfg::Widget::State::NORMAL);
    });

    resetButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, barsScale, this] {
        m_algorithmMgr.Reset();
        for (auto &child : checkButtonsBox->GetChildren())
            child->SetState(sfg::Widget::State::NORMAL);
        barsScale->SetState(sfg::Widget::State::NORMAL);
    });

    pauseButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { m_algorithmMgr.Pause(); });

    resumeButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { m_algorithmMgr.Resume(); });

    shuffleButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, barsScale, this] {
        m_algorithmMgr.Shuffle();
        for (auto &child : checkButtonsBox->GetChildren())
            child->SetState(sfg::Widget::State::NORMAL);
        barsScale->SetState(sfg::Widget::State::NORMAL);
    });

    auto buttonsBoxRow0 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.0f);
    auto buttonsBoxRow1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.0f);
    auto buttonsBoxRow2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.0f);

    buttonsBoxRow0->Pack(startButton);
    buttonsBoxRow0->Pack(restartButton);
    buttonsBoxRow0->Pack(resetButton);
    buttonsBoxRow1->Pack(pauseButton);
    buttonsBoxRow1->Pack(resumeButton);
    buttonsBoxRow2->Pack(shuffleButton);

    auto allButtonBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
    allButtonBox->Pack(buttonsBoxRow0, false);
    allButtonBox->Pack(buttonsBoxRow1, false);
    allButtonBox->Pack(buttonsBoxRow2, false);

    // -------------- ADD TO MAIN BOX ------------------
    auto mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 15.0f);
    mainBox->Pack(allButtonBox, false);
    mainBox->Pack(checkButtonsBox, false);
    mainBox->Pack(scaleBoxes, false);

    // -------------- ADD TO MAIN WINDOW ------------------
    auto window = sfg::Window::Create(sfg::Window::Style::TITLEBAR | sfg::Window::Style::BACKGROUND);
    window->SetTitle("Settings");
    window->SetPosition(sf::Vector2f(Window::GetWidth() - 200.0f, 0.0f));
    window->SetRequisition(sf::Vector2f(200.0f, Window::GetHeight()));
    window->Add(mainBox);

    GuiMgr::Add(window);
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
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