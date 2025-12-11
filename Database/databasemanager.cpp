#include "databasemanager.h"
#include <QSqlRecord>
#include <QList>
#include <QVariant>

// Implementación del constructor
DatabaseManager::DatabaseManager(const QString &databaseName)
    : DB_NAME(databaseName)
{
}

bool DatabaseManager::openDatabase()
{
    if (QSqlDatabase::contains("inventario_connection")) {
        m_db = QSqlDatabase::database("inventario_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "inventario_connection");
        m_db.setDatabaseName(DB_NAME);
    }

    if (!m_db.open()) {
        qDebug() << "ERROR: No se pudo abrir la base de datos:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Conexión a SQLite establecida con éxito:" << DB_NAME;
    return true;
}

bool DatabaseManager::createTable()
{
    QSqlQuery query(m_db);
    QString sql = "CREATE TABLE IF NOT EXISTS components ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "name TEXT NOT NULL,"
                  "type TEXT,"
                  "quantity INTEGER NOT NULL,"
                  "location TEXT,"
                  "purchase_date DATE"
                  ");";

    if (!query.exec(sql)) {
        qDebug() << "ERROR al crear la tabla components:" << query.lastError().text();
        return false;
    }
    qDebug() << "Tabla 'components' verificada/creada.";
    return true;
}

bool DatabaseManager::addComponent(const Component &component)
{
    QSqlQuery query(m_db);
    QString sql = "INSERT INTO components (name, type, quantity, location, purchase_date) "
                  "VALUES (:name, :type, :quantity, :location, :purchase_date)";

    query.prepare(sql);
    query.bindValue(":name", component.name());
    query.bindValue(":type", component.type());
    query.bindValue(":quantity", component.quantity());
    query.bindValue(":location", component.location());
    query.bindValue(":purchase_date", component.purchaseDate().toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "ERROR al añadir componente:" << query.lastError().text();
        return false;
    }
    qDebug() << "Componente añadido con éxito:" << component.name();
    return true;
}

QList<Component> DatabaseManager::allComponents() const
{
    QList<Component> components;
    // La implementación no es crítica, ya que la interfaz usa QSqlTableModel
    return components;
}

// IMPLEMENTACIÓN DE REPORTE (Lectura completa de todos los campos)
QVector<QStringList> DatabaseManager::getAllComponentsForReport() const
{
    QVector<QStringList> componentsData;
    QSqlQuery query(m_db);

    // Selecciona todos los campos de la base de datos
    if (query.exec("SELECT id, name, type, quantity, location, purchase_date FROM components ORDER BY name ASC"))
    {
        while (query.next())
        {
            QStringList row;
            // Mapea cada valor de la base de datos a un string en la lista para el reporte
            row << query.value("id").toString();
            row << query.value("name").toString();
            row << query.value("type").toString();
            row << query.value("quantity").toString();
            row << query.value("location").toString();
            row << query.value("purchase_date").toString();

            componentsData.append(row);
        }
    } else {
        qDebug() << "ERROR Reporte: No se pudo leer la base de datos:" << query.lastError().text();
    }

    return componentsData;
}
