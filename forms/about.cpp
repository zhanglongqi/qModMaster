#include "about.h"
#include "ui_about.h"
#include "modbus-version.h"

const QString VER = "QModMaster 0.4.7";
const QString LIB_VER = LIBMODBUS_VERSION_STRING;
const QString URL = "<a href = ""http://sourceforge.net/projects/qmodmaster"">Sourceforge Project Home Page</a>";

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->lblVersion->setText(VER);
    ui->lblLibVersion->setText("libmodbus " + LIB_VER);
    ui->lblURL->setText(URL);
}

About::~About()
{
    delete ui;
}
