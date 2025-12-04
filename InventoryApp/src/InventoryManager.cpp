#include "InventoryManager.h"
#include <algorithm>

InventoryManager::InventoryManager(DatabaseManager& dbManager) : m_dbManager(dbManager) {}

bool InventoryManager::addComponent(const Component& component) {
    return m_dbManager.addComponent(component);
}

bool InventoryManager::removeComponent(int id) {
    return m_dbManager.removeComponent(id);
}

bool InventoryManager::updateComponent(const Component& component) {
    return m_dbManager.updateComponent(component);
}

std::vector<Component> InventoryManager::listAll() {
    return m_dbManager.getAllComponents();
}

std::vector<Component> InventoryManager::search(const QString& query) {
    return m_dbManager.searchComponents(query);
}

std::vector<Component> InventoryManager::getLowStockComponents(int threshold) {
    std::vector<Component> all = m_dbManager.getAllComponents();
    std::vector<Component> lowStock;
    std::copy_if(all.begin(), all.end(), std::back_inserter(lowStock),
                 [threshold](const Component& c) { return c.getQuantity() < threshold; });
    return lowStock;
}
