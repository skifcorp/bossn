#include "soapbossnSoapBindingProxy.h"
#include "bossnSoapBinding.nsmap"
#include "webservicesequence.h"

#include "asyncfuncbase.h"
#include "settingstool.h"

#include "warnmessages.h"
#include "mifarereader.h"
#include "mifarecard.h"
#include "func.h"

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextStream>
#include <QDir>

#include <sstream>
#include <iosfwd>
#include <ios>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/thread.hpp>

#include "fatal_exit.h"





void BlockData::checkData(int place) const
{
    if (  block_num < 0 ) {
        fatal_exit("block_num not initialized!!!", "place", place);
    }

    if ( data_.size() != block_size) {
        fatal_exit("bad block size", blockSize() , "place", place);
    }

}

QString NumberedBlockData::toString () const
{
    checkData(1);

    QString string_data, delim;
    for ( int i = 0; i<data_.size(); ++i ) {
        string_data += delim + QString::number(static_cast<uchar>(data_[i]), 16);
        delim = " ";
    }

    return QString::number( blockNum() ) + ":" + string_data;
}

void NumberedBlockData::fromString (const QString& s)
{
    data_.clear();
    setBlockNum(-1);

    QStringList key_with_bytes = s.split(":");
    if ( key_with_bytes.size() != 2 ) {
        throw MainSequenceException( one_block_data_corrupted, "after splitting get count: "
                                     + QString::number(key_with_bytes.size())  );
    }

    bool ok = false;
    int bn = key_with_bytes.first().toInt(&ok);

    if (!ok) {
        throw MainSequenceException( one_block_data_corrupted, " cant get block num: " + key_with_bytes.first() );
    }

    setBlockNum(bn);

    if ( bn < 0 || bn > blocks_count - 1) {
        throw MainSequenceException( one_block_data_corrupted, " bad block num: " + QString::number(bn) );
    }

    QStringList bytes = key_with_bytes.last().split( " ", QString::SkipEmptyParts );

    if ( bytes.count() > 0 && bytes.count() != block_size ) {
        throw MainSequenceException(web_service_wrong_reader_bytes_count, "web service returned " +
                                    QString::number(bytes.count()) +  " bytes for reader!");
    }

    for(const QString& byte : bytes ) {
        bool ok = false;
        auto ret = byte.toInt(&ok, 16);
        if ( !ok ) {
            throw MainSequenceException(web_service_corrupted_reader_byte, "web service returned corrupted byte (" +
                                        byte + ") for reader!");
        }
        data_.push_back( ret );
    }
}






QString DefaultedBlockData::toString () const
{
    QString string_data, delim;
    for ( int i = 0; i<data_.size(); ++i ) {
        string_data += delim + QString::number(static_cast<uchar>(data_[i]), 16);
        delim = " ";
    }

    return string_data;
}

void DefaultedBlockData::fromString (const QString& s)
{
    data_.clear();

    QStringList bytes = s.split( " ", QString::SkipEmptyParts );

    if ( bytes.count() > 0 && bytes.count() != block_size ) {
        throw MainSequenceException(web_service_wrong_reader_bytes_count, "2: web service returned " +
                                    QString::number(bytes.count()) +  " bytes for reader!");
    }

    for(const QString& byte : bytes ) {
        bool ok = false;
        auto ret = byte.toInt(&ok, 16);
        if ( !ok ) {
            throw MainSequenceException(web_service_corrupted_reader_byte, "web service returned corrupted byte (" +
                                        byte + ") for reader!");
        }
        data_.push_back( ret );
    }
}






QString NumberedBlocksData::toString() const
{
    QString ret = "{";
    QString sep;

    for ( const auto & bd : data_list ) {
        ret += sep + bd->toString();
        sep = ",";
    }

    ret += "}";

    return ret;
}

void NumberedBlocksData::fromString( const QString& s )
{
    data_list.clear();

    int left_sep = s.indexOf("{");
    int right_sep = s.indexOf("}");

    if ( left_sep == -1 ) {
        throw MainSequenceException(web_service_corrupted_reader_byte, "cant find left separator {");
    }

    if ( right_sep == -1 ) {
        throw MainSequenceException(web_service_corrupted_reader_byte, "cant find right separator }");
    }

    QString string_data = s.mid( left_sep, right_sep - left_sep + 1 );

    QStringList string_blocks = string_data.split(",");

    for ( const auto& d : string_blocks ) {
        std::unique_ptr<BlockData> bd = std::unique_ptr<NumberedBlockData>(new NumberedBlockData() );
        bd->fromString( d );
        data_list.push_back( std::move(bd) );
    }
}
void NumberedBlocksData::append(int bl, const QByteArray & d)
{
    data_list.push_back( std::unique_ptr<BlockData>(new NumberedBlockData(bl, d)) );
}



