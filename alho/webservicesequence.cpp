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

#include <sstream>
#include <iosfwd>


#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/categories.hpp>


namespace io = boost::iostreams;

struct AAA : public io::flushable_tag, io::bidirectional_device_tag{};

class GsoapSource
{
public:
    typedef char            char_type;
    typedef AAA category;
    GsoapSource(string& ){}

    bool flush()
    {
        return true;
    }

    std::streamsize read(char* s, std::streamsize n)
    {
        SocketHelper sh(*this);

        sh.exechange();

        //qDebug() << "RRRRRRRRRRRRRRRRRRRRRRRRRRREEEEEEEEEEEEEEEEEED";

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
        //qDebug() << "WRIIIIIIIIIIIIIIIIII";

        write_buffer += s;

        //std::cout << "---------------------write_buffer: " << write_buffer<<std::endl;

        return n;
    }

    const std::string& writeBuffer() const {return write_buffer;}
    void setReadBuffer( const std::string& rb )
    {
        read_buffer = rb;
    }

private:
    std::string read_buffer
#if 0
    = "HTTP/1.1 200 OK\n"                          \
            "Date: Fri, 26 Apr 2013 12:06:18 GMT\n"                   \
            "Server: Apache/2.2.4 (Win32)\n"                          \
            "Content-Length: 445\n"                                   \
            "Set-Cookie: vrs_rc=;Version=1\n"                         \
            "Connection: close\n"                                     \
            "Content-Type: text/xml; charset=utf-8\n\n"                 \
            "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\">\n"    \
                "<soap:Header/>\n"                                                        \
                "<soap:Body> <m:HelloResponse xmlns:m=\"http://localhost\">\n"            \
                "<m:return xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"\n"               \
                        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">reader2:1 0 0 0 0 69 0 0 0 0 0 0 0 8e 8 0 10 27 84 f9 68 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0,\n"   \
            "tablo2:Hello!!!!!!!!!!!!!</m:return>\n"                  \
            "</m:HelloResponse></soap:Body>\n"            \
            "</soap:Envelope>\n"
#endif
     ;

    string write_buffer;
    string::size_type pos_ = 0;
};




SocketHelper::SocketHelper(GsoapSource & s):source_(s)
{
//    connect( &socket_, SIGNAL(connected()) , this, SLOT(onConnected());
//    connect( &socket_, SIGNAL(disconnected()) , this, SLOT(onDisconnected());
//    connect( &socket_, SIGNAL(error(QAbstractSocket::SocketError)) , this, SLOT(onError(QAbstractSocket::SocketError)));
//    connect( &socket_, SIGNAL(readyRead()) , this, SLOT(onReadyRead()));
}

SocketHelper::~SocketHelper()
{

}

void SocketHelper::exechange()
{
    socket_.connectToHost("127.0.0.1", 80);
    socket_.waitForConnected();
    socket_.write( source_.writeBuffer().c_str() );

    //std::cout << "wrote:  "<< source_.writeBuffer();

    socket_.waitForBytesWritten();
    if (socket_.waitForReadyRead()) {
        QByteArray arr = socket_.readAll();
        std::string s(arr.data(), arr.size());
        source_.setReadBuffer( s );
    }
}


void SocketHelper::onConnected()
{

}

void SocketHelper::onDisconnected()
{

}

void SocketHelper::onError(QAbstractSocket::SocketError)
{

}

void SocketHelper::onReadyRead()
{

}

void SocketHelper::onTimeout()
{

}




class WebServiceAsync : public async_func_base2
{
public:
    WebServiceAsync(Coroutine2& c) : async_func_base2(c)
    {

    }

    ~WebServiceAsync()
    {

    }

    QMap<QString, QString> exchangeData(const QString& s)
    {
        return async_exec([&s]{
            TestSoapBindingProxy proxy;
            //proxy.soap->mode = SOAP_IO_STORE;
            //proxy.soap->recv_timeout = 1;
            //proxy.soap->connect_timeout = 1;

            std::string buffer = "HTTP/1.1 200 OK\n"                          \
                    "Date: Fri, 26 Apr 2013 12:06:18 GMT\n"                   \
                    "Server: Apache/2.2.4 (Win32)\n"                          \
                    "Content-Length: 445\n"                                   \
                    "Set-Cookie: vrs_rc=;Version=1\n"                         \
                    "Connection: close\n"                                     \
                    "Content-Type: text/xml; charset=utf-8\n\n"                 \
                    "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\">\n"    \
                        "<soap:Header/>\n"                                                        \
                        "<soap:Body> <m:HelloResponse xmlns:m=\"http://localhost\">\n"            \
                        "<m:return xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"\n"               \
                                "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">reader2:1 0 0 0 0 69 0 0 0 0 0 0 0 8e 8 0 10 27 84 f9 68 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0,\n"   \
                    "tablo2:Hello!!!!!!!!!!!!!</m:return>\n"                  \
                    "</m:HelloResponse></soap:Body>\n"            \
                    "</soap:Envelope>\n";



            std::ostringstream os;
            //std::istringstream is(buffer);
            //is.rdbuf(BossnRdBuf());
            //std::istream is;

            io::stream <GsoapSource> is(buffer);

            //is.
            //is.open();

            proxy.soap->os = &is;
            proxy.soap->is = &is;

            _ns1__Hello hello;
            _ns1__HelloResponse resp;


            hello.param = s.toStdString();

            int ret = proxy.Hello( &hello, &resp );
            if ( ret != SOAP_OK ) {
                std::cerr << "!!!!!!!!!!!!!!!!!!!!!error: " << ret << std::endl;

            }
            else {
                std::cout << "!!!!!!!!!!!!!!!!!!!ret: " << resp.return_  << std::endl;
            }

            //ret = proxy.Hello( &hello, &resp );
            //if ( ret != SOAP_OK ) {
            //    qWarning() << "!!!!!!!!!!!!!!!!!!!!!!!!soap FAILED: " << ret;
//
            //}
            //else {
                //qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!! ret: " << QString::fromStdString( resp.return_ );
            //}




            //std::cout << "is: " << is.str()  << "\n----------->os " << os.str()
            //          << std::endl;

            QMap<QString, QString> retm;
            retm["aaa"] = QString::fromStdString( resp.return_ );

            return retm;
        }, "Exchange web service data", "Exchange web service data" );
    }

    void acceptedCardResult( bool )
    {

    }
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
#if 0
    current_card_tag = get_setting<QString>("current_card_tag", s);
    current_card_prop = get_setting<QString>("current_card_prop", s);
#endif

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

    while(on_weight) {
        ActivateCardISO14443A act = alho_settings.reader.activate_idle.func().value<ActivateCardISO14443A>();
        MifareCardSector card(act, alho_settings.reader, card_code, data_block);

        if ( !card.active() ) {
            sleepnbtm();
            continue;
        }

        try {
            printOnTablo(tr(processing_message));

            card.autorize();

            //qDebug() << getReaderBytes(card);

            WebServiceAsync w(*this);
            auto ret = w.exchangeData( mapToString( getSimpleTagsValues(  ) ) + ",\n" + getReaderBytes(card) );

            sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
            printOnTablo( tr(apply_card_message) );                                  
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
        //qDebug( )     << "1";
        wake_timer.stop();
        //qDebug( )     << "2";

        if (  status() == Stopped  )
            wakeUp();
    }
    //qDebug( )     << "3";

    qDebug() << "disappear finished!!!";

}


void WebServiceSequence::wakeUp()
{
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
