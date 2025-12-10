#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots de Botones
    void on_btnAnadir_clicked();
    void on_btnEditar_clicked();
    void on_btnEliminar_clicked();
    void on_btnReporte_clicked();

    // Slots de Interacción
    void on_lineEditBuscar_textChanged(const QString &texto);

    // *** CORRECCIÓN: Ahora el nombre coincide con tu UI 'Filtrar' ***
    void on_Filtrar_currentIndexChanged(int index);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    void setupModel();
    void handleComponentDialog(int sourceRow = -1);

    Ui::MainWindow *ui;
    QSqlTableModel *componentModel;
    QSortFilterProxyModel *proxyModel;
};

#endif // MAINWINDOW_H
