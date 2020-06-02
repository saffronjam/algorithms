#pragma once

#include <memory>

#include <SFML/Audio/Music.hpp>

#include <IResourceMgr.h>

class MusicMgr : public IResourceMgr<std::shared_ptr<sf::Music>>
{
public:
    MusicMgr() = default;
    MusicMgr(const MusicMgr &) = delete;
    const MusicMgr &operator()(const MusicMgr &) = delete;

private:
    using IResourceMgr<std::shared_ptr<sf::Music>>::Get;

public:
    virtual void Load(const std::string &filepath) override
    {
        std::shared_ptr<sf::Music> resource = std::make_shared<sf::Music>();
        if (!resource->openFromFile(filepath))
        {
            THROW(Exception, "Failed to open music: %s", filepath.c_str());
        }
        m_resources.emplace(std::make_pair(filepath, resource));
    }
};