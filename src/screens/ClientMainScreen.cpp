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

    // -------------- ALL LABELS ------------------
    auto visStyleLabel = sfg::Label::Create("--- Visualisation Style ---");
    auto visAlgLabel = sfg::Label::Create("--- Visible Algorithms ---");
    auto settingsLabel = sfg::Label::Create("--- Settings ---");

    // -------------- NUMBER OF BARS ------------------
    auto elementsLabel = sfg::Label::Create();
    elementsLabel->SetText("Elements: 500");
    auto elementsScale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);

    auto elementsAdjustment = elementsScale->GetAdjustment();

    elementsAdjustment->SetLower(4.64159f);
    elementsAdjustment->SetUpper(464.159f);
    elementsAdjustment->SetValue(100.0f);

    elementsAdjustment->SetMinorStep(2.0f);
    elementsAdjustment->SetMajorStep(100.0f);

    elementsAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([elementsAdjustment, elementsLabel, this] {
        auto valMul = std::pow(elementsAdjustment->GetValue(), 1.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Elements: " << valMul;
        elementsLabel->SetText(oss.str());
        m_algorithmMgr.PopPushUntil(valMul);
    });
    {
        auto valMul = std::pow(elementsAdjustment->GetValue(), 1.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Elements: " << valMul;
        elementsLabel->SetText(oss.str());
        m_algorithmMgr.PopPushUntil(valMul);
    }

    elementsScale->SetRequisition(sf::Vector2f(80.f, 20.f));

    auto elementsScalebox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    elementsScalebox->Pack(elementsLabel, false, false);
    elementsScalebox->Pack(elementsScale, false, false);

    // -------------- SLEEP DELAY ------------------
    auto sleepDelayLabel = sfg::Label::Create();
    auto sleepDelayScale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);

    auto sleepDelayAdjustment = sleepDelayScale->GetAdjustment();

    sleepDelayAdjustment->SetLower(0.0f);
    sleepDelayAdjustment->SetUpper(26.826957952797f);
    sleepDelayAdjustment->SetValue(13.894954943731f);

    sleepDelayAdjustment->SetMinorStep(0.1f);
    sleepDelayAdjustment->SetMajorStep(0.3f);

    sleepDelayAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([sleepDelayAdjustment, sleepDelayLabel, this] {
        auto valMul = std::pow(sleepDelayAdjustment->GetValue(), 3.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Sleep delay " << valMul << " us";
        sleepDelayLabel->SetText(oss.str());
        m_algorithmMgr.SetSleepDelay(sf::microseconds(valMul));
    });
    {
        auto valMul = std::pow(sleepDelayAdjustment->GetValue(), 3.5f);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << "Sleep delay " << valMul << " us";
        sleepDelayLabel->SetText(oss.str());
        m_algorithmMgr.SetSleepDelay(sf::microseconds(valMul));
    }

    sleepDelayScale->SetRequisition(sf::Vector2f(80.f, 20.f));

    auto sleepDelayScalebox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    sleepDelayScalebox->Pack(sleepDelayLabel, false, false);
    sleepDelayScalebox->Pack(sleepDelayScale, false, false);
    sleepDelayScalebox->SetRequisition(sf::Vector2f(150.0f, 0.0f));

    auto sleepDelayCheck = sfg::CheckButton::Create("");
    sleepDelayCheck->SetActive(true);
    sleepDelayCheck->GetSignal(sfg::ToggleButton::OnToggle).Connect([sleepDelayCheck, sleepDelayLabel, sleepDelayAdjustment, sleepDelayScale, this] {
        if (sleepDelayCheck->IsActive())
        {
            sleepDelayScale->SetState(sfg::Widget::State::NORMAL);
            auto valMul = std::pow(sleepDelayAdjustment->GetValue(), 1.5f);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(0) << "Sleep delay " << valMul << " us";
            sleepDelayLabel->SetText(oss.str());
            m_algorithmMgr.SetSleepDelay(sf::microseconds(valMul));
        }
        else
        {
            sleepDelayScale->SetState(sfg::Widget::State::INSENSITIVE);
            sleepDelayLabel->SetText("Sleep delay deactivated");
            m_algorithmMgr.SetSleepDelay(sf::microseconds(0));
        }
    });

    auto sleepDelaySettingsBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.0f);
    sleepDelaySettingsBox->Pack(sleepDelayCheck, false);
    sleepDelaySettingsBox->Pack(sleepDelayScalebox, false);

    // -------------- PACK SCALES INTO ONE BOX ------------------
    auto scaleBoxes = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
    scaleBoxes->Pack(elementsScalebox);
    scaleBoxes->Pack(sleepDelaySettingsBox);

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

    // -------------- COMBO BOX AND SPECTRUM CHECK --------------------
    auto visStyleComboBox = sfg::ComboBox::Create();
    visStyleComboBox->AppendItem("Bars");
    visStyleComboBox->AppendItem("Circles");
    visStyleComboBox->AppendItem("Hoops");
    visStyleComboBox->SelectItem(0);

    visStyleComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect([visStyleComboBox, this] {
        switch (visStyleComboBox->GetSelectedItem())
        {
        case 0:
            m_algorithmMgr.SetVisType(IAlgorithm::VisType::Bars);
            break;
        case 1:
            m_algorithmMgr.SetVisType(IAlgorithm::VisType::Circles);
            break;
        case 2:
            m_algorithmMgr.SetVisType(IAlgorithm::VisType::Hoops);
            break;
        default:
            break;
        }
    });

    auto visStyleSpectrumCheck = sfg::CheckButton::Create("Spectrum");
    visStyleSpectrumCheck->SetActive(false);
    visStyleSpectrumCheck->GetSignal(sfg::ToggleButton::OnToggle).Connect([visStyleSpectrumCheck, this] {
        visStyleSpectrumCheck->IsActive() ? m_algorithmMgr.ActivateSpectrum() : m_algorithmMgr.DeactivateSpectrum();
    });

    auto visStyleSettingsBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 4.0f);
    visStyleSettingsBox->Pack(visStyleComboBox);
    visStyleSettingsBox->Pack(visStyleSpectrumCheck);

    // -------------- ALL BUTTONS ------------------
    auto startButton = sfg::Button::Create("Start");
    auto restartButton = sfg::Button::Create("Restart");
    auto resetButton = sfg::Button::Create("Reset");
    auto pauseButton = sfg::Button::Create("Pause");
    auto resumeButton = sfg::Button::Create("Resume");
    auto shuffleButton = sfg::Button::Create("Shuffle");

    startButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, elementsScale, this] {
        m_algorithmMgr.Start();
        elementsScale->SetState(sfg::Widget::State::INSENSITIVE);
    });

    restartButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, elementsScale, this] {
        m_algorithmMgr.Restart();
        elementsScale->SetState(sfg::Widget::State::NORMAL);
    });

    resetButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, elementsScale, this] {
        m_algorithmMgr.Reset();
        elementsScale->SetState(sfg::Widget::State::NORMAL);
    });

    pauseButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { m_algorithmMgr.Pause(); });

    resumeButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { m_algorithmMgr.Resume(); });

    shuffleButton->GetSignal(sfg::Widget::OnLeftClick).Connect([checkButtonsBox, elementsScale, this] {
        m_algorithmMgr.Shuffle();
        elementsScale->SetState(sfg::Widget::State::NORMAL);
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

    // --------------- SUB BOXES ----------------------
    auto visStyleBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 4.0f);
    visStyleBox->Pack(visStyleLabel);
    visStyleBox->Pack(visStyleSettingsBox);

    auto visAlgBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 4.0f);
    visAlgBox->Pack(visAlgLabel);
    visAlgBox->Pack(checkButtonsBox);

    auto settingsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 4.0f);
    settingsBox->Pack(settingsLabel);
    settingsBox->Pack(scaleBoxes);

    // -------------- ADD TO MAIN BOX ------------------
    auto mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 15.0f);
    mainBox->Pack(allButtonBox, false);
    mainBox->Pack(visStyleBox, false);
    mainBox->Pack(visAlgBox, false);
    mainBox->Pack(settingsBox, false);

    // -------------- ADD TO MAIN WINDOW ------------------
    auto window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
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