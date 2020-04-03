// Registry - a generic framework for registering subclasses of some type.
// 
// Copyright (c) 2020 Ethan McTague
#pragma once
#include <unordered_map>
#include <string>
#include <functional>

namespace jl {

    /// A Registry allows subclasses of the type `T` to be registered with a string key, along with a function that,
    /// given arguments of types from `Args`, creates an instance of that type.
    template<typename T, typename... Args>
    class Registry {

        /// A map that associates strings with functions that generate instances of subclasses.
        std::unordered_map<std::string, std::function<T*(Args...)>> creators;

    public:

        /// Add a new type to the registry. `name` is the name that will be used to create instances of this type, and
        /// `factory` is a function which, given a set of `Args` will return a pointer to a new instance of the type.
        void add(std::string name, std::function<T*(Args...)> factory) {
            creators[name] = factory;
        }

        /// Create an instance of a type registered in the registry, using the given arguments.
        T *create(std::string type, Args ...args) {
            if (creators.count(type))
                return creators[type](args...);
            else return nullptr;
        }

        /// Create a shared_ptr instance of the registered type.
        std::shared_ptr<T> create_shared(const std::string& type, Args ...args) {
            return std::shared_ptr<T>(create(type, args...));
        }

    };

}