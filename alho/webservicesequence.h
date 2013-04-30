#ifndef WEBSERVICESEQUENCE_H
#define WEBSERVICESEQUENCE_H

#include "alhosequence.h"
#include "mainsequencesettings.h"
#include "tags.h"
#include "mainsequencebaseop.h"


#include <QTcpSocket>
#include <QTimer>

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

    std::unique_ptr<QTcpSocket> getSocket() const;
    std::unique_ptr<QTimer> getTimer() const;
};

class WebServiceSequence : public MainSequenceBaseOp
{
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
};


#endif // WEBSERVICESEQUENCE_H
