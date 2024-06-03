#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>

class Component {
    public:
        virtual void initialize() = 0;
        virtual ~Component() = default;
};

class Components {
    public:
        Components();
        void addComponent(Component*);
        void addComponent(Component*[], int );
        void initializeAll();

    private:
        static const int MAX_COMPONENTS{10};
        Component *components[MAX_COMPONENTS];
        int componentCount;
};

#endif // COMPONENTS_H
