#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "component.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QVector> // Necesario para QVector<QStringList>
#include <QStringList> // Necesario para QVector<QStringList>

class DatabaseManager
{
public:
    DatabaseManager(const QString &databaseName = "inventario.sqlite");

    bool openDatabase();
    bool createTable();

    // Operación CREATE
    bool addComponent(const Component &component);

    // Operación READ (Lectura para la interfaz, si la usaras)
    QList<Component> allComponents() const;

    // Operación READ para Reporte (¡NUEVO!)
    QVector<QStringList> getAllComponentsForReport() const;

private:
    QSqlDatabase m_db;
    const QString DB_NAME;
};

#endif // DATABASEMANAGER_H
