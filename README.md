# Minigin

## Introduction

Welcome to Minigin, a lightweight game engine created with the goal of learning programming patterns and expanding my knowledge and proficiency in game development.

During the development of Minigin, several design decisions were made to ensure flexibility, performance, and ease of use. This README provides an overview of these design choices and the reasoning behind them.

## Design Decisions

### Start Event Initialization

One of the design decisions in Minigin is the incorporation of a "start" event before the game loop begins. This event allows components to initialize themselves and perform any necessary setup tasks before the game starts. By providing a dedicated start event, developers can ensure that crucial objects are properly constructed and prepared before they are accessed during the game loop, leading to smoother and more reliable gameplay.

### Use of Delegates

Minigin leverages the use of delegates, which serve as powerful tools to tie different parts of the game together. Delegates enable efficient communication between objects and allow for dynamic function binding. By utilizing delegates, developers can decouple components and create more flexible and reusable systems within the game engine.

### Singleton Pattern

Minigin incorporates the Singleton pattern to facilitate centralized management. This pattern is primarily employed for frequently accessed resources and critical components. By utilizing single instances of these objects, Minigin ensures easy access and simplifies the organization and coordination of different components.

#### GameTime Singleton

By implementing GameTime as a singleton in Minigin, developers can easily access the delta time without the need to pass it throughout the update methods explicitly. This approach provides several advantages:

1. **Simplicity and Convenience**: As a singleton, GameTime can be accessed from anywhere within the game engine without the need for additional parameter passing. Developers can effortlessly retrieve the delta time whenever necessary, reducing code complexity and enhancing code readability.

2. **Consistency**: GameTime ensures that the delta time value remains consistent across all update methods, eliminating the possibility of unintentional variations due to separate timekeeping instances. This consistency is crucial for maintaining synchronized and predictable gameplay.

3. **Efficiency**: With GameTime as a singleton, the engine avoids unnecessary overhead that would be incurred by passing the delta time as a parameter. This streamlined approach allows for more efficient and optimized game updates, improving overall performance.

By leveraging the GameTime singleton, Minigin simplifies time management in game development, enabling developers to focus on implementing core gameplay features rather than handling time-related intricacies.

### Resource Manager Singleton

Minigin includes a resource manager singleton that ensures efficient loading and management of textures and fonts within the game engine. The resource manager operates as a centralized repository, preventing redundant loading of resources and providing easy access to already loaded assets.

When a texture or font is requested, the resource manager checks if it has already been loaded by referring to a map with the file path as the key. If the resource is found, the resource manager returns the existing instance. Otherwise, it loads the resource, stores it in the map for future use, and returns the newly loaded instance.

### Simple Collision System

Minigin features a simple collision system that includes OnCollisionEnter and OnCollisionExit events. The collision system is currently integrated into the engine's update loop, providing a straightforward and lightweight solution for handling basic collision detection and response.

As the collision system in Minigin is designed to handle simple interactions, it fits seamlessly within the update loop. By processing collisions during each frame update, the engine ensures timely and responsive collision behavior.

For future expansion, if more complex collision functionalities or a fixed time step update are required, the collision system can be further developed to meet those needs. However, for its current implementation, the integration into the update loop provides an efficient and effective solution.

### Grid Classes

Minigin includes grid classes inspired by the Elite framework provided during my gameplay programming course at DAE. These grid classes have been simplified and tailored to seamlessly fit into the lightweight 2D engine. Developers can efficiently organize game elements and implement grid-based functionality, such as pathfinding or spatial partitioning, using these grid classes. The The PacMan project within this repository leverages these grid classes by using additional custom classes to enhance the functionality of the grid system. These custom classes provide specific features and management capabilities, allowing the grid to come to life.

### Scene Graph and Component-Based Architecture

Taking inspiration from Unity, Minigin utilizes a scene graph and component-based architecture to provide a flexible and intuitive way to structure game objects. The scene graph enables easy access to components and facilitates the construction of variations of objects by adding, removing, or modifying game object components. 
