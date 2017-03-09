#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_on_button_clicked()
{
    transmit_voltage(current_to_voltage(45));
}

void MainWindow::on_off_button_clicked()
{
    transmit_voltage(current_to_voltage(0));
}
