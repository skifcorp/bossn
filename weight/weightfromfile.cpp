#include "weightfromfile.h"
#include <QByteArray>
#include <QString>
#include <QFile>

BossnFactoryRegistrator<WeightFromFile> WeightFromFile::registrator("WeightFromFile");

void WeightFromFile::readWeight(float & ret, uint & err)
{
    QByteArray ba = io_device()->readAll();

    ret = parseWeightFrameAnswer(ba, err);
}

void WeightFromFile::zero(uint &)
{

}

QByteArray WeightFromFile::weightRequestFrame() const
{
    return QByteArray( );
}
float WeightFromFile::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    bool ok = false;
    float fret = ba.toFloat(&ok);
    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return NAN;
    }
    err = 0;
    return fret;
}
/*
float WeightFromFile::getWeight()
{
	QFile weightFile;
	weightFile.setFileName("weight.conf");
	weightFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray ves=weightFile.readAll();
	weightFile.close();

	return ves.toFloat();
}*/
