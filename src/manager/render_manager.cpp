#include "render_manager.h"

RenderManager::RenderManager(Component *parent, const int width, const int height, const std::string &title)
    : Component(parent), window(sf::VideoMode(width, height), title, sf::Style::Default)
{
    this->window.setFramerateLimit(144);
    this->window.setVisible(false);
}

RenderManager::RenderManager(Component *parent)
    : RenderManager(parent, 1280, 1024, "Maze")
{
}

void RenderManager::process(std::function<void(sf::RenderWindow&)> onDraw)
{
    this->window.clear();

    onDraw(this->window);

    this->window.display();
}

sf::RenderWindow& RenderManager::getWindow()
{
    return this->window;
}