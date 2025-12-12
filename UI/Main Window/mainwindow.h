#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // --- BOTONES (Nombres según tu diseño .ui) ---
    void on_btnAnadir_clicked();
    void on_btnEditar_clicked();
    void on_btnEliminar_clicked();
    void on_btnReporte_clicked(); // <--- AQUÍ ESTÁ EL NOMBRE CORRECTO

    // --- FILTROS ---
    void on_lineEditBuscar_textChanged(const QString &arg1);
    void on_Filtrar_currentTextChanged(const QString &arg1);

    // Función auxiliar
    void refreshTable();
    void aplicarFiltros();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
