#ifndef MIFAREREADERLIST_H
#define MIFAREREADERLIST_H
#include <QList>
#include "mifarereader.h"
class MifareReaderList: public PorterDriver
{
     Q_OBJECT
public:



    MifareReaderList(const MifareReaderList& ) = delete;
    ~MifareReaderList();

    static PorterDriver * create(const QVariantMap& conf )
    {
        return new MifareReaderList(conf);
    }

    Q_INVOKABLE QVariant doOn();
    Q_INVOKABLE QVariant doOff();
    Q_INVOKABLE QVariant doSound( const QVariant& );
    Q_INVOKABLE QVariant activateIdleA();
    Q_INVOKABLE QVariant getHostCodedKey(const QVariant& );
    Q_INVOKABLE QVariant doAuth( const QVariant& );
    Q_INVOKABLE QVariant readBlock( const QVariant& );
    Q_INVOKABLE QVariant writeBlock( const QVariant&, const QVariant&  );



protected:
    MifareReaderList(const QVariantMap& );
private:
    static BossnFactoryRegistrator<MifareReaderList>  registator;
    QList <MifareReader::Pointer> readers;
    int activeReader;
};

#endif // MIFAREREADERLIST_H
