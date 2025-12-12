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

    if (!dbManager.openDatabase() || !dbManager.createTable()) {
        QMessageBox::critical(nullptr, "Error Crítico", "Fallo al iniciar la Base de Datos.");
        return 1;
    }

    Component testComponent("Resistencia 10K Ohm", "Pasivo", 200, "Caja R1", QDate::currentDate());

    if (dbManager.addComponent(testComponent)) {
        qDebug() << "Artículo de prueba insertado.";
    } else {
        qDebug() << "Fallo en la inserción de prueba.";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
