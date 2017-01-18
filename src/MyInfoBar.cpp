#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "MyInfoBar.h"

MyInfoBar::MyInfoBar(QWidget *parent) : QFrame(parent)
{
    label = new QLabel;

    QPushButton *button = new QPushButton;
    button->setStyleSheet("QPushButton{image: url(:/icons/close8-black.png);\
                                       background-color: lightgrey;\
                                       border: none;\
                                       height: 12px;\
                                       width: 12px}\
                           QPushButton:hover{image: url(:/icons/close8-red.png);\
                                             background-color: lightgrey;\
                                             border: none;\
                                             height: 12px;\
                                             width: 12px}\
                           QPushButton:pressed{image: url(:/icons/close8-red.png);\
                                             background-color: lightgrey;\
                                             border: 1px solid darkgrey;\
                                             border-radius: 2px;\
                                             height: 12px;\
                                             width: 12px}");
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setFocusPolicy(Qt::NoFocus);
    connect(button, SIGNAL(clicked()), this, SLOT(hide()));

    QHBoxLayout *hboxlayout = new QHBoxLayout;
    hboxlayout->addWidget(label);
    hboxlayout->addWidget(button);

    setLayout(hboxlayout);
    setFrameStyle(QFrame::Box);
    hide();
}

MyInfoBar::MyInfoBar(QString message, InfoType type, QWidget *parent) : MyInfoBar(parent)
{
    setMessage(message);
    setInfoType(type);
}

void MyInfoBar::setInfoType(InfoType type)
{
    switch (type) {
        case Question:
            setStyleSheet("color: black;\
                          background-color: skyblue");
            break;
        case Error:
            setStyleSheet("color: white;\
                          background-color: red");
            break;
        default:
        case Warning:
            setStyleSheet("color: black;\
                          background-color: yellow");
            break;
    }
}

void MyInfoBar::setMessage(QString message)
{
    label->setText(message);
}

void MyInfoBar::show(QString message, InfoType type)
{
    setMessage(message);
    setInfoType(type);
    QFrame::show();
}
