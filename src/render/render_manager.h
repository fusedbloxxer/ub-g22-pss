#ifndef __RENDER_MANAGER__
#define __RENDER_MANAGER__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>

#include "../base/component.h"

class RenderManager : public Component
{
public:
    RenderManager(Component* parent, const int width, const int height, const std::string& title);
    RenderManager(Component* parent);

public:
    void process(std::function<void(sf::RenderWindow&)> onDraw);

    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window;
};

#endif