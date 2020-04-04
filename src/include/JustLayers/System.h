// System - Provides a simple, convenient template for ECS Systems.
// 
// Copyright (c) 2020 Ethan McTague
#pragma once
#include "ECS.h"
#include "Taskmaster.h"
#include <optional>
#include <tuple>
#include <unordered_map>

namespace jl {

    /// Provides assistance in developing a custom System, which is both a Task and an ECS Listener.
    template<typename T, typename ...RequiredComponents>
    class System : public IECSListener, public ITask {
    private:

        /// Used to allow recursive iteration of required components.
        template<typename...>
        struct dummy_typed_struct {
        };

        /// Iterate through each required component (in RequiredComponents) and ensure that the entity contains that
        /// component. If so, keep building a tuple of those components, otherwise give up and return null.
        template<typename ComponentType, typename ...FurtherTypes>
        std::optional<std::tuple<ComponentType, FurtherTypes...>>
        buildWithComponents(Entity *entity, dummy_typed_struct<ComponentType, FurtherTypes...>) {
            auto component = entity->getComponent<ComponentType>();

            if (component == nullptr)
                return nullptr;

            auto prior = buildWithComponents(entity, dummy_typed_struct<FurtherTypes...>());
            if (!prior.has_value()) return nullptr;

            return std::tuple_cat(std::make_tuple(component), prior.value());
        }

        /// Base case for buildWithComponents recursion - just provides an empty tuple.
        std::optional<std::tuple<>> buildWithComponents(Entity *entity, dummy_typed_struct<>) {
            return std::make_tuple();
        }


    protected:


        /// A list of all entities (specifically, their required components in a tuple)
        std::unordered_map<unsigned, std::tuple<RequiredComponents...>> entities;


        /// The Entity Component System architecture this system is a part of.
        ECS *ecs;

        /// The taskmaster which handles this task.
        Taskmaster *taskmaster;

        System(ECS *ecs, Taskmaster *taskmaster) : ecs(ecs), taskmaster(taskmaster) {
            ecs->addECSListener(this);
            taskmaster->addTask(IndirectTask::make(this));
        }

        /// Called by the system *after* an entity has been added to the entities list.
        virtual void onEntityAdded(Entity *entity) {}

        /// Called by the system *before* an entity has been removed from the entities list.
        virtual void onEntityRemoving(Entity *entity) {}



    public:


        void entityAdded(Entity *entity) final {

            // Obtain a tuple of all the components this system needs from the entity.
            std::optional<std::tuple<RequiredComponents...>> components = buildWithComponents(entity, dummy_typed_struct<RequiredComponents...>());

            // If the tuple wasn't returned it means this entity isn't something we need to care about.
            if (!components) return;

            entities.insert(entity->uuid, components.value());
            onEntityAdded(entity);
        }


        void entityRemoving(Entity *entity) final {
            // Remove the entity from this system if we actually have it.
            if (entities.count(entity->uuid)) {
                onEntityRemoving(entity);
                entities.erase(entity->uuid);
            }
        }

        virtual ~System() {
            ecs->removeECSListener(this);
        }

    };

}