#include "main_window.h"
#include "tabs/http_tab.h"
#include "ui_main_window.h"

namespace Per
{
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // TODO: temporary until actual tabs are implemented
    const auto httpRequestModel = CreateDefaultHttpRequestModel();

    const auto httpTab = new HttpTab(ui->centralwidget, *httpRequestModel);
    ui->centralwidget->layout()->addWidget(httpTab);
}

MainWindow::~MainWindow()
{
    delete ui;
}
} // namespace Per
