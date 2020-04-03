# JustLayers

This is a small library that might serve as the foundation of a simple game engine. In this architecture, there is no
singular 'Engine' / 'Game' object that handles everything - rather, several smaller parts can be used together in a
nice, flexible manner to make a working engine.

I am logging my development process [here](https://forums.tigsource.com/index.php?topic=69774.0).

The **Taskmaster** is an object responsible for managing a set of `ITasks` (*Tasks*). Tasks provide some functionality that should
occur repeatedly (i.e. every frame) until it is no longer needed. The Taskmaster effectively provides the functionality
of the main game loop in most engines.

The **ECS** keeps track of Entities, Components, and of course... ...*IECSListeners*? Entities are composed of any
number of Components, and `IECSListeners` are notified of the addition and deletion of entities.

Notice that **Systems** were notably absent from the ECS design? This is because **Systems** are where the Taskmaster
and the ECS come together - systems need to be aware of entities and their components, and also need to perform updates
on each frame. Hence, a System class needs to be both an `ITask` and `IECSListener`. For convenience, a base template
class `System` is provided - this class can automatically keep a list of Entities which possess the particular
components your system deals with, as well as self-registering with the ECS and Task system.

The **ResourceManager** is a type that can keep track of some sort of resource. Imagine, for example, you need to load
textures, ensuring you only unload them when nothing uses them anymore. With `ResourceManager`, these textures would be
stored in a map whose key is the same as the set of values necessary to initialize that texture. The `ResourceManager`
can load the texture when asked to, but will instead reuse already loaded resources whenever it can.

Let's look at a very small, very incomplete example:

```c++
#include <JustLayers/ECS.h>
#include <JustLayers/Taskmaster.h>
#include <JustLayers/System.h>

class RenderComponent : public jl::Component {
    // ...info on the mesh to use?
};

class RenderSystem : public jl::System<RenderSystem, RenderComponent> {
public:
    
    RenderSystem(jl::ECS *e, jl::Taskmaster *t) : System(e, t) {
        // Any required setup
    }
    
    bool updateTask(jl::Taskmaster *t) override {

        // Iterates over every entity that has a RenderComponent.
        // renderableEntity is actually a tuple of the entity's required components.
        for (auto& renderableEntity : entities) {
            auto renderComp = std::get<RenderComponent>(renderableEntity);
            // Some code to render...
        }
    
        // Return true because the system must keep going.
        return true;

    }

};


int main() {
    jl::ECS ecs;
    jl::Taskmaster taskmaster;

    RenderSystem rs(ecs, taskmaster);

    // ...add a system to stop the taskmaster when the player hits close, etc....

    ecs.addEntity(
        new Entity()
            .addComponent(new RenderComponent(/* ... whatever this component needs ... */))
    );

    taskmaster.start();
}
```

The RenderSystem would be informed, of course, of the entity being added, as it contains a RenderComponent, the system's
only required component. Then, once the taskmaster starts, the system would have the opportunity to render this entity
each frame.


This framework leaves a lot of flexibility - multiple taskmasters might be made to coordinate across threads, one might
add a way to dynamically load and unload systems depending on the current state, etc... Most importantly, the means by
which different things update, and the means by which entities are managed are kept separate at their core - the ECS
could be replaced with a more traditional OOP scene graph without any changes to the Task system, or one might move onto
a fancier task scheduling system in the future.
