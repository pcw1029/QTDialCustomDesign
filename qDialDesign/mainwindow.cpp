#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dial = new CustomDial(this);
    ui->verticalLayout->addWidget(dial);
}

MainWindow::~MainWindow()
{
    delete ui;
}

