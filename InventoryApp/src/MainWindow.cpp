#include "MainWindow.h"
#include "ReportGenerator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QGroupBox>

MainWindow::MainWindow(InventoryManager& inventoryManager, QWidget* parent)
    : QMainWindow(parent), m_inventoryManager(inventoryManager) {
    setupUI();
    refreshTable(m_inventoryManager.listAll());
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Left Side: Table and Search
    QVBoxLayout* leftLayout = new QVBoxLayout();

    // Search Bar
    QHBoxLayout* searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Buscar por nombre o tipo...");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::handleSearch);
    searchLayout->addWidget(new QLabel("Buscar:"));
    searchLayout->addWidget(m_searchEdit);
    leftLayout->addLayout(searchLayout);

    // Table
    m_tableWidget = new QTableWidget();
    m_tableWidget->setColumnCount(6);
    m_tableWidget->setHorizontalHeaderLabels({"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha"});
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::handleTableSelection);
    leftLayout->addWidget(m_tableWidget);

    // Report Button
    m_reportButton = new QPushButton("Generar Reporte CSV");
    connect(m_reportButton, &QPushButton::clicked, this, &MainWindow::handleReportButton);
    leftLayout->addWidget(m_reportButton);

    mainLayout->addLayout(leftLayout, 2); // 2/3 width

    // Right Side: Form
    QGroupBox* formGroup = new QGroupBox("Detalles del Componente");
    QVBoxLayout* rightLayout = new QVBoxLayout(formGroup);
    QFormLayout* formLayout = new QFormLayout();

    m_nameEdit = new QLineEdit();
    m_typeEdit = new QLineEdit();
    m_quantityEdit = new QLineEdit();
    m_quantityEdit->setValidator(new QIntValidator(0, 1000000, this));
    m_locationEdit = new QLineEdit();
    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setCalendarPopup(true);

    formLayout->addRow("Nombre:", m_nameEdit);
    formLayout->addRow("Tipo:", m_typeEdit);
    formLayout->addRow("Cantidad:", m_quantityEdit);
    formLayout->addRow("Ubicación:", m_locationEdit);
    formLayout->addRow("Fecha Compra:", m_dateEdit);

    rightLayout->addLayout(formLayout);

    // Buttons
    m_addButton = new QPushButton("Agregar");
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::handleAddButton);
    rightLayout->addWidget(m_addButton);

    m_updateButton = new QPushButton("Actualizar");
    m_updateButton->setEnabled(false);
    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::handleUpdateButton);
    rightLayout->addWidget(m_updateButton);

    m_deleteButton = new QPushButton("Eliminar");
    m_deleteButton->setEnabled(false);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButton);
    rightLayout->addWidget(m_deleteButton);

    rightLayout->addStretch();
    mainLayout->addWidget(formGroup, 1); // 1/3 width

    setWindowTitle("Gestor de Inventario");
    resize(1000, 600);
}

void MainWindow::refreshTable(const std::vector<Component>& components) {
    m_tableWidget->setRowCount(0);
    for (const auto& comp : components) {
        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        m_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(comp.getId())));
        m_tableWidget->setItem(row, 1, new QTableWidgetItem(comp.getName()));
        m_tableWidget->setItem(row, 2, new QTableWidgetItem(comp.getType()));
        m_tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(comp.getQuantity())));
        m_tableWidget->setItem(row, 4, new QTableWidgetItem(comp.getLocation()));
        m_tableWidget->setItem(row, 5, new QTableWidgetItem(comp.getPurchaseDate().toString(Qt::ISODate)));

        // Highlight low stock
        if (comp.getQuantity() < 5) {
            for (int i = 0; i < 6; ++i) {
                m_tableWidget->item(row, i)->setBackground(Qt::red);
                m_tableWidget->item(row, i)->setForeground(Qt::white);
            }
        }
    }
}

void MainWindow::clearForm() {
    m_nameEdit->clear();
    m_typeEdit->clear();
    m_quantityEdit->clear();
    m_locationEdit->clear();
    m_dateEdit->setDate(QDate::currentDate());
    m_selectedId = -1;
    m_updateButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_tableWidget->clearSelection();
}

void MainWindow::handleAddButton() {
    if (m_nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "El nombre es obligatorio");
        return;
    }

    Component comp(-1, m_nameEdit->text(), m_typeEdit->text(), m_quantityEdit->text().toInt(),
                   m_locationEdit->text(), m_dateEdit->date());

    if (m_inventoryManager.addComponent(comp)) {
        refreshTable(m_inventoryManager.listAll());
        clearForm();
        QMessageBox::information(this, "Éxito", "Componente agregado");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo agregar el componente");
    }
}

void MainWindow::handleSearch() {
    QString query = m_searchEdit->text();
    if (query.isEmpty()) {
        refreshTable(m_inventoryManager.listAll());
    } else {
        refreshTable(m_inventoryManager.search(query));
    }
}

void MainWindow::handleTableSelection() {
    auto items = m_tableWidget->selectedItems();
    if (items.isEmpty()) return;

    int row = items.first()->row();
    m_selectedId = m_tableWidget->item(row, 0)->text().toInt();
    m_nameEdit->setText(m_tableWidget->item(row, 1)->text());
    m_typeEdit->setText(m_tableWidget->item(row, 2)->text());
    m_quantityEdit->setText(m_tableWidget->item(row, 3)->text());
    m_locationEdit->setText(m_tableWidget->item(row, 4)->text());
    m_dateEdit->setDate(QDate::fromString(m_tableWidget->item(row, 5)->text(), Qt::ISODate));

    m_updateButton->setEnabled(true);
    m_deleteButton->setEnabled(true);
}

void MainWindow::handleUpdateButton() {
    if (m_selectedId == -1) return;

    Component comp(m_selectedId, m_nameEdit->text(), m_typeEdit->text(), m_quantityEdit->text().toInt(),
                   m_locationEdit->text(), m_dateEdit->date());

    if (m_inventoryManager.updateComponent(comp)) {
        refreshTable(m_inventoryManager.listAll());
        clearForm();
        QMessageBox::information(this, "Éxito", "Componente actualizado");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar");
    }
}

void MainWindow::handleDeleteButton() {
    if (m_selectedId == -1) return;

    if (QMessageBox::question(this, "Confirmar", "¿Estás seguro de eliminar este componente?") == QMessageBox::Yes) {
        if (m_inventoryManager.removeComponent(m_selectedId)) {
            refreshTable(m_inventoryManager.listAll());
            clearForm();
        } else {
            QMessageBox::critical(this, "Error", "No se pudo eliminar");
        }
    }
}

void MainWindow::handleReportButton() {
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar Reporte", "", "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        if (ReportGenerator::generateCSV(m_inventoryManager.listAll(), fileName)) {
            QMessageBox::information(this, "Éxito", "Reporte guardado correctamente");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo guardar el reporte");
        }
    }
}
