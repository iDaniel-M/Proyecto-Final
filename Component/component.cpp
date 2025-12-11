#include "component.h"

// Implementación del constructor por defecto (si se usa en alguna parte)
Component::Component()
    : m_quantity(0)
{
}

// Implementación del constructor principal
Component::Component(const QString &name, const QString &type, int quantity, const QString &location, const QDate &purchaseDate)
    : m_name(name),
      m_type(type),
      m_quantity(quantity),
      m_location(location),
      m_purchaseDate(purchaseDate)
{
}
