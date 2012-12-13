#ifndef CARDSTRUCTS_H
#define CARDSTRUCTS_H

#include <QString>
#include <QMap>
#include <QByteArray>
#include <QVariant>
#include <QDateTime>

#include <functional>

using std::function;

class FuncNotFoundException
{
public:
    FuncNotFoundException(const QString& fn) : func_name(fn) {}
    QString funcName() const {return func_name;}
private:
    QString func_name;
};

class StructMemberConf
{
public:
    QString memberName;
    QString typeName;
    uint offset;
    uint length;
    StructMemberConf(const QString& mn, const QString& tn, uint offs, uint len)
        :memberName(mn), typeName(tn), offset(offs), length(len) {}

    StructMemberConf():offset(0), length(0){}
    QVariant defaultValue() const
    {
        return getFactoryFunction(typeName, typesFactoryForDefaultValue)();
    }

    typedef QMap<QString, function<QVariant   (const QByteArray& )> > TypesFactoryForRead;
    typedef QMap<QString, function<QByteArray (const QVariant&   )> > TypesFactoryForWrite;
    typedef QMap<QString, function<QVariant   ( )                 > > TypesFactoryForDefaultValue;

    static function<QVariant   (const QByteArray& ) > functionForRead (const QString& type_name)
    {
        return getFactoryFunction(type_name, typesFactoryForRead);
    }

    static function<QByteArray (const QVariant&   ) > functionForWrite(const QString& type_name)
    {
        return getFactoryFunction(type_name, typesFactoryForWrite);
    }

    static bool registerTypes();

private:
    static TypesFactoryForRead         typesFactoryForRead;
    static TypesFactoryForWrite        typesFactoryForWrite;
    static TypesFactoryForDefaultValue typesFactoryForDefaultValue;



    template <class FuncContainer>
    static typename FuncContainer::mapped_type getFactoryFunction(const QString& type_name, const FuncContainer& cont)
    {
        auto iter = cont.find(type_name);
        if ( iter != cont.end() )
            return *iter;

        throw FuncNotFoundException(type_name);
    }
};

struct BlockConf
{
    BlockConf() :blockNum(0) {}
    BlockConf(uint bn,  uint bs):blockNum(bn),blockSize(bs) {}
    uint blockNum;
    uint blockSize;
};

//typedef QList<BlockConf> BlocksConf;
class BlocksConf : public QList<BlockConf>
{
public:
    BlocksConf() {}
    ~BlocksConf() {}

    uint memorySize() const
    {
        uint s = 0;
        for (int i = 0 ;i<count(); ++i) {
            s += this->operator [](i).blockSize;
        }
        return s;

    }

};

struct StructConf
{
    typedef QList<StructMemberConf> MembersConf;    
    MembersConf  members_conf;

    bool empty () const {return members_conf.empty();}
    const StructMemberConf& findByMember(const QString& ) const;
//    uint size() const;
};

class CardStructs
{
public:
    static const StructConf& bill_conf(const QString& class_name);
    static const BlocksConf& blocks_conf();

    static const int material_byte_address = 47;

private:
    static QMap<QString, StructConf> bill_classes;
    static BlocksConf default_blocks_conf;

    static bool initialized;
    static bool initialize();
};


#endif // CARDSTRUCTS_H
