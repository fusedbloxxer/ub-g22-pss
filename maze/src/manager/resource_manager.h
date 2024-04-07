#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <filesystem>

#include "../components/scene_object.h"

class ResourceManager : public Component
{
    std::filesystem::path resourcePath;

public:
    ResourceManager(Component* parent, const std::filesystem::path& path);
    ResourceManager(Component* parent);

public:
    std::filesystem::path getResourcePath() const;
    std::filesystem::path getTexturePath() const;
    std::filesystem::path getOutputPath() const;
    std::filesystem::path getInputPath() const;
    void setPath(const std::filesystem::path&);
};

#endif