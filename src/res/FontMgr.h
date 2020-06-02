#pragma once

#include <SFML/Graphics/Font.hpp>

#include "IResourceMgr.h"

class FontMgr : public IResourceMgr<sf::Font>
{
public:
    FontMgr() = default;
    FontMgr(const FontMgr &) = delete;
    const FontMgr &operator()(const FontMgr &) = delete;

    virtual void Load(const std::string &filepath) override
    {
        sf::Font resource;
        if (!resource.loadFromFile(filepath))
        {
            THROW(Exception, "Failed to load font: %s", filepath.c_str());
        }
        m_resources.emplace(std::make_pair(filepath, resource));
    }
};