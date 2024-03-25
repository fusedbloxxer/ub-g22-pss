#include "event_manager.h"
#include "render_manager.h"

EventManager::EventManager(Component* parent)
    : Component(parent) {}

void EventManager::process(const sf::Event &event)
{
    if (event.type == sf::Event::Closed)
    {
        this->getChildOrSibling<RenderManager>()->getWindow().close();
    }
}