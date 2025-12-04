#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QLabel>
#include "InventoryManager.h"
#include "DatabaseManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(InventoryManager& inventoryManager, QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleAddButton();
    void handleSearch();
    void handleTableSelection();
    void handleUpdateButton();
    void handleDeleteButton();
    void handleReportButton();

private:
    void setupUI();
    void refreshTable(const std::vector<Component>& components);
    void clearForm();

    InventoryManager& m_inventoryManager;

    // UI Elements
    QTableWidget* m_tableWidget;
    QLineEdit* m_searchEdit;

    // Form Elements
    QLineEdit* m_nameEdit;
    QLineEdit* m_typeEdit;
    QLineEdit* m_quantityEdit;
    QLineEdit* m_locationEdit;
    QDateEdit* m_dateEdit;

    QPushButton* m_addButton;
    QPushButton* m_updateButton;
    QPushButton* m_deleteButton;
    QPushButton* m_reportButton;

    // State
    int m_selectedId = -1;
};

#endif // MAINWINDOW_H