void DefaultedBlocksData::append(int bl, const QByteArray & d)
{
    data_list.push_back( std::unique_ptr<BlockData>(new DefaultedBlockData(bl, d)) );
}


QString DefaultedBlocksData::toString() const
{
    QString sep;
    QString ret;

    for ( const auto & bd : data_list ) {
        ret += sep + bd->toString();
        sep = " ";
    }

    return ret;
}

void DefaultedBlocksData::fromString( const QString& string_data )
{
    data_list.clear();

    const int whitespace_num = bytes_num - 1;
    if ( string_data.count(" ") != whitespace_num  ) {
        throw MainSequenceException(web_service_wrong_reader_bytes_count,
                                    "DefaultedBlocksData::fromString: bad number of whitespaces " +
                                  QString::number(string_data.count(" ")) +  "in bytes for reader!");

    }

    QString b1 = blockByteString( 0, string_data );
    //qDebug() << "b1: " << b1;
    QString b2 = blockByteString( 1, string_data );
    //qDebug() << "b2: " << b2;
    QString b3 = blockByteString( 2, string_data );
    //qDebug() << " b3: " << b3;

    std::unique_ptr<BlockData> bd1 = std::unique_ptr<BlockData>(new DefaultedBlockData() );
    bd1->fromString( b1 );
    bd1->setBlockNum(block_num1);
    data_list.push_back( std::move(bd1) );

    std::unique_ptr<BlockData> bd2 = std::unique_ptr<BlockData>(new DefaultedBlockData() );
    bd2->fromString( b2 );
    bd2->setBlockNum(block_num2);
    data_list.push_back( std::move(bd2) );

    std::unique_ptr<BlockData> bd3 = std::unique_ptr<BlockData>(new DefaultedBlockData() );
    bd3->fromString( b3 );
    bd3->setBlockNum(block_num3);
    data_list.push_back( std::move(bd3) );
}

int DefaultedBlocksData::whitespacePos( int num, const QString& s ) const
{
    if ( num < 0 )
        return 0;
    if (  num > s.count(" ") )
        return s.length();

    int counter = 0;
    int pos = 0;
    while ( counter < num  && pos < s.length() - 1 )  {
        pos = s.indexOf(" ", pos);
        if ( pos == -1 ) {
            qWarning() << "something terrible because I must find all whitespaces in this string: " << s
                       << " counter: " << counter <<  " pos " << pos << "num: " << num;
            qFatal("exit");
        }

        ++pos;
        ++counter;
    }

    return pos;
}

QString DefaultedBlocksData::blockByteString(int bn, const QString& s) const
{
    const int start_whitespace_num  = bn * BlockData::block_size + ( bn == 0? -1  : 0 );
    const int finish_whitespace_num = (bn + 1) * BlockData::block_size;


    //qDebug () << "start_white_space: " << start_whitespace_num << " finish_whitespace_num " << finish_whitespace_num;

    const int string_start_pos = whitespacePos( start_whitespace_num, s );
    const int string_finish_pos = whitespacePos( finish_whitespace_num, s );

    //qDebug () << "start_white_space_pos: " << string_start_pos << " finish_whitespace_num_pos " << string_finish_pos;

    const int len = string_finish_pos - string_start_pos;

    return s.mid(string_start_pos, len).simplified();
}



QVariantList DefaultedBlocksData::blockConf()
{
    QVariantList ret;

    QVariantMap m1;
    m1["block_num"] = block_num1;
    m1["block_size"] = 16;
    ret.push_back(m1);

    QVariantMap m2;
    m1["block_num"] = block_num2;
    m1["block_size"] = 16;
    ret.push_back(m2);

    QVariantMap m3;
    m1["block_num"] = block_num3;
    m1["block_size"] = 16;
    ret.push_back(m3);

    return ret;
}

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

class MainSequenceHiddenException
{
public:
    MainSequenceHiddenException() = default;
    MainSequenceHiddenException( int erc ) : error_code(erc){}

    int errorCode() const {return error_code;}

private:
    int error_code = 0;
};


