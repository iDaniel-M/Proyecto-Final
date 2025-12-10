#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "componentdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QDebug>
#include <QHeaderView>
#include <QFileDialog>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QStandardPaths>
#include <QDir>
#include <QPageSize>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupModel();

    // 1. Configuración del Proxy (Búsqueda y Filtro)
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(componentModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->tableView->setModel(proxyModel);

    // 2. Configuración Visual
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // 3. Configuración del Filtro (USANDO NOMBRE 'Filtrar')
    ui->Filtrar->clear();
    // El índice 0 será "Filtrar...", que servirá para mostrar todo
    ui->Filtrar->addItems({"Filtrar...", "Electrónico", "Mecánico", "Herramienta", "Consumible"});

    // *** CONEXIÓN MANUAL USANDO 'Filtrar' ***
    // Asegúrate de que en tu .h el slot se llame 'on_Filtrar_currentIndexChanged'
    connect(ui->Filtrar, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_Filtrar_currentIndexChanged(int)));

    // 4. Conexión de doble click
    connect(ui->tableView, &QTableView::doubleClicked, this, &MainWindow::on_tableView_doubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete componentModel;
    delete proxyModel;
}

void MainWindow::setupModel()
{
    QSqlDatabase db = QSqlDatabase::database("inventario_connection");
    componentModel = new QSqlTableModel(this, db);
    componentModel->setTable("components");
    componentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    componentModel->setHeaderData(1, Qt::Horizontal, tr("Nombre"));
    componentModel->setHeaderData(2, Qt::Horizontal, tr("Tipo"));
    componentModel->setHeaderData(3, Qt::Horizontal, tr("Cantidad"));
    componentModel->setHeaderData(4, Qt::Horizontal, tr("Ubicación"));
    componentModel->setHeaderData(5, Qt::Horizontal, tr("Fecha Compra"));

    componentModel->select();
    ui->tableView->hideColumn(0);
}

// ---------------------- LÓGICA DEL FILTRO (USANDO 'Filtrar') ----------------------
void MainWindow::on_Filtrar_currentIndexChanged(int index)
{
    QString tipoFiltro = "";
    proxyModel->setFilterKeyColumn(2); // Columna 2 es 'Tipo'

    switch(index) {
        case 1: tipoFiltro = "Electrónico"; break;
        case 2: tipoFiltro = "Mecánico"; break;
        case 3: tipoFiltro = "Herramienta"; break;
        case 4: tipoFiltro = "Consumible"; break;
        default: tipoFiltro = ""; // Caso 0: "Filtrar..." -> Muestra todo
    }

    if (tipoFiltro.isEmpty()) {
        proxyModel->setFilterKeyColumn(-1); // Buscar en todas las columnas
        proxyModel->setFilterFixedString(""); // Quitar filtro
    } else {
        proxyModel->setFilterFixedString(tipoFiltro); // Aplicar filtro
    }
}

void MainWindow::on_lineEditBuscar_textChanged(const QString &texto)
{
    proxyModel->setFilterRegularExpression(texto);
}

// ---------------------- CRUD Y DIÁLOGOS ----------------------
void MainWindow::handleComponentDialog(int sourceRow)
{
    ComponentDialog *dialog;
    bool isEditing = (sourceRow != -1);

    if (isEditing) {
        QString nombre = componentModel->data(componentModel->index(sourceRow, 1)).toString();
        QString tipo = componentModel->data(componentModel->index(sourceRow, 2)).toString();
        int cantidad = componentModel->data(componentModel->index(sourceRow, 3)).toInt();
        QString ubicacion = componentModel->data(componentModel->index(sourceRow, 4)).toString();
        QDate fecha = componentModel->data(componentModel->index(sourceRow, 5)).toDate();

        dialog = new ComponentDialog(this, nombre, tipo, cantidad, ubicacion, fecha);
    } else {
        dialog = new ComponentDialog(this);
    }

    if (dialog->exec() == QDialog::Accepted)
    {
        int targetRow = isEditing ? sourceRow : componentModel->rowCount();

        if (!isEditing) {
            componentModel->insertRow(targetRow);
        }

        componentModel->setData(componentModel->index(targetRow, 1), dialog->getNombre());
        componentModel->setData(componentModel->index(targetRow, 2), dialog->getTipo());
        componentModel->setData(componentModel->index(targetRow, 3), dialog->getCantidad());
        componentModel->setData(componentModel->index(targetRow, 4), dialog->getUbicacion());
        componentModel->setData(componentModel->index(targetRow, 5), dialog->getFechaAdquisicion());

        if (componentModel->submitAll()) {
            QMessageBox::information(this, "Éxito", isEditing ? "Actualizado correctamente." : "Añadido correctamente.");
        } else {
            QMessageBox::warning(this, "Error", "Error al guardar: " + componentModel->lastError().text());
            componentModel->revertAll();
        }
    }
    componentModel->select();
    delete dialog;
}

