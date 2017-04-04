#ifndef MACOSEVENTLISTENER_H
#define MACOSEVENTLISTENER_H

#include <string>
#include <functional>

struct Event {
public:
    enum class Modifier {
        None = 0,
        Ctrl = 1 << 1,
        Alt = 1 << 2,
        Cmd = 1 << 3,
        Shift = 1 << 4,
    };

    Modifier modifiers;
    std::string key;

};

void initMacOsEventListener(std::function<void(Event)> listener);

#endif // MACOSEVENTLISTENER_H
