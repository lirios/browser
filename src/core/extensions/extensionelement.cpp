#include "extensionelement.h"

ExtensionElement::ExtensionElement(QObject *parent)
    : QObject(parent)
{
    m_valid = true;
}

ExtensionElement *ExtensionElement::clone(QObject *parent)
{
    ExtensionElement* element = new ExtensionElement(parent);
    element->setValid(valid());
    element->setExtensionName(extensionName());
    element->setName(name());
    return element;
}
