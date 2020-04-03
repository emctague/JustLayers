// ResourceManager - Provides a generic resource management tool.
// 
// Copyright (c) 2020 Ethan McTague
#pragma once
#include <unordered_map>
#include <functional>

namespace jl {

    /// A ResourceManager handles resources of type `T`, which can be loaded and differentiated via some value of type
    /// `ResourceKey`.
    template<typename T, typename ResourceKey>
    class ResourceManager {

        /// Actively loaded resources.
        std::unordered_map<ResourceKey, std::shared_ptr<T>> resources;

        /// A function that loads resources given a value of the key type.
        std::function<std::shared_ptr<T>(ResourceKey)> loader;

    public:

        /// Initialize this resource manager, passing a function which, when provided with a value of the resource key
        /// type, can produce a shared pointer to a resource of that type.
        explicit ResourceManager(std::function<std::shared_ptr<T>(ResourceKey)> loader) : loader(loader) {

        }

        /// Get a resource, reusing an already loaded instance if possible.
        std::shared_ptr<T> get(ResourceKey key) {
            if (!resources.count(key))
                resources[key] = loader(key);

            return resources[key];
        }

        /// Purge unused resources from the list.
        void cleanup() {
            for (auto it = resources.begin(); it < resources.end(); it++) {
                if (it->use_count() < 2) resources.erase(it--);
            }
        }
    };

}