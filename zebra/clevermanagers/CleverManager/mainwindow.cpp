#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "zebra_peer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initZebra();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initZebra()
{
#if defined(ZEBRA_MODULE)
    Zebra_Peer *peer = Zebra_Peer::bulid(this);
#endif
}