class GsoapSource
{
public:
    GsoapSource( Coroutine2& c, const QString& ip, int port):coro_(c), ip_(ip), port_(port){}

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
                sh.exechange(ip_, port_);
                if ( terminate_ ) {
                    //qDebug() << "in gsoap with terminating flag!!!!!!!!!!";
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
        //if (coro_.status() != Coroutine2::Stopped)
        //qDebug() << "before cont!!!";
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
    QString ip_;
    int port_ = 0;
};


template <class RealSource>
class FakeSource
{
public:
    using char_type = char;
    using category  = io::bidirectional_device_tag;

    template <class ... Args>
    FakeSource( Args && ... args ):source_(new RealSource(std::forward<Args>(args)...))
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
        if (!source_) {
            fatal_exit("big error in fakesource::terminate - source_ is null!!!");
        }

        source_->terminate();
    }

    bool isTerminating( ) const
    {
        return source_->isTerminating();
    }
private:
    //std::shared_ptr<GsoapSource> source_;
    std::shared_ptr<RealSource> source_;
};


SocketHelper::SocketHelper(GsoapSource & s):source_(s)
{
}

SocketHelper::~SocketHelper()
{
    //qDebug() << "socket helper destructed!!!!!!!!!!!";
}

void SocketHelper::exechange(const QString& ip, int port)
{
    auto socket_   = getSocket();
    auto timeout_timer = getTimer();

    error = timeout = got_result = false;

    socket_->connectToHost(ip, port);
    timeout_timer->start(10000);

    if ( socket_->state() != QTcpSocket::ConnectedState ) {
        while ( !got_result ) {
            source_.coro().yield();
            if ( source_.isTerminating() )
                return;
        }
        got_result = false;
    }

    if (  error ) {
        throw MainSequenceException( connect_to_service_server_error, "connection to ip " + ip
                                     + " port: "
                                     + QString::number(port)
                                     + " error! ", socket_->errorString() );
    }

    if (  timeout )
        throw MainSequenceException( connect_to_service_server_timeout, "connection to host " + ip
                                     + " port: "
                                     + QString::number( port )
                                     + " timeout! ", "" );



    socket_->write( source_.writeBuffer().c_str() );

    timeout_timer->start(10000);
    //qDebug() << "socketHelper ->exchange-> after write - waiting for answer!!!!";
    while ( !got_result ) {
        //qDebug() << "before yield";
        source_.coro().yield();
        //qDebug() << "after yield";

        if ( source_.isTerminating() ) {
            //qDebug() << "terminating flag present!!!!!!!!!";
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
    //timeout_timer->stop();
    //socket_->flush();
    //socket_->close();
    //qDebug() << "socket closed!";

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
}

void SocketHelper::onTimeout()
{
    //qWarning() << "socket timeout!";

    got_result = timeout = true;

    if ( source_.coro().status() != Coroutine2::Stopped ) {
        qWarning() << "Surious error! Got readyRead in socket helper while coro is not in stopped state!!";
        return;
    }


    source_.coro().cont();
}






class GsoapBlockingSource
{
public:
    GsoapBlockingSource( const QString& ip, int port): ip_(ip), port_(port){}

    ~GsoapBlockingSource()
    {
    }

    GsoapBlockingSource( const GsoapSource& ) = delete;

    std::streamsize read(char* s, std::streamsize n)
    {
        if ( !buffer_initialized ) {
            buffer_initialized  = true;
            try {
                SocketHelperWithBlocking sh(*this);
                sh.exechange(ip_, port_);
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

    void exception()
    {
        if (has_exception) {
            has_exception = false;
            throw current_exception;
        }
    }

private:
    std::string read_buffer;

    string write_buffer;
    string::size_type pos_ = 0;
    bool buffer_initialized = false;
    MainSequenceException current_exception;
    bool has_exception = false;

    QString ip_;
    int port_ = 0;
};







SocketHelperWithBlocking::SocketHelperWithBlocking(GsoapBlockingSource & s):source_(s)
{
}

SocketHelperWithBlocking::~SocketHelperWithBlocking()
{
}

void SocketHelperWithBlocking::exechange(const QString& ip, int port)
{
    QTcpSocket socket_;


    //connect( &socket_, SIGNAL(error(QAbstractSocket::SocketError)),
    //         this, SLOT(onError(QAbstractSocket::SocketError)));


    socket_.connectToHost(ip, port);
    if ( !socket_.waitForConnected( 10000 ) ) {
        throw MainSequenceException( connect_to_service_server_error, "connection to ip " + ip
                                     + " port: "
                                     + QString::number(port)
                                     + " error! ", socket_.errorString() );
    }

    socket_.write( source_.writeBuffer().c_str() );
    if (!socket_.waitForBytesWritten(10000) ) {
        throw MainSequenceException( write_to_service_server_error, "write to ip " + ip
                                     + " port: "
                                     + QString::number(port)
                                     + " error! ", socket_.errorString() );
        return;
    }


    if (!socket_.waitForReadyRead(10000) ) {
        throw MainSequenceException( read_from_service_server_error, "read from host " + socket_.peerAddress().toString() +
                                     " error!", socket_.errorString() );
    }

    QByteArray arr = socket_.readAll();

    std::string s(arr.data(), arr.size());

    source_.setReadBuffer( s );
}
















template <class RealSource>
class AutoDestroybossnSoapBindingProxy : public bossnSoapBindingProxy
{
public:
    template <class ... Args>
    AutoDestroybossnSoapBindingProxy(const char * userid, const char * passwd, Args && ... args
                                     ) : bossnSoapBindingProxy(SOAP_C_UTFSTRING), source_(std::forward<Args>(args)...)
    {
        soap->fconnect    = fake_connect;
        soap->fopen       = nullptr;
        soap->fdisconnect = fake_disconnect;
        soap->fclose      = nullptr;

        soap->userid = userid;
        soap->passwd = passwd;

        soap->os = &is;
        soap->is = &is;

        is.open( source_, 0 );
    }

    ~AutoDestroybossnSoapBindingProxy()
    {
        destroy();
    }

    auto& source()
    {
        return source_;
    }

private:
    io::stream < FakeSource<RealSource> > is;
    FakeSource<RealSource> source_;
};



template <class RealSource>
class WebServiceAsync
{
public:
    WebServiceAsync(const QString& ip, int port):
        ip_(ip), port_(port)
    {

    }

    ~WebServiceAsync()
    {

    }

    std::pair<QString, QString> exchangeData(const QString& s, const QString& platform_id, const QString& uid, const char * userid, const char * passwd, Coroutine2 & coro_)
    {
        _ns1__ExchangeResponse resp;
        try {
            makeCall( [&](AutoDestroybossnSoapBindingProxy<RealSource>& proxy){
                _ns1__Exchange arg;

                arg.param = s.toStdString();
                arg.platformId = platform_id.toStdString();
                arg.RFID_USCOREUID = uid.toStdString();

                return proxy.Exchange( &arg, &resp );
            }, userid, passwd, coro_ );
        }
        catch(const MainSequenceHiddenException& ex ) {
            throw MainSequenceException( gsoap_data_exchange_request_error, "1: Error in request data: " +
                                         QString::number(ex.errorCode()), "");
        }

        if (!resp.return_) {
            return std::make_pair(QString(), QString());
        }

        return  std::make_pair( QString::fromUtf8( resp.return_->tags.c_str() ),
                                QString::fromUtf8( resp.return_->additional.c_str() ) );
    }

    void acceptedCardResult( bool res, const QString& platform_id, const char * userid, const char * passwd, Coroutine2 & coro_ )
    {
        try {
            makeCall( [&](AutoDestroybossnSoapBindingProxy<RealSource>& proxy){
                _ns1__Accept arg;
                _ns1__AcceptResponse resp;

                arg.flag = res;
                arg.platformId = platform_id.toStdString();

                return proxy.Accept( &arg, &resp );
            }, userid, passwd, coro_ );
        }
        catch(const MainSequenceHiddenException& ex ) {
            throw MainSequenceException( gsoap_accept_card_result_request_error,
                                         "2: Error in accept card!!! errc: " + QString::number(ex.errorCode()));
        }
    }

    void appeared( const QString& platform_id, const char * userid, const char * passwd, Coroutine2 & coro_ )
    {
        try {
            makeCall( [&](AutoDestroybossnSoapBindingProxy<RealSource>& proxy){
                _ns1__Appeared arg;
                _ns1__AppearedResponse resp;

                arg.platformId = platform_id.toStdString();

                return proxy.Appeared( &arg, &resp );
            }, userid, passwd, coro_ );
        }
        catch(const MainSequenceHiddenException& ex ) {
            throw MainSequenceException( gsoap_appear_request_error,
                                         "3: Error in appear errc: " + QString::number(ex.errorCode()));
        }
    }


    void disappear( const QString& platform_id, const char * userid, const char * passwd )
    {
        try {
            AutoDestroybossnSoapBindingProxy<RealSource> proxy(userid, passwd, ip_, port_);
            _ns1__Disappeared arg;
            _ns1__DisappearedResponse resp;

            arg.platformId = platform_id.toStdString();

            auto ret = proxy.Disappeared( &arg, &resp );

            if (  ret != SOAP_OK ) {
                proxy.source().exception();
                throw MainSequenceHiddenException(ret);
            }
        }
        catch(const MainSequenceHiddenException& ex ) {
            throw MainSequenceException( gsoap_disappear_request_error,
                                         "4: Error in appear errc: " + QString::number(ex.errorCode()));
        }
    }

    void terminate()
    {
        terminating_ = true;
        if (!cur_fake_source) {
            fatal_exit("big error in webserviceasync::terminate - cur_fake_source is null!!!");
        }
        //qDebug() << "111111111111";
        cur_fake_source->terminate();
    }

    bool isTerminating() const
    {
        if (!cur_fake_source)
            return terminating_;

        return cur_fake_source->isTerminating(); //ugly hack
    }
    bool isRunning() const
    {
        return cur_fake_source != nullptr;
    }
private:
    //Coroutine2 & coro_;
    FakeSource<RealSource> * cur_fake_source = nullptr;
    bool terminating_ = false;
    QString ip_;
    int port_;

    template <class F>
    void makeCall(F && f, const char * userid, const char * passwd, Coroutine2 & coro_)
    {
        std::shared_ptr<WebServiceAsync> cur_fake_source_guard( this , [&](WebServiceAsync * wsa){
            wsa->cur_fake_source = nullptr;
        } ); Q_UNUSED(cur_fake_source_guard);

        terminating_ = false;

        Q_ASSERT( !cur_fake_source );

        AutoDestroybossnSoapBindingProxy<RealSource> proxy(userid, passwd, coro_, ip_, port_);

        cur_fake_source = &proxy.source();

        int ret = f( proxy );
        //qDebug() << "inMakeCall -> checking terminate_flag!!!!!!!!";
        if ( cur_fake_source->isTerminating() ) {
            //qDebug() << "inMakeCall -> GOT terminate flag!!!; returning";
            return ;
        }

        if ( ret != SOAP_OK ) {
            cur_fake_source->exception();

            throw MainSequenceHiddenException(ret);  //MainSequenceException( gsoap_accept_card_result_request_error, "Error in accept card!!!");
        }
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

    seq_id                  = get_setting<int>("id", s);

    ip_                     = get_setting<QString>("ip", s);
    port_                   = get_setting<int>("port", s);
    userid_                 = get_setting<QString>("userid", s);
    passwd_                 = get_setting<QString>("passwd", s);


    QVariantList rbc = get_setting<QVariantList>("read_blocks_conf", s, DefaultedBlocksData::blockConf() );

    for ( const auto& bc : rbc ) {
        const auto& m = bc.toMap();

        read_blocks_conf.push_back( BlockConf( get_setting<int>("block_num", m),
                                              get_setting<int>("block_size", m) ) );
    }

    uses_photo                              = get_setting<bool>("uses_photo", s, false);

    if (uses_photo) {
        exit_photo                          = get_setting<QVariantMap>("exit_photo", s);
        enter_photo                         = get_setting<QVariantMap>("enter_photo", s);
    }

    photo_base_path_arg_name  = get_setting<QString>("photo_base_path_arg_name", s, "photo_dir");
    photo_rel_path_arg_name   = get_setting<QString>("photo_rel_path_arg_name", s, "jpeg_dir");
    photo_file_base_name      = get_setting<QString>("photo_file_base_name", s, "jpeg_file");

    setObjectName( "MainSequence num: " + QString::number(seq_id) );

    restart();

    cont();  //for initializing tablo and do
}

void WebServiceSequence::run()
{
    if ( init ) {
        //printOnTablo(tr(greeting_message));

        printOnTablo(tr2(greeting_message));

        setLightsToGreen();
        init = false;
        return;
    }

    seqDebug() << "something appeared on weight!!!! id" << seq_id;

    on_weight = true;
    wake_timer.setTurnedOn(true);

    setLightsToRed();

    while (on_weight)    {
        std::shared_ptr<WebServiceSequence> cur_fake_source_guard( this ,[&]
        (WebServiceSequence * wss){
            wss->cur_webservice_async = nullptr;
        } ); Q_UNUSED(cur_fake_source_guard);

        Q_ASSERT(!cur_webservice_async);

        WebServiceAsync<GsoapSource> was(ip_, port_);

        cur_webservice_async = &was;

        try {
            QByteArray userid = userid_.toAscii();
            QByteArray passwd = passwd_.toAscii();

            was.appeared( QString::number(seqId()), userid.data(), passwd.data(), *this );
            if ( was.isTerminating() )
                continue;

            break;
        }
        catch( const MainSequenceException& ex ) {
            seqWarning()<<"sequence_exception in appeared: " << ex.adminMessage() << " sys: " + ex.systemMessage();
            sleepnbtmerr(ex.userMessage(), tr2(apply_card_message));
            continue;
        }
    }

    bool main_part_started = false;
    if ( on_weight ) {
        printOnTablo(tr2(apply_card_message));

        alho_settings.reader.all_readers_do_on();
        main_part_started = true;
    }

    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    std::shared_ptr<WebServiceSequence> current_card_guard( this, [this](WebServiceSequence*){
        alho_settings.current_card.setProperty(QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    }); Q_UNUSED(current_card_guard);


    while(on_weight) {
        std::pair<ActivateCardISO14443A, int> act = alho_settings.reader.all_readers_activate_idle();

        if ( act.second == -1 ) {
            sleepnbtm();
            continue;
        }

        MifareCardSector card(act.first, alho_settings.reader[act.second], card_code, data_block);

        if ( !card.active() ) {
            sleepnbtm();
            continue;
        }

        try {
            std::shared_ptr<WebServiceSequence> cur_fake_source_guard( this ,[&]
            (WebServiceSequence * wss){
                wss->cur_webservice_async = nullptr;
            } ); Q_UNUSED(cur_fake_source_guard);

            Q_ASSERT(!cur_webservice_async);

            //printOnTablo(tr2(processing_message));

            card.autorize();            

            checkForStealedCard( act.first );

            WebServiceAsync<GsoapSource> was(ip_, port_);
            cur_webservice_async = &was;

            QByteArray userid = userid_.toAscii();
            QByteArray passwd = passwd_.toAscii();

            QString ret_data;
            QString add_data;

            std::tie(ret_data, add_data) =
                    was.exchangeData( mapToString( getSimpleTagsValues(  ) ) +
                        ",\n" + getReaderBytes(card), QString::number(seqId()),
                        byteArrayToString(act.first.uid, 16, ""), userid.data(), passwd.data(), *this );


            if (was.isTerminating()) {
                //qDebug() << "after excgange with terminate";
                continue;
            }


            if ( ret_data.isEmpty() ) {
                throw MainSequenceException( tr2(internal_webservice_error), "returned data is empty" );
            }

            printOnDisplay( ret_data );
            printOnDisplay( add_data );


            if ( ret_data == "-1" ) {
                throw MainSequenceException( tr2(internal_webservice_error), "internal webservice error" );
            }             

            try {
                QMap<QString, QString> ret =  stringToMap(ret_data);
                writeTagsValues( ret, card );

                QMap<QString, QString> add_map = stringToMap(add_data);
                QString photo_base_path, photo_rel_path, photo_file_path;

                std::tie(photo_base_path, photo_rel_path, photo_file_path)= tryGetPhotoPathes( add_map );

                if ( !photo_base_path.isEmpty() && !photo_rel_path.isEmpty() && !photo_file_path.isEmpty() ) {
                    QString photo_abs_path = getPhotoFullPathCreatingSubdirs( photo_base_path, photo_rel_path );
                    if ( !photo_abs_path.isEmpty() )
                        makePhoto( photo_abs_path,  photo_file_path);
                }
            }
            catch ( const MifareCardWriteException& ) {
                was.acceptedCardResult(false, QString::number(seqId()), userid.data(), passwd.data(), *this);
                throw;
            }

            alho_settings.reader[act.second].do_sound.func(Q_ARG(QVariant, appSetting<int>("beep_length")));
            was.acceptedCardResult(true, QString::number(seqId()), userid.data(), passwd.data(), *this);

            sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
            printOnTablo( tr2(apply_card_message) );

            continue;
        }
        catch (MifareCardAuthException& ex) {
            seqWarning() << "auth_exeption! "<<ex.adminMessage();

            sleepnbtmerr(tr2(card_autorize_error_message), tr2(apply_card_message));
            continue;
        }
        catch (MifareCardReadException& ex) {
            seqWarning() << "read_card_exception! "<<ex.adminMessage();
            sleepnbtmerr(ex.userMessage(), tr2(apply_card_message));
            continue;
        }
        catch (MifareCardWriteException& ex) {
            seqWarning() << "write_card_exception! "<<ex.adminMessage();
            sleepnbtmerr(ex.userMessage(), tr2(apply_card_message));
            continue;
        }
        catch (MainSequenceException& ex) {
            seqWarning()<<"sequence_exception: " << ex.adminMessage() << " sys: " + ex.systemMessage();
            printOnDisplay( "user_message: " + ex.userMessage() );
            sleepnbtmerr(ex.userMessage(), tr2(apply_card_message));
            continue;
        }
        catch (MifareCardException& ex) {
            seqWarning()<<"mifare_card_exception: " << ex.adminMessage();
            sleepnbtmerr(ex.userMessage(), tr2(apply_card_message));
            continue;
        }

    }


    printOnTablo(tr2(greeting_message));
    setLightsToGreen();

    if ( main_part_started )
        alho_settings.reader.all_readers_do_off();

    int loc_seq_id = seqId();
    auto ip__ = ip_;
    auto port__ = port_;
    auto userid__ = userid_;
    auto passwd__ = passwd_;
    boost::thread disappear_thread([ip__, port__, userid__, passwd__, loc_seq_id] {
            try {
                WebServiceAsync<GsoapBlockingSource> was(ip__, port__);

                QByteArray userid = userid__.toAscii();
                QByteArray passwd = passwd__.toAscii();
                was.disappear( QString::number(loc_seq_id), userid.data(), passwd.data() );
            }
            catch (const MainSequenceException& ex) {
                qDebug() << "sequence_exception on disappear: " <<  ex.adminMessage() << " sys: " + ex.systemMessage();
            }
            qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!disappear_thread finished";
    });

    disappear_thread.detach();

    seqDebug () << "exit from onAppearOnWeight!!!!!!!";
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
    wake_timer.setTurnedOn(false);
    if ( wake_timer.isActive() ) {
        wake_timer.stop();
        //qDebug() << "disap1 ";
        if (  status() == Stopped  ) {
            //qDebug() << "disap1.1 ";
            wakeUp();
            //qDebug() << "disap1.2 ";
        }
    }
    //qDebug() << "disap2 ";
    if ( cur_webservice_async && cur_webservice_async->isRunning() ) {
        //qDebug() << "disap2.1 ";
        cur_webservice_async->terminate();
        //qDebug() << "disap2.2 ";
    }
    //qDebug() << "disap3 ";

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
#if 0
    //QByteArray card_bytes = card.readByteArray( CardStructs::blocks_conf() );
    QByteArray card_bytes = card.readByteArray( read_blocks_conf );
    QString delim;
    QString ret = card.readerTagMethods().name + ":";

    for ( int i = 0; i<card_bytes.count(); ++i ) {
        ret += delim + QString::number(static_cast<uchar>(card_bytes[i]), 16);
        delim = " ";
    }

    return ret;
#endif
    if ( cardreader_web_protocol == CardReaderWebProtocol::Defaulted ) {
        DefaultedBlocksData ret;
        for ( const auto& bc : read_blocks_conf )     {
            //BlockData bd(bc.blockNum, card.readBlock(bc));
            ret.append(bc.blockNum, card.readBlock(bc));
        }
        return card.readerTagMethods().name + ":" + ret.toString();
    }
    else if ( cardreader_web_protocol == CardReaderWebProtocol::Numbered ) {
        NumberedBlocksData ret;
        for ( const auto& bc : read_blocks_conf )     {
            //BlockData bd(bc.blockNum, card.readBlock(bc));
            ret.append(bc.blockNum, card.readBlock(bc));
        }
        return  card.readerTagMethods().name + ":" + ret.toString();
    }
    else {
        qWarning()<< "1: unknown card reader web protocol: " << static_cast<int>(cardreader_web_protocol);
        qFatal("exit");
    }
    return QString();
}


void WebServiceSequence::writeReaderBytes( const QString& s, MifareCardSector&  card )
{
#if 0
    QByteArray arr;

    QStringList bytes = s.split( " ", QString::SkipEmptyParts );

    if ( bytes.count() > 0 && static_cast<uint>(bytes.count()) != CardStructs::blocks_conf().memorySize() ) {
        throw MainSequenceException(web_service_wrong_reader_bytes_count, "web service returned " +
                                    QString::number(bytes.count()) +  " bytes for reader!");
    }

    for(const QString& byte : bytes ) {
        bool ok = false;
        auto ret = byte.toInt(&ok, 16);
        if ( !ok ) {
            throw MainSequenceException(web_service_corrupted_reader_byte, "web service returned corrupted byte (" +
                                        byte + ") for reader!");
        }
        arr.push_back( ret );

    }

    if ( bytes.count() > 0 )
        card.writeByteArray( arr, CardStructs::blocks_conf() );
#endif
    if ( cardreader_web_protocol == CardReaderWebProtocol::Defaulted  ) {
        DefaultedBlocksData bsd;
        bsd.fromString( s );

        for ( auto iter = bsd.begin(); iter != bsd.end(); ++iter ) {
            card.writeBlock( (*iter)->data(), BlockConf( (*iter)->blockNum(), (*iter)->blockSize() ) );
        }
    }
    else if (cardreader_web_protocol == CardReaderWebProtocol::Numbered ){
        NumberedBlocksData bsd;
        bsd.fromString( s );

        for ( auto iter = bsd.begin(); iter != bsd.end(); ++iter ) {
            card.writeBlock( (*iter)->data(), BlockConf( (*iter)->blockNum(), (*iter)->blockSize() ) );
        }

    }
}

void WebServiceSequence::writeTagsValues( const QMap<QString, QString>& m, MifareCardSector&  card )
{
    QString tablo_text;
    bool found = false;
    for ( QMap<QString, QString>::const_iterator iter = m.begin(); iter != m.end(); ++iter ) {
        if ( alho_settings.tablo_tag.containsTag( iter.key() ) ) {
            tablo_text = *iter;
            found = true;
            break;
        }
    }

    if ( !found ) {
        throw MainSequenceException("Tablo tag error!" ,"Tablo tag not found!!!!!!");
    }

    auto reader_bytes = m.find( card.readerTagMethods().name );
    if ( reader_bytes == m.end() ) {
        throw MainSequenceException("Reader tag error", "reader tag not found!!!!!! for " + card.readerTagMethods().name);
    }

    if ( reader_bytes->isEmpty() )
        throw  MainSequenceException(tablo_text, "Hi level alho error! ");

    //alho_settings.tablo_tag.func( Q_ARG(QVariant, QVariant::fromValue(tablo_text) ) );

    writeReaderBytes( *reader_bytes, card );

    for ( QMap<QString, QString>::const_iterator iter = m.begin(); iter != m.end(); ++iter ) {
        auto tablo_tag_iter = alho_settings.tablo_tag.find( iter.key() );

        if ( tablo_tag_iter != alho_settings.tablo_tag.end() ) {
            tablo_tag_iter->func( Q_ARG(QVariant, QVariant::fromValue( *iter ) )  );
        }
    }

}


std::tuple<QString, QString, QString> WebServiceSequence::tryGetPhotoPathes( const QMap<QString, QString>& args ) const
{
    std::tuple<QString, QString, QString> ret;

    {
        auto iter = args.find(photo_base_path_arg_name);
        if (  iter == args.end() ) return ret;
        std::get<0>(ret) = *iter;
    }

    {
        auto iter = args.find(photo_rel_path_arg_name);
        if (  iter == args.end() ) return ret;
        std::get<1>(ret) = *iter;
    }

    {
        auto iter = args.find(photo_file_base_name);
        if (  iter == args.end() ) return ret;
        std::get<2>(ret) = *iter;
    }


    return ret;
}


QString WebServiceSequence::getPhotoFullPathCreatingSubdirs(const QString& photo_base_path, const QString& photo_rel_path )
{
    QDir base_dir(photo_base_path);

    if ( !base_dir.exists()  ) {
        seqWarning() << "photo_base_path " << photo_base_path << " doesnt exists";
        return QString();
    }

    if ( !base_dir.mkpath( photo_rel_path ) ) {
        seqWarning() << "cant create " << photo_rel_path << " in photo_base_path " << photo_base_path;
        return QString();
    }

    base_dir.cd( photo_rel_path );

    return base_dir.path();
}


void WebServiceSequence::makePhoto(const QString& photo_abs_path, const QString& photo_file_base_name)
{
    if (!uses_photo) {
        seqWarning() << "want photo: " << photo_abs_path << " but uses_photo is false!!!";
        return;
    }

    QFileInfo enter_file( photo_abs_path, photo_file_base_name + get_setting<QString>("channel_alias", enter_photo) );
    QFileInfo exit_file ( photo_abs_path,  photo_file_base_name + get_setting<QString>("channel_alias", exit_photo) );

    QString entr_path = QDir::toNativeSeparators(  enter_file.absoluteFilePath() );
    QString exit_path = QDir::toNativeSeparators(  exit_file.absoluteFilePath() );

    //qDebug() << "enter_file name: " << entr_path;
    //qDebug() << "enter_file name: " << exit_path;

    auto pw1 = entr_path.toStdWString();
    auto pw2 = exit_path.toStdWString();

    capture.grabPhoto(pw1.c_str(),
                      get_setting<QString>("channel_num", enter_photo ).toInt());

    capture.grabPhoto(pw2.c_str() ,
                      get_setting<QString>("channel_num", exit_photo).toInt());
}
