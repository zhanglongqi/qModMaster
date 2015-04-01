#include <QtWidgets/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QDir>

#include "QsLog.h"
#include "QsLogDest.h"
#include "mainwindow.h"
#include "modbusadapter.h"
#include "modbuscommsettings.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //init the logging mechanism
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::InfoLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("QModMaster.log"));
    QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath,true,65535,2));
    QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    //Modbus Adapter
    ModbusAdapter modbus_adapt(NULL);
    //Program settings
    ModbusCommSettings settings("qModMaster.ini");

    //show main window
    MainWindow mainWin(NULL, &modbus_adapt, &settings);
    //connect signals - slots
    QObject::connect(&modbus_adapt, SIGNAL(refreshView()), &mainWin, SLOT(refreshView()));
    QObject::connect(&mainWin, SIGNAL(resetCounters()), &modbus_adapt, SLOT(resetCounters()));
    mainWin.show();

    return a.exec();

}
