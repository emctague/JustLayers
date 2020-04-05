// ECS - Provides Entity Component System classes for JustLayers.
// 
// Copyright (c) 2020 Ethan McTague
#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

namespace jl {


    /// Represents any sort of Component available to an entity.
    class Component {
    public:
        virtual ~Component() = default;
    };


    /// Represents an entity within the game world.
    class Entity {
    private:

        /// Used internally to generate UUIDs.
        static unsigned uuidCounter;

        /// The components contained by this entity.
        std::unordered_map<std::type_index, std::shared_ptr<Component>> components;

    public:

        /// Create a new entity.
        Entity();

        /// Create an entity with the given components. Useful for setup in code.
        template<class ...Types>
        Entity (Types*... components) : Entity() {
            addComponents(components...);
        }


        /// A Unique Identifier for this entity. Note that identifiers are not necessarily unique across levels, or when
        /// more entities have been created than the maximum number of unsigned integers (unlikely!)
        const unsigned uuid;

        /// Obtain the component on this entity of the given type.
        template<typename T>
        std::shared_ptr<T> getComponent() {
            std::type_index index(typeid(T));
            return components.count(index) != 0 ? std::static_pointer_cast<T>(components[index]) : nullptr;
        }

        /// Add a component to the entity.
        template<typename T>
        Entity *addComponent(T *component) {
            components[std::type_index(typeid(T))] = std::shared_ptr<Component>(component);
            return this;
        }

        /// Base case for 'addComponents' - does nothing.
        inline Entity *addComponents() {
            return this;
        }

        /// Add multiple components at once.
        template<typename T, typename ...Types>
        inline Entity *addComponents(T *val, Types *...values) {
            addComponent<T>(val);
            addComponents(values...);
            return this;
        }

    };



    /// Something that wants to be informed of changes to the ECS.
    class IECSListener {
    public:

        /// Called after an entity has been added.
        virtual void entityAdded(Entity *entity) = 0;

        /// Called before an entity is removed.
        virtual void entityRemoving(Entity *entity) = 0;
    };


    /// Handles entities, components, and anything that needs to know about them.
    class ECS {

        /// Tracks all entities currently in the ECS.
        std::unordered_map<unsigned, std::unique_ptr<Entity>> entities;

        /// Tracks all ECS listeners.
        std::vector<IECSListener *> listeners;

    public:

        /// Add an entity to the ECS. The ECS will take ownership of the entity.
        void addEntity(Entity *e);

        /// Remove an entity from the ECS.
        void removeEntity(Entity *e);

        /// Register a listener which wants to be informed of entity updates.
        void addECSListener(IECSListener *listener);

        /// Remove a listener.
        void removeECSListener(IECSListener *listener);
    };

};
