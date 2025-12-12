#include "componentdialog.h"
#include "ui_componentdialog.h"

ComponentDialog::ComponentDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentDialog)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle("Añadir Nuevo Componente");
}

ComponentDialog::ComponentDialog(QWidget *parent, const QString &nombre, const QString &tipo,
                                 int cantidad, const QString &ubicacion, const QDate &fecha)
    : QDialog(parent), ui(new Ui::ComponentDialog)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle("Editar Componente");

    ui->lineEdit->setText(nombre);
    ui->spinBox->setValue(cantidad);
    ui->lineEdit_2->setText(ubicacion);
    ui->dateEdit->setDate(fecha);

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
    ui->comboTipo->addItems({"Electrónico", "Mecánico", "Herramienta", "Consumible"});
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setMaximumDate(QDate::currentDate());
}

QString ComponentDialog::getNombre() const { return ui->lineEdit->text(); }
QString ComponentDialog::getTipo() const { return ui->comboTipo->currentText(); }
int ComponentDialog::getCantidad() const { return ui->spinBox->value(); }
QString ComponentDialog::getUbicacion() const { return ui->lineEdit_2->text(); }
QDate ComponentDialog::getFechaAdquisicion() const { return ui->dateEdit->date(); }
