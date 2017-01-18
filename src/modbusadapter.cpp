#include "modbusadapter.h"
#include <QtDebug>
#include "mainwindow.h"

#include "QsLog.h"
#include <errno.h>

ModbusAdapter *m_instance;

ModbusAdapter::ModbusAdapter(QObject *parent) :
    QObject(parent),
    m_modbus(NULL)
{
    m_instance = this;
    regModel=new RegistersModel(this);
    rawModel=new RawDataModel(this);
    m_connected = false;
    m_ModBusMode = EUtils::None;
    m_pollTimer = new QTimer(this);
    m_timeOut = 0;
    m_transactionIsPending = false;
    m_packets = 0;
    m_errors = 0;
    connect(m_pollTimer,SIGNAL(timeout()),this,SLOT(modbusTransaction()));
    connect(regModel,SIGNAL(refreshView()),this,SIGNAL(refreshView()));
}

void ModbusAdapter::modbusConnectRTU(QString port, int baud, QChar parity, int dataBits, int stopBits, int RTS, int timeOut)
{
    //Modbus RTU connect
    QString line;
    modbusDisConnect();

    QLOG_INFO()<<  "Modbus Connect RTU";

    m_modbus = modbus_new_rtu(port.toLatin1().constData(),baud,parity.toLatin1(),dataBits,stopBits,RTS);
    line = "Connecting to Serial Port [" + port + "]...";
    QLOG_INFO() <<  line;

    //Debug messages from libmodbus
    #ifdef LIB_MODBUS_DEBUG_OUTPUT
        modbus_set_debug(m_modbus, 1);
    #endif

    m_timeOut = timeOut;

    if(m_modbus && modbus_connect(m_modbus) == -1) {
        mainWin->showUpInfoBar(tr("Connection failed\nCould not connect to serial port."), MyInfoBar::Error);
        QLOG_ERROR()<<  "Connection failed. Could not connect to serial port";
        m_connected = false;
        line += "Failed";
    }
    else {
        //TODO : RTU
        //struct timeval response_timeout;
        //response_timeout.tv_sec = timeOut;
        //response_timeout.tv_usec = 0;
        modbus_set_response_timeout(m_modbus, timeOut, 0);
        m_connected = true;
        line += "OK";
        mainWin->hideInfoBar();
        QLOG_INFO() << line;
    }

    m_ModBusMode = EUtils::RTU;

    //Add line to raw data model
    line = EUtils::SysTimeStamp() + " : " + line;
    rawModel->addLine(line);

}

void ModbusAdapter::modbusConnectTCP(QString ip, int port, int timeOut)
{
    //Modbus TCP connect
    QString strippedIP = "";
    QString line;
    modbusDisConnect();

    QLOG_INFO()<<  "Modbus Connect TCP";

    line = "Connecting to IP : " + ip + ":" + port;
    QLOG_INFO() <<  line;
    strippedIP = stripIP(ip);
    if (strippedIP == ""){
        mainWin->showUpInfoBar(tr("Connection failed\nBlank IP Address."), MyInfoBar::Error);
        QLOG_ERROR()<<  "Connection failed. Blank IP Address";
        return;
    }
    else {
        m_modbus = modbus_new_tcp(strippedIP.toLatin1().constData(), port);
        mainWin->hideInfoBar();
        QLOG_INFO() <<  "Connecting to IP : " << ip << ":" << port;
    }

    //Debug messages from libmodbus
    #ifdef LIB_MODBUS_DEBUG_OUTPUT
        modbus_set_debug(m_modbus, 1);
    #endif

    m_timeOut = timeOut;

    if(m_modbus && modbus_connect(m_modbus) == -1) {
        mainWin->showUpInfoBar(tr("Connection failed\nCould not connect to TCP port."), MyInfoBar::Error);
        QLOG_ERROR()<<  "Connection to IP : " << ip << ":" << port << "...failed. Could not connect to TCP port";
        m_connected = false;
        line += "Failed";
    }
    else {
        //TODO : TCP
        //struct timeval response_timeout;
        //response_timeout.tv_sec = timeOut;
        //response_timeout.tv_usec = 0;
        modbus_set_response_timeout(m_modbus, timeOut, 0);
        m_connected = true;
        line += "OK";
        mainWin->hideInfoBar();
        QLOG_INFO() << line;
    }

    m_ModBusMode = EUtils::TCP;

    //Add line to raw data model
    line = EUtils::SysTimeStamp() + " : " + line;
    rawModel->addLine(line);

}


void ModbusAdapter::modbusDisConnect()
{
    //Modbus disconnect

    QLOG_INFO()<<  "Modbus disconnected";

    if(m_modbus) {
        modbus_close(m_modbus);
        modbus_free(m_modbus);
        m_modbus = NULL;
    }

    m_connected = false;

    m_ModBusMode = EUtils::None;

}

bool ModbusAdapter::isConnected()
{
    //Modbus is connected

    return m_connected;
}

