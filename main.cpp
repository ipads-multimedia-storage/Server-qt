#include "Master.h"
#include "widgets.h"

#include <QApplication>

QString SettingsWidget::ADDR = "122.230.103.223:50080";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;

    return a.exec();
}
