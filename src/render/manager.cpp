#include <functional>
#include <algorithm>

#include "manager.h"

Manager::Manager()
{
    this->addComponent<ResourceManager>(std::make_unique<ResourceManager>(this));
    this->addComponent<RenderManager>(std::make_unique<RenderManager>(this));
    this->addComponent<EventManager>(std::make_unique<EventManager>(this));
}

void Manager::init()
{
    this->getComponent<RenderManager>()->getWindow().setVisible(true);
}

std::filesystem::path Manager::getInputPath()
{
    return this->getComponent<ResourceManager>()->getInputPath();
}

std::filesystem::path Manager::getOutputPath()
{
    return this->getComponent<ResourceManager>()->getOutputPath();
}

void Manager::setResourcePath(const std::filesystem::path &path)
{
    this->getComponent<ResourceManager>()->setPath(path);
}

void Manager::setResolution(const int width, const int height)
{
    this->getComponent<RenderManager>()->getWindow().setSize(sf::Vector2u(width, height));
}

void Manager::setTitle(const std::string &title)
{
    this->getComponent<RenderManager>()->getWindow().setTitle(title);
}

void Manager::addSceneObject(std::shared_ptr<SceneObject> obj)
{
    this->_sceneObjects.push_back(obj);
}

void Manager::runLoop()
{
    this->_clock = sf::Clock();

    while (this->getComponent<RenderManager>()->getWindow().isOpen())
    {
        this->process();
    }
}

void Manager::process()
{
    for (auto event = sf::Event(); this->getComponent<RenderManager>()->getWindow().pollEvent(event);)
    {
        this->getComponent<EventManager>()->process(event);
    }

    sf::Time delta = this->_clock.restart();

    for (auto obj : _sceneObjects)
    {
        obj->update(delta);
    }

    this->getComponent<RenderManager>()->process([this](sf::RenderWindow &window)
    {
        this->draw(window);
    });
}

void Manager::draw(sf::RenderWindow &window)
{
    for (auto obj : _sceneObjects)
    {
        window.draw(*obj);
    }
}
