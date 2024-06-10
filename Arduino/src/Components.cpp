#include "Components.h"

/**
 * @brief Constructor for the Components class.
 * 
 * Initializes the component array and sets the component count to zero.
 */
Components::Components() : componentCount(0) {
    for (int i = 0; i < MAX_COMPONENTS; ++i) {
        components[i] = nullptr;
    }
}

/**
 * @brief Adds a single component to the components array.
 * 
 * Adds the provided component to the array if there is space available.
 * 
 * @param component A pointer to the component to be added.
 */
void Components::addComponent(Component* component) {
    if (componentCount < MAX_COMPONENTS) {
        components[componentCount++] = component;
    } else {
        Serial.println("Error: Maximum number of components reached.");
    }
}

/**
 * @brief Adds multiple components to the components array.
 * 
 * Iterates through the provided array of components and adds each to the components array.
 * 
 * @param componentsArray An array of pointers to components to be added.
 * @param size The number of components in the array.
 */
void Components::addComponent(Component* componentsArray[], int size) {
    for (int i = 0; i < size; ++i) {
        addComponent(componentsArray[i]);
    }
}

/**
 * @brief Initializes all added components.
 * 
 * Calls the initialize method on each component in the array.
 */
void Components::initializeAll() {
    for (int i = 0; i < componentCount; ++i) {
        if (components[i]) {
            components[i]->initialize();
        }
    }
}
