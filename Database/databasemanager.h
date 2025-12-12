#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QDebug>
#include "component.h" // Incluimos la clase que acabamos de arreglar

class DatabaseManager
{
public:
    // --- ESTA ES LA LÍNEA QUE TE FALTABA PARA QUE FUNCIONE MAINWINDOW ---
    static DatabaseManager& instance();

    // Métodos públicos
    bool openConnection();
    bool createTable();
    QList<Component> getAllComponents(); // Declaración del método

private:
    // Constructor privado (Singleton)
    DatabaseManager();

    // Prohibir copias
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
