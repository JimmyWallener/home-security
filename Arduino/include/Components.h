#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>

class Component {
    public:
        virtual void initialize() = 0;
};

class Components {
    public:
        Components();
        void addComponent(Component *component);
        void addComponent(Component *component[], int size);
        void initializeAll();

    private:
        static const int MAX_COMPONENTS = 10;
        Component *components[MAX_COMPONENTS];
        int componentCount;
};

#endif // COMPONENTS_H
