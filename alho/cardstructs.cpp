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

    ulong temp=timeInMinutes/60/24;
    int   ost=timeInMinutes-temp*60*24;

    dt=dt.addDays(temp);

    int hour=ost/60;
    int minute=ost-hour*60;

    QTime time(hour, minute);
    dt.setTime(time);
    return dt;
}

ulong dateTimeToTimeShit(const QDateTime& dt)
{
    QDateTime startDT(QDate (2000, 01, 01));

    if (dt<startDT) {
//qDebug () << "smaller!";
        return 0;
    }
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
                            QDataStream st(arr); st.setByteOrder(QDataStream::LittleEndian);
                            uint ret = 0;
                            st.readRawData(reinterpret_cast<char *>(&ret), 3);
                            return QVariant(ret);
                        });


    typesFactoryForRead.insert("boolarr", [](const QByteArray& arr){
                            QDataStream st(arr); st.setByteOrder(QDataStream::LittleEndian);
                            //QBitArray ret;
                            ushort val;
                            QBitArray ret(16);
                            st >> val;
                            for ( int i = 0; i<16; ++i ) {
                                ret.setBit(i, val & 0x1<<i );
                            }
                            return QVariant(ret);
                        });

    typesFactoryForRead.insert("ushort", [](const QByteArray& arr){
                            QDataStream st(arr); st.setByteOrder(QDataStream::LittleEndian);
                            ushort ret;
                            st >> ret;
                            return QVariant(ret);
                        });

    typesFactoryForRead.insert("uchar", [](const QByteArray& arr){
                            return QVariant(static_cast<uchar>(arr[0]));
                        });


    typesFactoryForRead.insert("datetimeshit", [](const QByteArray& arr) {
                            QDataStream st(arr); st.setByteOrder(QDataStream::LittleEndian);
                            uint ret = 0;
                            st.readRawData(reinterpret_cast<char *>(&ret), 3);
                            //qDebug () << "reading time_shit "<<ret<<" timeShitToDateTime(ret) "<<timeShitToDateTime(ret);
                            return QVariant( timeShitToDateTime(ret) );
                        });








    typesFactoryForWrite.insert("uint", [](const QVariant& val){
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly); st.setByteOrder(QDataStream::LittleEndian);
                            uint tmp = val.toUInt();
                            st.writeRawData(reinterpret_cast<char *>(&tmp), 3);
                            return ret;
                        });


    typesFactoryForWrite.insert("boolarr", [](const QVariant& val){
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly); st.setByteOrder(QDataStream::LittleEndian);
                            QBitArray tmp = val.toBitArray();
                            ushort save_val = 0;
                            for (int i = 0; i<16; ++i) {
                                save_val = save_val | (tmp.testBit(i)<<i);
                            }

                            st << save_val;
                            return ret;
                        });

    typesFactoryForWrite.insert("ushort", [](const QVariant& val){                                
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly); st.setByteOrder(QDataStream::LittleEndian);
                            st << static_cast<ushort>(val.toUInt());
                            return ret;
                        });

    typesFactoryForWrite.insert("uchar", [](const QVariant& val){
                            return QByteArray(1, static_cast<uchar>(val.toUInt()));
                        });

    typesFactoryForWrite.insert("datetimeshit", [](const QVariant& val) {
                            QByteArray ret;
                            QDataStream st(&ret, QIODevice::WriteOnly); st.setByteOrder(QDataStream::LittleEndian);

                            ulong tmp = dateTimeToTimeShit(val.toDateTime());

                            //qDebug () << "datetime: "<<val.toDateTime()<<" shit_date_time: "<<tmp;

                            st.writeRawData(reinterpret_cast<char *>(&tmp), 3);
                            return ret;
                        });


    return true;
}
