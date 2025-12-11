#include "componentdialog.h"
#include "ui_componentdialog.h"

// Constructor para AÑADIR
ComponentDialog::ComponentDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentDialog)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle("Añadir Nuevo Componente");
}

// Constructor para EDITAR
ComponentDialog::ComponentDialog(QWidget *parent, const QString &nombre, const QString &tipo,
                                 int cantidad, const QString &ubicacion, const QDate &fecha)
    : QDialog(parent), ui(new Ui::ComponentDialog)
{
    ui->setupUi(this); // Configura la interfaz visual
    setupUI();         // Configura los items del combo y fecha
    setWindowTitle("Editar Componente");

    // Precargar datos (Usando los nombres corregidos del paso 2)
    ui->lineEdit->setText(nombre);
    ui->spinBox->setValue(cantidad);
    ui->lineEdit_2->setText(ubicacion);
    ui->dateEdit->setDate(fecha);

    // Seleccionar el tipo correcto
    int index = ui->comboTipo->findText(tipo);
    if (index != -1) {
        ui->comboTipo->setCurrentIndex(index);
    }
}

ComponentDialog::~ComponentDialog()
{
    delete ui;
}

void ComponentDialog::setupUI()
{
    // Rellenar las opciones
    ui->comboTipo->addItems({"Electrónico", "Mecánico", "Herramienta", "Consumible"});

    // Configurar fecha
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setMaximumDate(QDate::currentDate());
}

// Getters
QString ComponentDialog::getNombre() const { return ui->lineEdit->text(); }
QString ComponentDialog::getTipo() const { return ui->comboTipo->currentText(); }
int ComponentDialog::getCantidad() const { return ui->spinBox->value(); }
QString ComponentDialog::getUbicacion() const { return ui->lineEdit_2->text(); }
QDate ComponentDialog::getFechaAdquisicion() const { return ui->dateEdit->date(); }
