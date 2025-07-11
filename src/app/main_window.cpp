#include "main_window.h"
#include "tabs/http_tab.h"
#include "ui_main_window.h"

namespace Per
{
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const auto httpRequest = new HttpRequestModel{.name = "Google",
                                                  .url = "https://www.google.com/",
                                                  .method = GET,
                                                  .headers = {},
                                                  .parameters = {},
                                                  .contentType = "",
                                                  .bodyContent = ""};
    SetDefaultHttpHeaders(httpRequest);

    httpRequest->parameters.append({.enabled = false, .key = "hello", .value = "world"});

    const auto httpTab = new HttpTab(this, httpRequest);

    ui->centralwidget->layout()->addWidget(httpTab);
}

MainWindow::~MainWindow()
{
    delete ui;
}
} // namespace Per
