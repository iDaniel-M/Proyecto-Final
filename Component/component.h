#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString> // Necesario para QString

class Component {
public:
    // 1. Declaración del Constructor vacío (SOLO declaración, termina en ;)
    Component();

    // 2. Declaración del Constructor con parámetros (debe coincidir exactamente)
    Component(int id, const QString &name, int quantity, double price);

    // 3. Declaración de los Getters
    int getId() const;
    QString getName() const;
    int getQuantity() const;
    double getPrice() const;

private:
    // 4. Declaración de las variables miembro (esto es lo que te falta)
    int m_id;
    QString m_name;
    int m_quantity;
    double m_price;
};

#endif // COMPONENT_H
