#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QDate>

class Component {
public:
    Component();
    Component(int id, const QString& name, const QString& type, int quantity, const QString& location, const QDate& purchaseDate);

    int getId() const;
    void setId(int id);

    QString getName() const;
    void setName(const QString& name);

    QString getType() const;
    void setType(const QString& type);

    int getQuantity() const;
    void setQuantity(int quantity);

    QString getLocation() const;
    void setLocation(const QString& location);

    QDate getPurchaseDate() const;
    void setPurchaseDate(const QDate& purchaseDate);

private:
    int m_id;
    QString m_name;
    QString m_type;
    int m_quantity;
    QString m_location;
    QDate m_purchaseDate;
};

#endif // COMPONENT_H
