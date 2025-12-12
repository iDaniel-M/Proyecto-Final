#include "databasemanager.h"

// Implementación del Singleton
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager _instance;
    return _instance;
}

// Constructor
DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("inventario.sqlite");

    if (!m_db.open()) {
        qDebug() << "Error fatal al inicio:" << m_db.lastError().text();
    }
}

bool DatabaseManager::openConnection()
{
    if (m_db.isOpen()) return true;
    return m_db.open();
}

bool DatabaseManager::createTable()
{
    if (!openConnection()) return false;

    QSqlQuery query;
    QString sql = "CREATE TABLE IF NOT EXISTS items ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "nombre TEXT, "
                  "cantidad INTEGER, "
                  "precio REAL)";
    return query.exec(sql);
}

// Implementación de getAllComponents
QList<Component> DatabaseManager::getAllComponents()
{
    QList<Component> lista;
    if (!openConnection()) return lista;

    QSqlQuery query("SELECT id, nombre, cantidad, precio FROM items");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString nombre = query.value(1).toString();
        int cantidad = query.value(2).toInt();
        double precio = query.value(3).toDouble();

        // Creamos el componente y lo añadimos
        lista.append(Component(id, nombre, cantidad, precio));
    }
    return lista;
}
