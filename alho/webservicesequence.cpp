#include "soapTestSoapBindingProxy.h"
#include "TestSoapBinding.nsmap"
#include "webservicesequence.h"

#include "asyncfuncbase.h"
#include "settingstool.h"

#include "warnmessages.h"
#include "mifarereader.h"
#include "mifarecard.h"

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

#include <sstream>
#include <iosfwd>
#include <ios>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/categories.hpp>


namespace io = boost::iostreams;

using std::unique_ptr;

int fake_connect(struct soap*, const char*, const char*, int)
{
    return 0;
}

int fake_disconnect(struct soap*)
{
    return 0;
}




class GsoapSource
{
public:


    GsoapSource( Coroutine2& c):coro_(c){}

    ~GsoapSource()
    {
    }

    GsoapSource( const GsoapSource& ) = delete;

    std::streamsize read(char* s, std::streamsize n)
    {
        if ( !buffer_initialized ) {
            buffer_initialized  = true;
            terminate_          = false;
            try {
                SocketHelper sh(*this);
                sh.exechange();
                if ( terminate_ ) {
                    return -1;
                }
            }
            catch (MainSequenceException & ex)
            {
                current_exception = ex;
                has_exception = true;
                return -1;
            }
        }                

        using namespace std;
        streamsize amt = static_cast<streamsize>(read_buffer.size() - pos_);
        streamsize result = (min)(n, amt);
        if (result != 0) {
            std::copy( read_buffer.begin() + pos_,
                       read_buffer.begin() + pos_ + result,
                     s );
            pos_ += result;
            return result;
        }
        else {
            return -1; // EOF
        }
    }

    std::streamsize write(const char* s, std::streamsize n)
    {
        string str( s, n );

        write_buffer += str;

        return n;
    }

    const std::string& writeBuffer() const {return write_buffer;}
    void setReadBuffer( const std::string& rb )
    {
        read_buffer = rb;
    }
    Coroutine2& coro() {return coro_;}
    const Coroutine2& coro() const {return coro_;}
    void exception()
    {
        if (has_exception) {
            has_exception = false;
            throw current_exception;
        }
    }

    void terminate()
    {
        terminate_ = true;  //need manual reset of terminate_
        coro_.cont();
    }

    bool isTerminating() const
    {
        return terminate_;
    }

private:
    Coroutine2 & coro_;
    std::string read_buffer;

    string write_buffer;
    string::size_type pos_ = 0;
    bool buffer_initialized = false;
    MainSequenceException current_exception;
    bool has_exception = false;
    bool terminate_ = false;
};



class FakeSource
{
public:
    using char_type = char;
    using category  = io::bidirectional_device_tag;

    FakeSource( Coroutine2 & c ):source_(new GsoapSource(c))
    {

    }
    ~FakeSource(){}

    std::streamsize read(char* s, std::streamsize n)
    {
        return source_->read(s, n);
    }

    std::streamsize write(const char* s, std::streamsize n)
    {
        return source_->write(s, n);
    }

    const std::string& writeBuffer() const
    {
        return source_->writeBuffer();
    }

    void setReadBuffer( const std::string& rb )
    {
        source_->setReadBuffer(rb);
    }
    Coroutine2& coro()
    {
        return source_->coro();
    }

    const Coroutine2& coro() const
    {
        return source_->coro();
    }

    void exception()
    {
        source_->exception();
    }

    void terminate()
    {
        source_->terminate();
    }

    bool isTerminating( ) const
    {
        return source_->isTerminating();
    }
private:
    std::shared_ptr<GsoapSource> source_;
};


SocketHelper::SocketHelper(GsoapSource & s):source_(s)
{
}

SocketHelper::~SocketHelper()
{

}

unique_ptr<QTcpSocket> SocketHelper::getSocket() const
{
    unique_ptr<QTcpSocket> s(new QTcpSocket);

    connect( s.get(), SIGNAL(connected()) , this, SLOT(onConnected()));

    connect( s.get(), SIGNAL(error(QAbstractSocket::SocketError)),
             this, SLOT(onError(QAbstractSocket::SocketError)));
    connect( s.get(), SIGNAL(readyRead()) , this, SLOT(onReadyRead()));

    return s;
}

unique_ptr<QTimer> SocketHelper::getTimer() const
{
    unique_ptr<QTimer> t(new QTimer);

    t->setSingleShot(true);

    connect( t.get(), SIGNAL(timeout()), this, SLOT(onTimeout()) );

    return t;
}


