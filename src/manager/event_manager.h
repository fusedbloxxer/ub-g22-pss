#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__

#include <SFML/Window/Event.hpp>
#include <unordered_set>
#include <functional>

#include "../components/component.h"

struct EventManager : public Component
{
    EventManager(Component* parent);

    void process(const sf::Event& event);
};

#endif