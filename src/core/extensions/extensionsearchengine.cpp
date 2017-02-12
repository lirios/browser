#include "extensionsearchengine.h"

ExtensionSearchEngine::ExtensionSearchEngine(QObject *parent)
    : ExtensionElement(parent)
{

}

ExtensionSearchEngine *ExtensionSearchEngine::clone(QObject *parent)
{
    ExtensionSearchEngine* searchEngine = new ExtensionSearchEngine(parent);
    searchEngine->setName(name());
    searchEngine->setExtensionName(extensionName());
    searchEngine->setTitle(title());
    searchEngine->setSummary(summary());
    searchEngine->setDescription(description());
    return searchEngine;
}