void SocketHelper::exechange()
{
    unique_ptr<QTcpSocket> socket_   = getSocket();
    unique_ptr<QTimer> timeout_timer = getTimer();

    error = timeout = got_result = false;

    socket_->connectToHost("192.168.0.66", 80);
    timeout_timer->start(5000);

    if ( socket_->state() != QTcpSocket::ConnectedState ) {
        while ( !got_result ) {
            source_.coro().yield();
            if ( source_.isTerminating() )
                return;
        }
        got_result = false;
    }

    if (  error ) {
        throw MainSequenceException( connect_to_service_server_error, "connection to host " + socket_->peerAddress().toString() +
                                     " error!", socket_->errorString() );
    }

    if (  timeout )
        throw MainSequenceException( connect_to_service_server_timeout, "connection to host " + socket_->peerAddress().toString() +
                                     " timeout!", "" );



    socket_->write( source_.writeBuffer().c_str() );

    timeout_timer->start(5000);

    while ( !got_result ) {
        source_.coro().yield();
        qDebug() << "11111111111";
        if ( source_.isTerminating() ) {
            qDebug() << "terminating!!!";
            return;
        }
    }
    got_result = false;


    if (  error ) {
        throw MainSequenceException( read_from_service_server_error, "read from host " + socket_->peerAddress().toString() +
                                     " error!", socket_->errorString() );
    }

    if (  timeout ) {
        throw MainSequenceException( read_from_service_server_timeout, "read from host " + socket_->peerAddress().toString() +
                                     " timeout!", "" );
    }


    QByteArray arr = socket_->readAll();

    std::string s(arr.data(), arr.size());

    source_.setReadBuffer( s );
    timeout_timer->stop();
}

void SocketHelper::onConnected()
{
    got_result = true;
    source_.coro().cont();
}

void SocketHelper::onError(QAbstractSocket::SocketError e)
{
    qWarning() << "socket error: " << e;
    error = got_result = true;
    if ( source_.coro().status() != Coroutine2::Stopped ) {
        qWarning() << "Surious warning! Got error in socket helper while coro is not in stopped state!!";
        return;
    }
    source_.coro().cont();
}

void SocketHelper::onReadyRead()
{   
    got_result = true;
    if ( source_.coro().status() != Coroutine2::Stopped ) {
        qWarning() << "Surious error! Got readyRead in socket helper while coro is not in stopped state!!";
        return;
    }

    source_.coro().cont();
    qDebug() << "exiting from onReadyRead!!!";
}

void SocketHelper::onTimeout()
{
    qWarning() << "socket timeout!";

    got_result = timeout = true;

    if ( source_.coro().status() != Coroutine2::Stopped ) {
        qWarning() << "Surious error! Got readyRead in socket helper while coro is not in stopped state!!";
        return;
    }


    source_.coro().cont();
}






class WebServiceAsync
{
public:
    WebServiceAsync(Coroutine2& c):coro_(c)
    {

    }

    ~WebServiceAsync()
    {

    }

    QMap<QString, QString> exchangeData(const QString& s)
    {
        std::shared_ptr<WebServiceAsync> cur_fake_source_guard( this , [&](WebServiceAsync * wsa){
            wsa->cur_fake_source = nullptr;
        } ); Q_UNUSED(cur_fake_source_guard);

        terminating_ = false;

        Q_ASSERT( !cur_fake_source );

        TestSoapBindingProxy proxy;
        //proxy.soap->mode = SOAP_IO_STORE;
        proxy.soap->fconnect    = fake_connect;
        proxy.soap->fopen       = nullptr;
        proxy.soap->fdisconnect = fake_disconnect;
        proxy.soap->fclose       = nullptr;


        io::stream <FakeSource> is;

        FakeSource source(coro_);
        cur_fake_source = &source;


        proxy.soap->os = &is;
        proxy.soap->is = &is;        

        is.open( source, 0 );

        _ns1__Hello hello;
        _ns1__HelloResponse resp;


        hello.param = s.toStdString();

        int ret = proxy.Hello( &hello, &resp );

        if ( source.isTerminating() ) {
            //is.close();
            return QMap<QString, QString>();
        }

        if ( ret != SOAP_OK ) {
            source.exception();

            throw MainSequenceException( gsoap_data_exchange_request_error, "Error in request data: " +
                                         QString::number(ret), "");
        }
        else {
            std::cout << "ret: " << resp.return_  << std::endl;
        }

        //is.close();

        QMap<QString, QString> retm;
        retm["aaa"] = QString::fromStdString( resp.return_ );

        return retm;
    }

    void acceptedCardResult( bool )
    {

    }

    void terminate()
    {
        terminating_ = true;
        cur_fake_source->terminate();
    }

    bool isTerminating() const
    {
        if (!cur_fake_source)
            return terminating_;

        return cur_fake_source->isTerminating(); //ugly hack
    }

private:
    Coroutine2 & coro_;
    FakeSource * cur_fake_source = nullptr;
    bool terminating_ = false;
};




WebServiceSequence::WebServiceSequence(Tags & t, const QVariantMap& m) : MainSequenceBaseOp("WebServiceSequence", t, m)
{

}

WebServiceSequence::~WebServiceSequence()
{

}