void ModbusAdapter::modbusTransaction()
{
    //Modbus request data

    QLOG_INFO() <<  "Modbus Transaction. Function Code = " << m_functionCode;
    m_packets += 1;

    switch(m_functionCode)
    {
            case MODBUS_FC_READ_COILS:
            case MODBUS_FC_READ_DISCRETE_INPUTS:
            case MODBUS_FC_READ_HOLDING_REGISTERS:
            case MODBUS_FC_READ_INPUT_REGISTERS:
                    modbusReadData(m_slave,m_functionCode,m_startAddr,m_numOfRegs);
                    break;

            case MODBUS_FC_WRITE_SINGLE_COIL:
            case MODBUS_FC_WRITE_SINGLE_REGISTER:
            case MODBUS_FC_WRITE_MULTIPLE_COILS:
            case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
                    modbusWriteData(m_slave,m_functionCode,m_startAddr,m_numOfRegs);
                    break;
            default:
                    break;
    }

    emit(refreshView());

}

void ModbusAdapter::modbusReadData(int slave, int functionCode, int startAddress, int noOfItems)
{

    QLOG_INFO() <<  "Modbus Read Data ";

    if(m_modbus == NULL) return;

    uint8_t dest[1024]; //setup memory for data
    uint16_t * dest16 = (uint16_t *) dest;
    memset(dest, 0, 1024);
    int ret = -1; //return value from read functions
    bool is16Bit = false;

    modbus_set_slave(m_modbus, slave);
    //request data from modbus
    switch(functionCode)
    {
            case MODBUS_FC_READ_COILS:
                    ret = modbus_read_bits(m_modbus, startAddress, noOfItems, dest);
                    break;

            case MODBUS_FC_READ_DISCRETE_INPUTS:
                    ret = modbus_read_input_bits(m_modbus, startAddress, noOfItems, dest);
                    break;

            case MODBUS_FC_READ_HOLDING_REGISTERS:
                    ret = modbus_read_registers(m_modbus, startAddress, noOfItems, dest16);
                    is16Bit = true;
                    break;

            case MODBUS_FC_READ_INPUT_REGISTERS:
                    ret = modbus_read_input_registers(m_modbus, startAddress, noOfItems, dest16);
                    is16Bit = true;
                    break;

            default:
                    break;
    }

    QLOG_INFO() <<  "Modbus Read Data return value = " << ret;

    //update data model
    if(ret == noOfItems)
    {
            for(int i = 0; i < noOfItems; ++i)
            {
                int data = is16Bit ? dest16[i] : dest[i];
                regModel->setValue(i,data);
            }
            mainWin->hideInfoBar();
    }
    else
    {

        regModel->setNoValidValues();
        m_errors += 1;

        QString line = "";
        if(ret < 0) {
                line = QString("System exception. [") +  modbus_strerror(errno) + "]";
                QLOG_ERROR() <<  "Modbus Read Data failed. " << line;
                rawModel->addLine(EUtils::SysTimeStamp() + " : " + line);
                line = QString(tr("Read data failed.\nSystem exception. [")) +  modbus_strerror(errno) + "]";
        }
        else {
                line = QString("Number of registers returned does not match number of registers requested!. [")  +  modbus_strerror(errno) + "]";
                QLOG_ERROR() <<  "Modbus Read Data failed. " << line;
                rawModel->addLine(EUtils::SysTimeStamp() + " : " + line);
                line = QString(tr("Read data failed.\nNumber of registers returned does not match number of registers requested!. ["))  +  modbus_strerror(errno) + "]";
        }

        mainWin->showUpInfoBar(line, MyInfoBar::Error);
        modbus_flush(m_modbus); //flush data
     }

}

