#include <QApplication>
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "InventoryManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Setup Database
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString dbPath = dir.filePath("inventory.db");

    DatabaseManager dbManager(dbPath);
    if (!dbManager.init()) {
        qDebug() << "Failed to initialize database at:" << dbPath;
        return -1;
    }

    InventoryManager inventoryManager(dbManager);

    MainWindow mainWindow(inventoryManager);
    mainWindow.show();

    return app.exec();
}
