#include "eutils.h"

EUtils::EUtils()
{
}

QString EUtils::formatValue(int value,int frmt, bool is16Bit)
{
    QString convertedValue;

    switch(frmt){

        case 2://Binary
        if (is16Bit)
            convertedValue = QString("%1").arg(value,16,2,QLatin1Char('0')).toUpper();
        else
            convertedValue = QString("%1").arg(value,0,2).toUpper();
        break;

        case 10://Decimal - Unsigned Integer
            convertedValue = QString("%1").arg((unsigned short)value,0,10).toUpper();
        break;

        case 16://Hex
        if (is16Bit)
            convertedValue = QString("%1").arg((unsigned short)value,4,16,QLatin1Char('0')).toUpper();
        else
            convertedValue = QString("%1").arg(value,0,16).toUpper();
        break;

        default://Default
        convertedValue = QString("%1").arg(value,0,10).toUpper();

    }

    return convertedValue;

}

