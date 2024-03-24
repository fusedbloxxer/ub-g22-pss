#include "resource_manager.h"

ResourceManager::ResourceManager(Component* parent, const std::filesystem::path& path)
    : Component(parent), resourcePath(path) {}
ResourceManager::ResourceManager(Component* parent)
    : ResourceManager(parent, "resources") {}

std::filesystem::path ResourceManager::getResourcePath() const
{
    return this->resourcePath;
}

std::filesystem::path ResourceManager::getTexturePath() const
{
    return this->resourcePath / "textures";
}

std::filesystem::path ResourceManager::getInputPath() const
{
    return this->resourcePath / "maze";
}

std::filesystem::path ResourceManager::getOutputPath() const
{
    return this->resourcePath / ".." / "output.txt";
}

void ResourceManager::setPath(const std::filesystem::path& path)
{
    this->resourcePath = path;
}