void WebServiceSequence::setSettings(const QVariantMap & s)
{
    alho_settings.init(s);

    seq_id                              = get_setting<int>("id", s);
//#if 0
//    current_card_tag = get_setting<QString>("current_card_tag", s);
//    current_card_prop = get_setting<QString>("current_card_prop", s);
//#endif

    setObjectName( "MainSequence num: " + QString::number(seq_id) );

    restart();

    cont();  //for initializing tablo and do
}

void WebServiceSequence::run()
{
    if ( init ) {
        printOnTablo(tr(greeting_message));
        setLightsToGreen();
        init = false;
        return;
    }

    seqDebug() << "something appeared on weight!!!! id" << seq_id;

    on_weight = true;

    printOnTablo(tr(apply_card_message));

    setLightsToRed();

    alho_settings.reader.do_on.func();

    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    std::shared_ptr<WebServiceSequence> current_card_guard( this, [this](WebServiceSequence*){
        alho_settings.current_card.setProperty(QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    }); Q_UNUSED(current_card_guard);


    while(on_weight) {
        ActivateCardISO14443A act = alho_settings.reader.activate_idle.func().value<ActivateCardISO14443A>();
        MifareCardSector card(act, alho_settings.reader, card_code, data_block);

        if ( !card.active() ) {
            sleepnbtm();
            continue;
        }

        try {
            qDebug() << "1";
            std::shared_ptr<WebServiceSequence> cur_fake_source_guard( this , [&](WebServiceSequence * wss){
                wss->cur_webservice_async = nullptr;
            } ); Q_UNUSED(cur_fake_source_guard);

            Q_ASSERT(!cur_webservice_async);

            printOnTablo(tr(processing_message));
            qDebug() << "2";
            card.autorize();            

            checkForStealedCard( act );
            qDebug() << "3";
            WebServiceAsync was(*this);
            cur_webservice_async = &was;
            qDebug() << "4";

            QMap<QString, QString> ret = was.exchangeData( mapToString( getSimpleTagsValues(  ) ) + ",\n" + getReaderBytes(card) );
            qDebug() << "5";

            if (cur_webservice_async->isTerminating()) {
                continue;
            }
            qDebug() << "6";
            sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
            printOnTablo( tr(apply_card_message) );                                  
            qDebug() << "7";
            continue;
        }
        catch (MifareCardAuthException& ex) {
            seqWarning() << "auth_exeption! "<<ex.message();

            sleepnbtmerr(tr(card_autorize_error_message), tr(apply_card_message));
            continue;
        }
        catch (MifareCardReadException& ex) {
            seqWarning() << "read_card_exception! "<<ex.message();
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardWriteException& ex) {
            seqWarning() << "write_card_exception! "<<ex.message();
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }
        catch (MainSequenceException& ex) {
            seqWarning()<<"sequence_exception: " << ex.adminMessage() << " sys: " + ex.systemMessage();
            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardException& ex) {
            seqWarning()<<"mifare_card_exception: " << ex.message();
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }

    }
    seqDebug () << "\n\nexit from onAppearOnWeight!!!!!!!";

    printOnTablo(tr(greeting_message));
    setLightsToGreen();
    //tags[current_card_tag]->setProperty(current_card_prop,
    //                                   QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    alho_settings.reader.do_off.func();


}


void WebServiceSequence::onAppearOnWeight(const QString&, AlhoSequence*)
{
    if ( status() == NotStarted || status() == Terminated ) {
        restart();
        cont();
    }
}

void WebServiceSequence::onDisappearOnWeight(const QString&, AlhoSequence*)
{
    seqDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    if ( wake_timer.isActive() ) {
        wake_timer.stop();

        if (  status() == Stopped  )
            wakeUp();
    }

    if (cur_webservice_async) {
        cur_webservice_async->terminate();
    }

    //qDebug( )     << "3";

    qDebug() << "disappear finished!!!";

}


void WebServiceSequence::wakeUp()
{
    qDebug() << "waked up!!!!!";
    cont();
}



QMap<QString, QString> WebServiceSequence::getSimpleTagsValues()
{
    QMap<QString, QString> ret;

    ret[alho_settings.perim_in.tag_name]      = QString::number( alho_settings.perim_in.func().toBool() );
    ret[alho_settings.perim_out.tag_name]     = QString::number( alho_settings.perim_out.func().toBool() );
    ret[alho_settings.weight_tag.tag_name]    = QString::number( alho_settings.weight_tag.func().toInt() );
    ret[alho_settings.weight_stable.tag_name] = QString::number( alho_settings.weight_stable.func().toBool() );

    return ret;
}

QString WebServiceSequence::getReaderBytes( MifareCardSector&  card)
{
    QByteArray card_bytes = card.readByteArray( CardStructs::blocks_conf() );
    QString delim;
    QString ret = alho_settings.reader.name + ":";

    for ( int i = 0; i<card_bytes.count(); ++i ) {
        ret += delim + QString::number(static_cast<uchar>(card_bytes[i]), 16);
        delim = " ";
    }

    return ret;
}