void ModbusAdapter::modbusWriteData(int slave, int functionCode, int startAddress, int noOfItems)
{

    QLOG_INFO() <<  "Modbus Write Data ";

    if(m_modbus == NULL) return;

    int ret = -1; //return value from functions

    modbus_set_slave(m_modbus, slave);
    //request data from modbus
    switch(functionCode)
    {
            case MODBUS_FC_WRITE_SINGLE_COIL:
                    ret = modbus_write_bit(m_modbus, startAddress,regModel->value(0));
                    noOfItems = 1;
                    break;

            case MODBUS_FC_WRITE_SINGLE_REGISTER:
                    ret = modbus_write_register( m_modbus, startAddress,regModel->value(0));
                    noOfItems = 1;
                    break;

            case MODBUS_FC_WRITE_MULTIPLE_COILS:
            {
                    uint8_t * data = new uint8_t[noOfItems];
                    for(int i = 0; i < noOfItems; ++i)
                    {
                            data[i] = regModel->value(i);
                    }
                    ret = modbus_write_bits(m_modbus, startAddress, noOfItems, data);
                    delete[] data;
                    break;
            }
            case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
            {
                    uint16_t * data = new uint16_t[noOfItems];
                    for(int i = 0; i < noOfItems; ++i)
                    {
                            data[i] = regModel->value(i);
                    }
                    ret = modbus_write_registers(m_modbus, startAddress, noOfItems, data);
                    delete[] data;
                    break;
            }

            default:
                    break;
    }

    QLOG_INFO() <<  "Modbus Write Data return value = " << ret;

    //update data model
    if(ret == noOfItems)
    {
        //values written correctly
        rawModel->addLine(EUtils::SysTimeStamp() + " : values written correctly.");
        mainWin->hideInfoBar();
    }
    else
    {

        regModel->setNoValidValues();
        m_errors += 1;

        QString line;
        if(ret < 0) {
                line = QString("System exception. [") +  modbus_strerror(errno) + "]";
                QLOG_ERROR() <<  "Modbus Write Data failed. " << line;
                rawModel->addLine(EUtils::SysTimeStamp() + " : " + line);
                line = QString(tr("Read data failed.\nSystem exception. [")) +  modbus_strerror(errno) + "]";
        }
        else {
                line = QString("Number of registers returned does not match number of registers requested!. [")  +  modbus_strerror(errno) + "]";
                QLOG_ERROR() <<  "Modbus Write Data failed. " << line;
                rawModel->addLine(EUtils::SysTimeStamp() + " : " + line);
                line = QString(tr("Write data failed.\nNumber of registers returned does not match number of registers requested!. ["))  +  modbus_strerror(errno) + "]";
         }

        mainWin->showUpInfoBar(line, MyInfoBar::Error);
        modbus_flush(m_modbus); //flush data
     }

}

void ModbusAdapter::busMonitorRequestData(uint8_t * data, uint8_t dataLen)
{

    //Request Raw data from port - Update raw data model

    QString line;

    for(int i = 0; i < dataLen; ++i ) {
        line += QString().sprintf( "%.2x  ", data[i] );
    }

    QLOG_INFO() << "Tx Data : " << line;
    line = EUtils::TxTimeStamp(m_ModBusMode) + " : " + line.toUpper();

    rawModel->addLine(line);

    m_transactionIsPending = true;

}
void ModbusAdapter::busMonitorResponseData(uint8_t * data, uint8_t dataLen)
{

    //Response Raw data from port - Update raw data model

    QString line;

    for(int i = 0; i < dataLen; ++i ) {
        line += QString().sprintf( "%.2x  ", data[i] );
    }

    QLOG_INFO() << "Rx Data : " << line;
    line = EUtils::RxTimeStamp(m_ModBusMode) + " : " + line.toUpper();

    rawModel->addLine(line);

    m_transactionIsPending = false;

}

void ModbusAdapter::setSlave(int slave)
{
    m_slave = slave;
}

void ModbusAdapter::setFunctionCode(int functionCode)
{
    m_functionCode = functionCode;
}

void ModbusAdapter::setStartAddr(int addr)
{
    m_startAddr = addr;
}

void ModbusAdapter::setNumOfRegs(int num)
{
    m_numOfRegs = num;
}

void ModbusAdapter::addItems()
{
    regModel->addItems(m_startAddr, m_numOfRegs, EUtils::ModbusIsWriteFunction(m_functionCode));
    //If it is a write function -> read registers
    if (!m_connected)
        return;
    else if (EUtils::ModbusIsWriteCoilsFunction(m_functionCode)){
        modbusReadData(m_slave,EUtils::ReadCoils,m_startAddr,m_numOfRegs);
        emit(refreshView());
    }
    else if (EUtils::ModbusIsWriteRegistersFunction(m_functionCode)){
        modbusReadData(m_slave,EUtils::ReadHoldRegs,m_startAddr,m_numOfRegs);
        emit(refreshView());
    }

}

void ModbusAdapter::setScanRate(int scanRate)
{
    m_scanRate = scanRate;
}

void ModbusAdapter::resetCounters()
{
    m_packets = 0;
    m_errors = 0;
    emit(refreshView());
}

int ModbusAdapter::packets()
{
    return m_packets;
}

int ModbusAdapter::errors()
{
    return m_errors;
}

void ModbusAdapter::startPollTimer()
{
    m_pollTimer->start(m_scanRate);
}

void ModbusAdapter::stopPollTimer()
{
    m_pollTimer->stop();
}

void ModbusAdapter::setTimeOut(int timeOut)
{

    m_timeOut = timeOut;

}

QString ModbusAdapter::stripIP(QString ip)
{
    //Strip zero's from IP
    QStringList ipBytes;
    QString res = "";
    int i;

    ipBytes = ip.split(".");
    if (ipBytes.size() == 4){
        res = ipBytes[0];
        i = 1;
        while (i <  ipBytes.size()){
            res = res + "." + QString("").setNum(ipBytes[i].toInt());
            i++;
        }
        return res;
    }
    else
        return "";

}

extern "C" {

void busMonitorRawResponseData(uint8_t * data, uint8_t dataLen)
{
        m_instance->busMonitorResponseData(data, dataLen);
}

void busMonitorRawRequestData(uint8_t * data, uint8_t dataLen)
{
        m_instance->busMonitorRequestData(data, dataLen);
}

}
