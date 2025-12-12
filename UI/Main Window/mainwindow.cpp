#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "componentdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QPrinter>
#include <QPainter>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Filtrar->clear();
    ui->Filtrar->addItem("Todos");
    ui->Filtrar->addItems({"Electrónico", "Mecánico", "Herramienta", "Consumible"});

    refreshTable();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::refreshTable()
{
    ui->lineEditBuscar->clear();
    ui->Filtrar->setCurrentIndex(0);
    aplicarFiltros();
}
void MainWindow::aplicarFiltros()
{
    QString textoBusqueda = ui->lineEditBuscar->text();
    QString tipoSeleccionado = ui->Filtrar->currentText();

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    QString sql = "SELECT id, name, type, quantity, location, acquisition_date FROM components WHERE 1=1";
    if (!textoBusqueda.isEmpty()) {
        sql += " AND name LIKE :search";
    }
    if (tipoSeleccionado != "Todos" && !tipoSeleccionado.isEmpty()) {
        sql += " AND type = :type";
    }

    query.prepare(sql);
    if (!textoBusqueda.isEmpty()) {
        query.bindValue(":search", "%" + textoBusqueda + "%");
    }
    if (tipoSeleccionado != "Todos") {
        query.bindValue(":type", tipoSeleccionado);
    }
    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model);
    }
}

void MainWindow::on_lineEditBuscar_textChanged(const QString &)
{
    aplicarFiltros();
}

void MainWindow::on_Filtrar_currentTextChanged(const QString &)
{
    aplicarFiltros();
}

void MainWindow::on_btnAnadir_clicked()
{
    ComponentDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO components (name, type, quantity, location, acquisition_date) "
                      "VALUES (:name, :type, :quantity, :location, :date)");

        query.bindValue(":name", dialog.getNombre());
        query.bindValue(":type", dialog.getTipo());
        query.bindValue(":quantity", dialog.getCantidad());
        query.bindValue(":location", dialog.getUbicacion());
        query.bindValue(":date", dialog.getFechaAdquisicion());
        if (query.exec()) {
            QMessageBox::information(this, "Listo", "Componente añadido.");
            refreshTable();
        } else {
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void MainWindow::on_btnEditar_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Selecciona una fila para editar.");
        return;
    }

    int row = index.row();
    QAbstractItemModel *m = ui->tableView->model();
    int id = m->index(row, 0).data().toInt();
    QString nombre = m->index(row, 1).data().toString();
    QString tipo = m->index(row, 2).data().toString();
    int cantidad = m->index(row, 3).data().toInt();
    QString ubicacion = m->index(row, 4).data().toString();
    QDate fecha = m->index(row, 5).data().toDate();

    ComponentDialog dialog(this, nombre, tipo, cantidad, ubicacion, fecha);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE components SET name=:n, type=:t, quantity=:q, location=:l, acquisition_date=:d "
                      "WHERE id=:id");
        query.bindValue(":n", dialog.getNombre());
        query.bindValue(":t", dialog.getTipo());
        query.bindValue(":q", dialog.getCantidad());
        query.bindValue(":l", dialog.getUbicacion());
        query.bindValue(":d", dialog.getFechaAdquisicion());
        query.bindValue(":id", id);
        if (query.exec()) {
            refreshTable();
            QMessageBox::information(this, "Listo", "Editado correctamente.");
        } else {
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void MainWindow::on_btnEliminar_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Selecciona una fila para eliminar.");
        return;
    }
    if (QMessageBox::question(this, "Confirmar", "¿Borrar este elemento?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        int id = ui->tableView->model()->index(index.row(), 0).data().toInt();
        QSqlQuery query;
        query.prepare("DELETE FROM components WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            refreshTable();
        } else {
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void MainWindow::on_btnReporte_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Guardar Reporte", QDir::homePath() + "/reporte.pdf", "Archivos PDF (*.pdf)");

    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Error", "No se pudo iniciar el PDF.");
        return;
    }
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(50, 100, "REPORTE DE INVENTARIO");
    painter.setFont(QFont("Arial", 10));
    painter.drawText(50, 130, "Fecha: " + QDate::currentDate().toString("dd/MM/yyyy"));

    int y = 180;

    QSqlQuery query("SELECT name, quantity, type, location FROM components");

    while (query.next()) {
        QString texto = QString("- %1: %2 un. (%3) en %4")
                        .arg(query.value(0).toString())
                        .arg(query.value(1).toString())
                        .arg(query.value(2).toString())
                        .arg(query.value(3).toString());
        painter.drawText(50, y, texto);
        y += 25;

        if (y > 1000) {
            printer.newPage();
            y = 50;
        }
    }

    painter.end();
    QMessageBox::information(this, "PDF Guardado", "Archivo creado en:\n" + fileName);
}
