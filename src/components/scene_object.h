#ifndef __SCENE_OBJECT__
#define __SCENE_OBJECT__

#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include "component.h"

struct Drawable : public sf::Drawable, public sf::Transformable
{
};

struct Updatable
{
    virtual void update(sf::Time delta) = 0;
};

struct SceneObject : public Component, public Drawable, public Updatable
{
};

#endif