#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <IResourceMgr.h>

class SoundBufferMgr : public IResourceMgr<sf::SoundBuffer>
{
public:
    SoundBufferMgr() = default;
    SoundBufferMgr(const SoundBufferMgr &) = delete;
    const SoundBufferMgr &operator()(const SoundBufferMgr &) = delete;

    virtual void Load(const std::string &filepath) override
    {
        sf::SoundBuffer resource;
        if (!resource.loadFromFile(filepath))
        {
            THROW(Exception, "Failed to load soundbuffer: %s", filepath.c_str());
        }
        m_resources.emplace(std::make_pair(filepath, resource));
    }
};