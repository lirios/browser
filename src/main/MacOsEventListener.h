#ifndef MACOSEVENTLISTENER_H
#define MACOSEVENTLISTENER_H

#include <string>
#include <functional>
#include <QObject>

enum class TabEvent {
    ShiftTab,
    CtrlShiftTab
};
class MacOsEventListener;

MacOsEventListener* initMacOsEventListener(MacOsEventListener *parent);

class MacOsEventListener : public QObject {
    Q_OBJECT

public:
    MacOsEventListener(QObject* parent = 0) : QObject(parent) {}

signals:
    void ctrlShiftTabPressed();
    void ctrlTabPressed();

};

#endif // MACOSEVENTLISTENER_H
