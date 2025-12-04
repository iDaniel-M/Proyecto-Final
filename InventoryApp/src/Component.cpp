#include "Component.h"

Component::Component() : m_id(-1), m_quantity(0) {}

Component::Component(int id, const QString& name, const QString& type, int quantity, const QString& location, const QDate& purchaseDate)
    : m_id(id), m_name(name), m_type(type), m_quantity(quantity), m_location(location), m_purchaseDate(purchaseDate) {}

int Component::getId() const { return m_id; }
void Component::setId(int id) { m_id = id; }

QString Component::getName() const { return m_name; }
void Component::setName(const QString& name) { m_name = name; }

QString Component::getType() const { return m_type; }
void Component::setType(const QString& type) { m_type = type; }

int Component::getQuantity() const { return m_quantity; }
void Component::setQuantity(int quantity) { m_quantity = quantity; }

QString Component::getLocation() const { return m_location; }
void Component::setLocation(const QString& location) { m_location = location; }

QDate Component::getPurchaseDate() const { return m_purchaseDate; }
void Component::setPurchaseDate(const QDate& purchaseDate) { m_purchaseDate = purchaseDate; }
