#ifndef COMPONENTDIALOG_H
#define COMPONENTDIALOG_H

#include <QDialog>
#include <QDate>
#include "component.h" // Incluimos el modelo si queremos pasar objetos, o simplemente usamos tipos básicos

QT_BEGIN_NAMESPACE
namespace Ui { class ComponentDialog; }
QT_END_NAMESPACE

class ComponentDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor para AÑADIR (valores por defecto)
    explicit ComponentDialog(QWidget *parent = nullptr);

    // Constructor para EDITAR (carga datos existentes)
    // Usaremos los tipos de datos simples para la edición.
    ComponentDialog(QWidget *parent, const QString &nombre, const QString &tipo,
                    int cantidad, const QString &ubicacion, const QDate &fecha);

    ~ComponentDialog();

    // Getters para recuperar los datos del formulario (¡CRUCIAL!)
    QString getNombre() const;
    QString getTipo() const;
    int getCantidad() const;
    QString getUbicacion() const;
    QDate getFechaAdquisicion() const;

private:
    Ui::ComponentDialog *ui;
    void setupUI(); // Inicializa QComboBox con los tipos
};

#endif // COMPONENTDIALOG_H
