#include "app/main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication per(argc, argv);

    Per::MainWindow window;

    window.show();

    return per.exec();
}
