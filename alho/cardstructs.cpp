#include "cardstructs.h"

#include <QString>
#include <QDataStream>
#include <QDateTime>
#include <QBitArray>
#include <QDebug>

StructMemberConf::TypesFactoryForRead StructMemberConf::typesFactoryForRead;
StructMemberConf::TypesFactoryForWrite StructMemberConf::typesFactoryForWrite;

bool types_registered = StructMemberConf::registerTypes();


QDateTime timeShitToDateTime(ulong timeInMinutes)
{
    QDate d(2000, 01, 01);
    QDateTime dt(d);

    unsigned long temp=timeInMinutes/60/24;
    int ost=timeInMinutes-temp*60*24;

    dt=dt.addDays(temp);

    int hour=ost/60;
    int minute=ost-hour*60;

    QTime time(hour, minute);
    dt.setTime(time);
    return dt;
}

ulong dateTimeToTimeShit(const QDateTime& dt)
{
    QDate d(2000, 01, 01);
    QDateTime startDT(d);
    if (dt<startDT) return 0;
    unsigned long temp = startDT.daysTo(dt)*24*60 + dt.time().hour()*60+dt.time().minute();
    return temp;
}

uint StructConf::size() const
{
    uint s = 0;
    for (int i = 0 ;i<blocks.count(); ++i) {
        s += blocks[i].blockSize;
    }
    return s;
}


bool StructMemberConf::registerTypes()
{
    typesFactoryForRead.insert("uint", [](const QByteArray& arr){
                            QDataStream st(arr);
                            uint ret = 0;
                            st.readRawData(reinterpret_cast<char *>(&ret), 3);
                            return QVariant(ret);
                        });


    typesFactoryForRead.insert("boolarr", [](const QByteArray& arr){
                            QDataStream st(arr);
                            QBitArray ret;
                            st >> ret;
                            return QVariant(ret);
                        });

    typesFactoryForRead.insert("ushort", [](const QByteArray& arr){
                            QDataStream st(arr);
                            ushort ret;
                            st >> ret;
                            return QVariant(ret);
                        });

    typesFactoryForRead.insert("uchar", [](const QByteArray& arr){
                            return QVariant(static_cast<uchar>(arr[0]));
                        });


    typesFactoryForRead.insert("datetimeshit", [](const QByteArray& arr) {
                            QDataStream st(arr);
                            uint ret;
                            st >> ret;
                            return QVariant( timeShitToDateTime(ret) );
                        });








    typesFactoryForWrite.insert("uint", [](const QVariant& val){
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly);
                            uint tmp = val.toUInt();
                            st.writeRawData(reinterpret_cast<char *>(&tmp), 3);
                            return ret;
                        });


    typesFactoryForWrite.insert("boolarr", [](const QVariant& val){
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly);
                            QBitArray tmp = val.toBitArray();
                            ushort save_val = 0;
                            for (int i = 0; i<16; ++i) {
                                save_val &= (tmp.testBit(i)<<i);
                            }
                            st << save_val;
                            return ret;
                        });

    typesFactoryForWrite.insert("ushort", [](const QVariant& val){
                                //qDebug () << "ushort: "<<val;
                                QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly);
                            st << static_cast<ushort>(val.toUInt());
                            return ret;
                        });

    typesFactoryForWrite.insert("uchar", [](const QVariant& val){
                            return QByteArray(1, static_cast<uchar>(val.toUInt()));
                        });

    typesFactoryForWrite.insert("datetimeshit", [](const QVariant& val) {
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly);
                            ulong tmp = dateTimeToTimeShit(val.toDateTime());
                            st.writeRawData(reinterpret_cast<char *>(&tmp), 3);
                            return ret;
                        });


    return true;
}
