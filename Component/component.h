#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QDate>

class Component
{
public:
    // Constructores
    Component();
    // Constructor principal usado para crear nuevos objetos a insertar
    Component(const QString &name, const QString &type, int quantity, const QString &location, const QDate &purchaseDate);

    // Getters
    QString name() const { return m_name; }
    QString type() const { return m_type; }
    int quantity() const { return m_quantity; }
    QString location() const { return m_location; }
    QDate purchaseDate() const { return m_purchaseDate; }

private:
    // Nota: El ID no se maneja aquí, lo autogenera SQLite (es una buena práctica)
    QString m_name;
    QString m_type;
    int m_quantity;
    QString m_location;
    QDate m_purchaseDate;
};

#endif // COMPONENT_H
