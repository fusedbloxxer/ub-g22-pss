#ifndef __COMPONENT__
#define __COMPONENT__

#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <string>

class Component
{
public:
    Component(Component* component = nullptr)
        : parent(component) {}

    virtual ~Component() = default;

public:
    template<typename T> requires std::is_base_of_v<Component, T> || std::is_same_v<Component, T>
    inline void addComponent(std::shared_ptr<T> component)
    {
        this->children[typeid(*component)] = component;
    }

    inline bool hasParent() const
    {
        return this->parent != nullptr;
    }

public:
    template<typename T> requires std::is_base_of_v<Component, T> || std::is_same_v<Component, T>
    inline std::shared_ptr<T> getComponent(bool direct = true)
    {
        return this->getChildOrAncestor<T>(direct ? 1 : -1);
    }

    template<typename T> requires std::is_base_of_v<Component, T> || std::is_same_v<Component, T>
    inline std::shared_ptr<T> getChildOrSibling()
    {
        return this->getChildOrAncestor<T>(2);
    }

    template<typename T> requires std::is_base_of_v<Component, T> || std::is_same_v<Component, T>
    std::shared_ptr<T> getChild()
    {
        const std::type_info& type = typeid(T);

        if (auto child = this->children.find(type); child != this->children.end())
        {
            return std::dynamic_pointer_cast<T>(child->second);
        }

        return std::shared_ptr<T>(nullptr);
    }

    template<typename T> requires std::is_base_of_v<Component, T> || std::is_same_v<Component, T>
    std::shared_ptr<T> getChildOrAncestor(long long int depth = -1)
    {
        for (auto node = this; node != nullptr && depth != 0; --depth)
        {
            if (auto component = node->getChild<T>(); component != nullptr)
            {
                return component;
            }

            node = node->parent;
        }

        return std::shared_ptr<T>(nullptr);
    }

protected:
    Component* parent = nullptr;

private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> children;
};

#endif