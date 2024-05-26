#include "Components.h"

Components::Components() : componentCount(0) {
    for (int i = 0; i < MAX_COMPONENTS; ++i) {
        components[i] = nullptr;
    }
}


void Components::addComponent(Component* component) {
    if (componentCount < MAX_COMPONENTS) {
        components[componentCount++] = component;
    } else {
        Serial.println("Error: Maximum number of components reached.");
    }
}

void Components::addComponent(Component* componentsArray[], int size) {
    for (int i = 0; i < size; ++i) {
        addComponent(componentsArray[i]);
    }
}


void Components::initializeAll() {
    for (int i = 0; i < componentCount; ++i) {
        if (components[i]) {
            components[i]->initialize();
        }
    }
}
// SensorLog.h
