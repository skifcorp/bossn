#ifndef MIFAREREADEREMULATOR_H
#define MIFAREREADEREMULATOR_H

#include <QByteArray>


#include "porterdriver.h"
#include "sharedfromthis.h"
#include "mifarecarddata.h"


class MifareReaderEmulator : public PorterDriver
{
    Q_OBJECT
public:
    MifareReaderEmulator(const MifareReaderEmulator& ) = delete;
    ~MifareReaderEmulator();

    static PorterDriver * create(const QVariantMap& conf )
    {
        return new MifareReaderEmulator(conf);
    }

    Q_INVOKABLE QVariant doOn();
    Q_INVOKABLE QVariant doOff();
    Q_INVOKABLE QVariant doSound( const QVariant& );
    Q_INVOKABLE QVariant activateIdleA();
    Q_INVOKABLE QVariant getHostCodedKey( const QVariant& );
    Q_INVOKABLE QVariant doAuth( const QVariant& );
    Q_INVOKABLE QVariant readBlock( const QVariant& );
    Q_INVOKABLE QVariant writeBlock( const QVariant&, const QVariant& );

protected:
    MifareReaderEmulator(const QVariantMap& );
private:
    static BossnFactoryRegistrator<MifareReaderEmulator>  registator;

    uchar address;
    static uchar frame_ident;


    friend class MifareCard;
    bool waitForAnswer();

    static QString errorMessage(uchar e);
    QString card_class_name;


    struct current_block_offsets
    {
        uint start_offset;
        uint finish_offset;
        current_block_offsets () : start_offset(0), finish_offset(0) {}
    };

    current_block_offsets getCurrentBlockOffsets(int num) const;
    MifareCardData defaultDataForBlock(int) const;
    QByteArray cardDataToByteArray( const MifareCardData&, int ) const;
    void checkBlockNum(int) const;
    int blockIndexByNum(uint) const;
};







#endif // MIFAREREADEREMULATOR_H