void MainWindow::on_btnAnadir_clicked() { handleComponentDialog(); }

void MainWindow::on_btnEditar_clicked()
{
    QModelIndex proxyIndex = ui->tableView->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "Aviso", "Selecciona un ítem.");
        return;
    }
    int sourceRow = proxyModel->mapToSource(proxyIndex).row();
    handleComponentDialog(sourceRow);
}

void MainWindow::on_btnEliminar_clicked() {
    QModelIndex proxyIndex = ui->tableView->currentIndex();
    if (!proxyIndex.isValid()) return;

    if (QMessageBox::question(this, "Confirmar", "¿Eliminar?") == QMessageBox::Yes) {
        QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
        if (componentModel->removeRow(sourceIndex.row())) {
            componentModel->submitAll();
        }
    }
    componentModel->select();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    on_btnEditar_clicked();
}

// ---------------------- REPORTES (CORREGIDO: SIN PUNTO Y COMA) ----------------------
void MainWindow::on_btnReporte_clicked()
{
    DatabaseManager dbManager("inventario.sqlite");
    QVector<QStringList> componentes = dbManager.getAllComponentsForReport();

    if (componentes.isEmpty()) {
        QMessageBox::information(this, "Reporte", "No hay datos.");
        return;
    }

    QStringList headers = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha"};
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    // CSV
    QString csvPath = QFileDialog::getSaveFileName(this, "Guardar CSV", defaultPath + "/reporte.csv", "CSV (*.csv)");
    if (!csvPath.isEmpty()) {
        QFile csvFile(csvPath);
        if (csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&csvFile);
            out.setCodec("UTF-8");
            out << headers.join(",") << "\n";
            for (const QStringList &row : componentes) out << row.join(",") << "\n";
            csvFile.close();
            QMessageBox::information(this, "Reporte", "CSV Generado.");
        }
    }

    // PDF
    QString pdfPath = QFileDialog::getSaveFileName(this, "Guardar PDF", defaultPath + "/reporte.pdf", "PDF (*.pdf)");
    if (!pdfPath.isEmpty()) {
        QPdfWriter pdfWriter(pdfPath);
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

        QPainter painter(&pdfWriter);
        QFont font = painter.font();
        int y = 50; int startX = 20;

        font.setPixelSize(20); font.setBold(true); painter.setFont(font);
        painter.drawText(startX, y, "Reporte de Inventario"); y += 50;

        font.setPixelSize(12); painter.setFont(font);
        QVector<int> colWidths = {30, 150, 100, 50, 100, 100};
        int currentX = startX;

        for (int i = 0; i < headers.size(); ++i) {
            painter.drawText(currentX, y, headers[i]);
            currentX += colWidths[i];
        }
        y += 10; painter.drawLine(startX, y, currentX, y); y += 20;

        font.setBold(false); painter.setFont(font);
        for (const QStringList &row : componentes) {
            if (y > pdfWriter.height() - 50) { pdfWriter.newPage(); y = 50; }
            currentX = startX;
            for (int i = 0; i < row.size(); ++i) {
                painter.drawText(currentX, y, row[i]);
                currentX += colWidths[i];
            }
            y += 25;
        }
        painter.end();
        QMessageBox::information(this, "Reporte", "PDF Generado.");
    }
}
