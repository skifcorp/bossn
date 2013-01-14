#include "weightfromfile.h"
#include <QByteArray>
#include <QString>
#include <QFile>

BossnFactoryRegistrator<WeightFromFile> WeightFromFile::registrator("WeightFromFile");

void WeightFromFile::readWeight(QVariant & ret, uint & err)
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
int WeightFromFile::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    bool ok = false;
    int fret = ba.toInt(&ok);
    if (!ok) {        
        err = PorterFrameCorrupted; return 0;
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
