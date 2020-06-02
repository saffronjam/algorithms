#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "IResourceMgr.h"
#include "GenericThrowMacros.h"

class TextureMgr : public IResourceMgr<sf::Texture>
{
public:
    TextureMgr() = default;
    TextureMgr(const TextureMgr &) = delete;
    const TextureMgr &operator()(const TextureMgr &) = delete;

    virtual void Load(const std::string &filepath) override
    {
        sf::Texture resource;
        if (!resource.loadFromFile(filepath))
        {
            THROW(Exception, "Failed to load texture: %s", filepath.c_str());
        }
        m_resources.emplace(std::make_pair(filepath, resource));
    }
};