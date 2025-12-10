#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "databasemanager.h"
#include "component.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager dbManager("inventario.sqlite");

    // 1. Verificar la conexión y la tabla
    if (!dbManager.openDatabase() || !dbManager.createTable()) {
        QMessageBox::critical(nullptr, "Error Crítico", "Fallo al iniciar la Base de Datos. La aplicación terminará.");
        return 1;
    }

    // 2. PRUEBA DE INSERCIÓN (CREACIÓN DE REGISTRO)
    Component testComponent("Resistencia 10K Ohm", "Pasivo", 200, "Caja R1", QDate::currentDate());

    if (dbManager.addComponent(testComponent)) {
        qDebug() << "Test: Artículo de prueba insertado correctamente.";
    } else {
        qDebug() << "Test: Fallo en la inserción de prueba.";
    }

    // 3. Iniciar la Interfaz si todo es exitoso
    MainWindow w;
    w.show();

    return a.exec();
}
