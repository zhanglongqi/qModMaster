#include "about.h"
#include "ui_about.h"

const QString VER = "QModMaster 0.4.4";
const QString URL = "<a href = ""http://sourceforge.net/projects/qmodmaster"">Sourceforge Project Home Page</a>";

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->lblVersion->setText(VER);
    ui->lbnURL->setText(URL);
}

About::~About()
{
    delete ui;
}
