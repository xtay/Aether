#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonGrid, SIGNAL(clicked()), ui->viewer, SLOT(toggleGridIsDrawn()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
