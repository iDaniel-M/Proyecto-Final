#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "DatabaseManager.h"
#include <vector>
#include <QString>

class InventoryManager {
public:
    InventoryManager(DatabaseManager& dbManager);

    bool addComponent(const Component& component);
    bool removeComponent(int id);
    bool updateComponent(const Component& component);
    std::vector<Component> listAll();
    std::vector<Component> search(const QString& query);
    std::vector<Component> getLowStockComponents(int threshold);

private:
    DatabaseManager& m_dbManager;
};

#endif // INVENTORYMANAGER_H
