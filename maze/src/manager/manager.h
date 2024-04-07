#ifndef __MANAGER__
#define __MANAGER__

#include <functional>
#include <filesystem>
#include <vector>
#include <memory>

#include "../components/scene_object.h"
#include "../components/component.h"
#include "resource_manager.h"
#include "render_manager.h"
#include "event_manager.h"

class Manager : public Component
{
public:
    Manager();

    std::filesystem::path getInputPath();

    std::filesystem::path getOutputPath();

    void init();

    void setTitle(const std::string& title);

    void setResourcePath(const std::filesystem::path&);

    void setResolution(const int width, const int height);

    void addSceneObject(std::shared_ptr<SceneObject> obj);

    void runLoop();

private:
    void process();

    void draw(sf::RenderWindow& window);

private:
    std::vector<std::shared_ptr<SceneObject>> _sceneObjects;

    sf::Clock _clock;
};

#endif