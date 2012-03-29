#ifndef DIDOFILE_H
#define DIDOFILE_H

#include "porterdriver.h"

class DidoFile : public PorterDriver
{
    Q_OBJECT
public:
    typedef QSharedPointer<DidoFile> Pointer;

    ~DidoFile() {}

    Q_INVOKABLE QVariant getDi();
    Q_INVOKABLE void setDo(const QVariant& );

    Q_INVOKABLE QVariant getDiBit(const QVariant& full_byte, const QVariant& num);
    Q_INVOKABLE QVariant setDoBit(const QVariant& full_byte, const QVariant& num, const QVariant& );

    static PorterDriver* create(const QMap<QString, QVariant>& )
    {
        return new DidoFile();
    }
protected:
    DidoFile()
    {

    }

private:
    static BossnFactoryRegistrator<DidoFile> registrator;
    uchar readAll( );

};

#endif // DIDOFILE_H
