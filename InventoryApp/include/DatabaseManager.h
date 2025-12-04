#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <vector>
#include "Component.h"

class DatabaseManager {
public:
    DatabaseManager(const QString& path);
    ~DatabaseManager();

    bool init();
    bool addComponent(const Component& component);
    bool removeComponent(int id);
    bool updateComponent(const Component& component);
    std::vector<Component> getAllComponents() const;
    std::vector<Component> searchComponents(const QString& query) const;

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
