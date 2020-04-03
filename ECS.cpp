// ECS
// 
// Copyright (c) 2020 Ethan McTague
#include "JustLayers/ECS.h"
#include <algorithm>

namespace jl {

    unsigned Entity::uuidCounter = 0;

    Entity::Entity() : uuid(uuidCounter++) {

    }

    void ECS::addEntity(Entity *e) {
        entities[e->uuid] = std::unique_ptr<Entity>(e);

        for (auto& listener : listeners) {
            listener->entityAdded(e);
        }
    }

    void ECS::removeEntity(Entity *e) {
        for (auto& listener : listeners) {
            listener->entityRemoving(e);
        }

        entities.erase(e->uuid);
    }

    void ECS::addECSListener(IECSListener *listener) {
        listeners.push_back(listener);
    }

    void ECS::removeECSListener(IECSListener *listener) {
        listeners.erase(std::find(listeners.begin(), listeners.end(), listener));
    }
}