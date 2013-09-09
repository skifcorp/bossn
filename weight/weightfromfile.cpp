#include "weightfromfile.h"
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QTime>

BossnFactoryRegistrator<WeightFromFile> WeightFromFile::registrator("WeightFromFile");

int WeightFromFile::calls_counter = 0;

void WeightFromFile::readWeight(QVariant & ret, uint & err)
{
    //qDebug() << "readWeight started!!! " << calls_counter++;
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), io_device_.data(), SLOT(emulateReadyRead()) );
    timer.start(900);
    yield();

    QByteArray ba = io_device()->readAll();
    //qDebug() << "readWeight before y";

    //qDebug() << "readWeight after  y";

    ret = parseWeightFrameAnswer(ba, err);

    //qDebug() << "readWeight exit!!!" << calls_counter--;
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
    bool ok = true;
    static int fret = ba.toInt(&ok);

    if (!ok) {        
        err = PorterFrameCorrupted; return 0;
    }

    err = 0;
    return fret++;
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
