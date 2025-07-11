#include "main_window.h"
#include "tabs/http_tab.h"
#include "ui_main_window.h"

#include <QPushButton>

namespace Per
{
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // TODO: temporary until actual tabs are implemented
    const auto httpRequestModel = new HttpRequestModel {.name = "Google",
                                                  .url = "https://www.google.com/",
                                                  .method = GET,
                                                  .headers = {},
                                                  .parameters = {},
                                                  .contentType = "",
                                                  .bodyContent = ""};
    SetDefaultHttpHeaders(*httpRequestModel);

    const auto httpTab = new HttpTab(ui->centralwidget, *httpRequestModel);
    ui->centralwidget->layout()->addWidget(httpTab);
}

MainWindow::~MainWindow()
{
    delete ui;
}
} // namespace Per
