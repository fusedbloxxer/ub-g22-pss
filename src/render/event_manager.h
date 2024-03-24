#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__

#include <SFML/Window/Event.hpp>
#include <functional>

#include "../base/component.h"

class EventManager : public Component
{
public:
    EventManager(Component* parent);

public:
    void process(const sf::Event& event);
};

#endif