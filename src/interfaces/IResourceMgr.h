#pragma once

#include <map>

#include "IException.h"

template <class T>
class IResourceMgr
{
public:
    IResourceMgr() = default;
    // Returns pointer resource from cache, if not existing, call Load();
    virtual T *Get(const std::string &filepath) noexcept
    {
        if (m_resources.find(filepath) == m_resources.end())
        {
            Load(filepath);
        }
        return &m_resources[filepath];
    }
    // Returns copy of resource from cache, if not existing, call Load();
    virtual const T &GetCopy(const std::string &filepath) noexcept
    {
        if (m_resources.find(filepath) == m_resources.end())
        {
            Load(filepath);
        }
        return m_resources[filepath];
    }
    // Load resource into memory
    virtual void Load(const std::string &filepath) = 0;

protected:
    std::map<std::string, T> m_resources;

public:
    class Exception : public IException
    {
    public:
        Exception(int line, const char *file, const char *errorString) noexcept
            : IException(line, file),
              errorString(errorString)
        {
        }
        const char *what() const noexcept override
        {
            std::ostringstream oss;
            oss << "[Type] " << GetType() << std::endl
                << "[Description] " << GetErrorString() << std::endl
                << GetOriginString();
            whatBuffer = oss.str();
            return whatBuffer.c_str();
        }
        virtual const char *GetType() const noexcept override
        {
            return "V-2DEngine ResourceMgr Exception";
        }
        const char *GetErrorString() const noexcept
        {
            return errorString.c_str();
        }

    private:
        std::string errorString;
    };
};