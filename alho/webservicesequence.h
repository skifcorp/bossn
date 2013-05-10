#ifndef WEBSERVICESEQUENCE_H
#define WEBSERVICESEQUENCE_H

#include "alhosequence.h"
#include "mainsequencesettings.h"
#include "tags.h"
#include "mainsequencebaseop.h"


#include <QTcpSocket>
#include <QTimer>
#include <QStringList>

#include <memory>

class MifareCardSector;

class GsoapSource;


class SocketHelper : public QObject
{
    Q_OBJECT
public:
    SocketHelper(GsoapSource & );
    ~SocketHelper();
    void exechange();
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
    int seq_id      = 0;

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

        QStringList l1 = m.split(";\n");

        for ( QString v : l1  ) {
            QStringList l2 = v.split(":");
            ret[ l2.first() ] = l2.last();
        }
        return ret;
    }


    WebServiceAsync * cur_webservice_async = nullptr;
};


#endif // WEBSERVICESEQUENCE_H
