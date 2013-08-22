#ifndef WEBSERVICESEQUENCE_H
#define WEBSERVICESEQUENCE_H

#include "alhosequence.h"
#include "mainsequencesettings.h"
#include "tags.h"
#include "mainsequencebaseop.h"


#include <QTcpSocket>
#include <QTimer>
#include <QStringList>
#include <QCoreApplication>

#include <memory>

class MifareCardSector;

class GsoapSource;

class BlockDataImp;

class BlockData
{
public:
    BlockData(){}
    BlockData(int bn, const QByteArray& ar) :data_(ar), block_num(bn)
    {}

    void setData( const QByteArray& a ) { data_ = a; }
    QByteArray data() const { return data_; }

    virtual QString toString () const = 0;
    virtual void fromString (const QString& ) = 0;
    int blockSize(  ) const { return data_.size(); }
    void setBlockNum( int bn ) { block_num = bn; }
    int blockNum(  ) const { return block_num; }

    static const int block_size = 16;
protected:
    QByteArray data_;


    const int blocks_count = 8;

    void checkData(int) const;
private:    
    int block_num = -1;
};



class DefaultedBlockData : public BlockData
{
public:
    DefaultedBlockData() : BlockData(){}
    DefaultedBlockData(int n, const QByteArray& arr) : BlockData(n, arr){}

    virtual QString toString () const;
    virtual void fromString (const QString& );
};


class NumberedBlockData : public BlockData
{
public:
    NumberedBlockData() : BlockData(){}
    NumberedBlockData(int n, const QByteArray& arr) : BlockData(n, arr){}

    virtual QString toString () const;
    virtual void fromString (const QString& );
private:

};


class BlocksData
{
public:
    virtual ~BlocksData() {}

    virtual void append( int , const QByteArray& ) = 0;
    virtual QString toString( ) const = 0;
    virtual void fromString(const QString& ) = 0;

    auto begin() {return data_list.begin();}
    auto begin() const{return data_list.begin();}

    auto end() {return data_list.end();}
    auto end() const {return data_list.end();}
protected:
    std::vector< std::unique_ptr<BlockData> > data_list;
private:

};


class DefaultedBlocksData : public BlocksData
{
public:
    virtual QString toString( ) const override;
    virtual void fromString(const QString& ) override;
    virtual void append( int bn, const QByteArray& ) override;

    static QVariantList blockConf();
private:
    const int bytes_num = 48;

    static const int block_num1 = 128;
    static const int block_num2 = 129;
    static const int block_num3 = 130;

    QString blockByteString(int block_num, const QString&) const;
    int whitespacePos( int num, const QString& s ) const;
};


class NumberedBlocksData : public BlocksData
{
public:
    virtual QString toString( ) const override;
    virtual void fromString(const QString& ) override;
    virtual void append( int , const QByteArray& ) override;
};


class SocketHelper : public QObject
{
    Q_OBJECT
public:
    SocketHelper(GsoapSource & );
    ~SocketHelper();
    void exechange(const QString& ip, int);
private:
    GsoapSource & source_;
    //QTcpSocket socket_;
    //QTimer timeout_timer;
private slots:
    void onConnected();
    //void onDisconnected();
    void onError(QAbstractSocket::SocketError);
    void onReadyRead();
    void onTimeout();
private:
    volatile bool error = false;
    volatile bool timeout = false;
    volatile bool got_result = false;

    auto getSocket() const
    {
        auto del_func = [](QTcpSocket * s){
            s->deleteLater();
        };

        std::unique_ptr<QTcpSocket, decltype(del_func)> s(new QTcpSocket, del_func);

        connect( s.get(), SIGNAL(connected()) , this, SLOT(onConnected()));

        connect( s.get(), SIGNAL(error(QAbstractSocket::SocketError)),
                 this, SLOT(onError(QAbstractSocket::SocketError)));
        connect( s.get(), SIGNAL(readyRead()) , this, SLOT(onReadyRead()));

        return s;
    }

    auto getTimer() const
    {
        auto del_func = [](QTimer * t){
            t->stop();
            t->deleteLater();
        };

        std::unique_ptr<QTimer, decltype(del_func)> t(new QTimer, del_func);

        t->setSingleShot(true);

        connect( t.get(), SIGNAL(timeout()), this, SLOT(onTimeout()) );

        return t;
    }
};

class WebServiceAcyncDeinitializer;
class WebServiceAsync;

enum class CardReaderWebProtocol{Defaulted, Numbered};

class WebServiceSequence : public MainSequenceBaseOp
{
    friend class WebServiceAcyncDeinitializer;
    Q_OBJECT
public:
    WebServiceSequence( Tags& t, const QVariantMap& m );
    ~WebServiceSequence();

    virtual void setSettings(const QVariantMap &) override;

    Q_INVOKABLE void onAppearOnWeight(const QString&, AlhoSequence*);
    Q_INVOKABLE void onDisappearOnWeight(const QString&, AlhoSequence*);

protected:
    virtual void run () override;
protected slots:
    virtual  void wakeUp() override;
private:
    bool on_weight  = false;
    bool init       = true;

    QString ip_;
    int port_ = 0;
    QString userid_;
    QString passwd_;

    QMap<QString, QString> getSimpleTagsValues() ;
    QString getReaderBytes( MifareCardSector&  );
    QString mapToString( const QMap<QString, QString>& m ) const
    {
        QString ret; QString delim;
        for ( QMap<QString, QString>::const_iterator iter = m.begin(); iter != m.end(); ++iter ) {
            ret += ( delim + iter.key() + ":" + iter.value() ) ;
            delim = ",\n";
        }
        return ret;
    }

    QMap<QString, QString> stringToMap( const QString& m ) const
    {
        QMap<QString, QString> ret;

        QStringList l1 = m.split(",\n");

        for ( const QString& v : l1  ) {
            QStringList l2 = v.split(":");
            ret[ l2.first().trimmed() ] = l2.last().trimmed();
        }

        return ret;
    }
    void writeTagsValues( const QMap<QString, QString>& , MifareCardSector&   );
    void writeReaderBytes( const QString&, MifareCardSector&  card );

    WebServiceAsync * cur_webservice_async = nullptr;

    static QString tr2(const char * s)
    {
        return QCoreApplication::translate("MainSequence", s);
    }

    BlocksConf read_blocks_conf;
    CardReaderWebProtocol cardreader_web_protocol = CardReaderWebProtocol::Defaulted;
};


#endif // WEBSERVICESEQUENCE_H
