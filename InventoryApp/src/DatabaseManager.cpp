#include "DatabaseManager.h"
#include <QVariant>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::init() {
    if (!m_db.open()) {
        qDebug() << "Error: Connection with database failed";
        return false;
    }

    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS components ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL,"
                              "type TEXT,"
                              "quantity INTEGER,"
                              "location TEXT,"
                              "purchase_date TEXT)");

    if (!success) {
        qDebug() << "CreateTable error:" << query.lastError();
    }
    return success;
}

bool DatabaseManager::addComponent(const Component& component) {
    QSqlQuery query;
    query.prepare("INSERT INTO components (name, type, quantity, location, purchase_date) "
                  "VALUES (:name, :type, :quantity, :location, :purchase_date)");
    query.bindValue(":name", component.getName());
    query.bindValue(":type", component.getType());
    query.bindValue(":quantity", component.getQuantity());
    query.bindValue(":location", component.getLocation());
    query.bindValue(":purchase_date", component.getPurchaseDate().toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "Add component error:" << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::removeComponent(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM components WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool DatabaseManager::updateComponent(const Component& component) {
    QSqlQuery query;
    query.prepare("UPDATE components SET name = :name, type = :type, quantity = :quantity, "
                  "location = :location, purchase_date = :purchase_date WHERE id = :id");
    query.bindValue(":name", component.getName());
    query.bindValue(":type", component.getType());
    query.bindValue(":quantity", component.getQuantity());
    query.bindValue(":location", component.getLocation());
    query.bindValue(":purchase_date", component.getPurchaseDate().toString(Qt::ISODate));
    query.bindValue(":id", component.getId());

    return query.exec();
}

std::vector<Component> DatabaseManager::getAllComponents() const {
    std::vector<Component> list;
    QSqlQuery query("SELECT * FROM components");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString type = query.value("type").toString();
        int quantity = query.value("quantity").toInt();
        QString location = query.value("location").toString();
        QDate date = QDate::fromString(query.value("purchase_date").toString(), Qt::ISODate);
        list.emplace_back(id, name, type, quantity, location, date);
    }
    return list;
}

std::vector<Component> DatabaseManager::searchComponents(const QString& text) const {
    std::vector<Component> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM components WHERE name LIKE :query OR type LIKE :query");
    query.bindValue(":query", "%" + text + "%");
    query.exec();

    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString type = query.value("type").toString();
        int quantity = query.value("quantity").toInt();
        QString location = query.value("location").toString();
        QDate date = QDate::fromString(query.value("purchase_date").toString(), Qt::ISODate);
        list.emplace_back(id, name, type, quantity, location, date);
    }
    return list;
}
