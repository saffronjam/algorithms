#include "Window.h"

sf::RenderWindow *Window::m_sfWindow = nullptr;
std::string Window::m_title("Unnamed window");
sf::VideoMode Window::m_videomode;
sf::Uint32 Window::m_style;
sf::Vector2i Window::m_nonFullscreenPosition;
sf::Transform Window::m_ndcTransform;
sf::Transform Window::m_defaultNdcTransform;
bool Window::m_fullscreen;
float Window::m_fov = 1.0f;

Window::Window(const std::string &title, int width, int height)
{
    m_videomode = sf::VideoMode(width, height);
    m_style = sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close;

    m_sfWindow = new sf::RenderWindow(m_videomode, title, m_style, sf::ContextSettings());
    m_sfWindow->setKeyRepeatEnabled(false);
    m_sfWindow->resetGLStates();
    SetTitle(title);
    PositionCenter();
    m_defaultNdcTransform = sf::Transform::Identity;
    m_defaultNdcTransform.scale(Lib::ConvertTo<float>(GetSize()) / 2.0f);
    m_defaultNdcTransform.translate(1.0f, 1.0f);
    m_defaultNdcTransform.scale(sf::Vector2f(1.0f, -1.0f));

    SetFoV(static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()));
}

Window::~Window()
{
    delete m_sfWindow;
    m_sfWindow = nullptr;
}

void Window::Draw(const sf::Drawable &drawable, sf::RenderStates renderStates)
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    renderStates.transform.combine(m_ndcTransform);
    Render(drawable, renderStates);
}

void Window::Clear()
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->clear();
}

void Window::Present() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->display();
}

void Window::PositionCenter() noexcept
{
    sf::VideoMode max = sf::VideoMode::getDesktopMode();
    sf::Vector2i halfSize = sf::Vector2i(m_sfWindow->getSize().x / 2u, m_sfWindow->getSize().y / 2u);
    m_sfWindow->setPosition(sf::Vector2i(max.width, max.height) / 2 - halfSize);
}

sf::RenderWindow *Window::GetSFWindow() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_sfWindow;
}

sf::Vector2i Window::GetPosition() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_sfWindow->getPosition();
}

sf::Vector2u Window::GetSize() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_sfWindow->getSize();
}

int Window::GetWidth() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return GetSize().x;
}

int Window::GetHeight() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return GetSize().y;
}

const std::string &Window::GetTitle() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_title;
}

sf::View Window::GetCurrentView() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_sfWindow->getView();
}

sf::View Window::GetDefaultView() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_sfWindow->getDefaultView();
}

sf::IntRect Window::GetScreenRect() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return sf::IntRect(0, 0, GetWidth(), GetHeight());
}

sf::FloatRect Window::GetNdcRect() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return sf::FloatRect(-1.0f, -1.0f, 2.0f, 2.0f);
}

void Window::SetPosition(const sf::Vector2i &pos) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->setPosition(pos);
}

void Window::SetSize(const sf::Vector2u &size) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->setSize(size);
}

void Window::SetTitle(const std::string &title) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->setTitle(title);
}

void Window::SetIcon(const std::string &icon) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    sf::Image image;
    image.loadFromFile(icon);
    m_sfWindow->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

void Window::SetFullscreen(bool toggle) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    if (toggle && !m_fullscreen)
    {
        m_fullscreen = true;
        m_videomode.width = GetSize().x;
        m_videomode.height = GetSize().y;
        m_nonFullscreenPosition = GetPosition();
        m_sfWindow->create(sf::VideoMode::getFullscreenModes()[0], GetTitle(), sf::Style::Fullscreen);
    }
    else if (!toggle && m_fullscreen)
    {
        m_fullscreen = false;
        m_sfWindow->create(m_videomode, GetTitle(), m_style);
        SetPosition(m_nonFullscreenPosition);
    }
}

void Window::SetVSync(bool toggle) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->setVerticalSyncEnabled(toggle);
}

void Window::SetFoV(float fov) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_fov = fov;
    ResetNdcTransform();
    m_ndcTransform.scale(1.0f / m_fov, 1.0f);
}

sf::Transform Window::GetNdcTransform() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_ndcTransform;
}

sf::Vector2f Window::RawToNdc(const sf::Vector2f &point) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_ndcTransform.getInverse().transformPoint(point);
}

sf::Vector2f Window::NdcToRaw(const sf::Vector2f &point) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    return m_ndcTransform.transformPoint(point);
}

void Window::Render(const sf::Drawable &drawable, sf::RenderStates renderStates)
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_sfWindow->draw(drawable, renderStates);
}

void Window::ResetNdcTransform() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sfWindow);
    m_ndcTransform = m_defaultNdcTransform;
}

Window::Exception::Exception(int line, const char *file, const char *errorString) noexcept
    : IException(line, file),
      errorString(errorString)
{
}

const char *Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << "[Type] " << GetType() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept
{
    return "V-2DEngine Window Exception";
}

const char *Window::Exception::GetErrorString() const noexcept
{
    return errorString.c_str();
}