#include "cardstructs.h"

#include <QString>
#include <QDataStream>
#include <QDateTime>
#include <QBitArray>
#include <QDebug>

#include "func.h"

StructMemberConf::TypesFactoryForRead StructMemberConf::typesFactoryForRead;
StructMemberConf::TypesFactoryForWrite StructMemberConf::typesFactoryForWrite;
StructMemberConf::TypesFactoryForDefaultValue StructMemberConf::typesFactoryForDefaultValue;

bool types_registered = StructMemberConf::registerTypes();

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
                            //qDebug() << "size: " << tmp.size();
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

                            st.writeRawData(reinterpret_cast<char *>(&tmp), 3);
                            return ret;
                        });





    typesFactoryForDefaultValue.insert("uint", [](){
                            return QVariant(0u);
                        });


    typesFactoryForDefaultValue.insert("boolarr", [](){
                            return QVariant(QBitArray(16));
                        });

    typesFactoryForDefaultValue.insert("ushort", [](){
                            return QVariant( static_cast<ushort> (0));
                        });

    typesFactoryForDefaultValue.insert("uchar", [](){
                            return QVariant( static_cast<uchar> (0));
                        });

    typesFactoryForDefaultValue.insert("datetimeshit", []() {
                            return QVariant(timeShitToDateTime(0));
                        });



    return true;
}





const StructMemberConf& StructConf::findByMember(const QString & member_name) const
{
    for ( auto iter = members_conf.begin(); iter != members_conf.end(); ++iter) {
        if (iter->memberName == member_name) return *iter;
    }

    qWarning() << "SructConf::findByMember cant find member with name: " << member_name;
    qFatal("Exiting");
    return *members_conf.end();
}
