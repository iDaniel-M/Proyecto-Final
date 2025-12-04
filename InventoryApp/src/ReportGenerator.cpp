#include "ReportGenerator.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

bool ReportGenerator::generateCSV(const std::vector<Component>& components, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return false;
    }

    QTextStream out(&file);
    out << "ID,Nombre,Tipo,Cantidad,Ubicacion,FechaCompra\n";

    for (const auto& comp : components) {
        out << comp.getId() << ","
            << comp.getName() << ","
            << comp.getType() << ","
            << comp.getQuantity() << ","
            << comp.getLocation() << ","
            << comp.getPurchaseDate().toString(Qt::ISODate) << "\n";
    }

    file.close();
    return true;
}
