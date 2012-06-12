#include "mifarereader.h"
#include "crctool.h"
#include "iostream"

#include "mifarecard.h"
#include "func.h"

using std::cout;


MifareRequestFrame::MifareRequestFrame():/*startCondition(0xFD),*/ address(0), ident(0), cmdCode(0), crc16(0)/*, finishCondition(0xFE)*/
{  }

MifareRequestFrame::~MifareRequestFrame()
{ }

QByteArray MifareRequestFrame::packFrame()
{
    QByteArray ret;
    ret.reserve(params.length() + 7);

    ret.append(startCondition);
    ret.append(address);
    ret.append(ident);
    ret.append(cmdCode);
    ret.append(params);

    crc16 = ~doCrc16( ret, 1 );

    ret.append(reinterpret_cast<char *>(&crc16), sizeof(crc16));
    ret.append(finishCondition);

    return staffBytes(ret);
}

void MifareRequestFrame::print() const
{
    cout<<"RequestFrame:\n"<<
          "  startCondition  = 0x"<<std::hex<<startCondition<<std::dec<<"\n"<<
          "  address         = "<<address<<"\n"<<
          "  ident           = "<<ident<<"\n"<<
          "  cmdCode         = "<<cmdCode<<"\n"<<
          "  params["<<std::hex<<"\n";
    //for( int i = 0; i< params.size(); ++i ) {
    //    cout<<params[i]<<" ";
    //}
    printByteArray(params);
    cout << "]\n"<<
            "  crc16           = 0x"<<crc16<<"\n"<<
            "  finishCondition = 0x"<<finishCondition<<std::dec<<"\n"<<
            "  paramsLen       = "<<params.length()<<"\n";
}

int MifareRequestFrame::size() const
{
    return params.length() + 7;
}

QByteArray MifareRequestFrame::staffBytes(const QByteArray& arr)
{
    if ( arr.isEmpty() ) return QByteArray();

    QByteArray ret;
    ret.reserve(arr.size());

    ret.append( arr[0] );
    for( int i = 1; i<arr.length()-1; ++i ) {
      switch ( static_cast<uchar>(arr[i]) ) {
        case 0xFD:
            ret.append(0xFF);
            ret.append(0x02);
        break;
        case 0xFE:
            ret.append(0xFF);
            ret.append(0x01);
        break;
        case 0xFF:
            ret.append(0xFF);
            ret.append(static_cast<char>(0x00));
        break;
        default:
          ret.append(arr[i]);
        break;
      }
    }
    ret.append( arr[arr.length() - 1] );

    return ret;
}

MifareResponseFrame::MifareResponseFrame()
                    :startCondition(0),
                     address(0),
                     ident(0),
                     cmdCode(0),
                     cmdStatus(0),
                     crc16(0),
                     finishCondition(0)
{
}

MifareResponseFrame::~MifareResponseFrame()
{

}

bool MifareResponseFrame::checkCrc() const
{
    QByteArray arr;
    arr.reserve(params.length() + 4);

    arr.append(address);
    arr.append(ident);
    arr.append(cmdCode);
    arr.append(cmdStatus);
    arr.append(params);

    ushort calc_crc = ~doCrc16(arr);

    return crc16 == calc_crc;//(ushort)~calc_crc;
}

void MifareResponseFrame::print() const
{
    cout<<"ResponseFrame:\n"<<
          "  startCondition  = 0x"<<std::hex<<startCondition<<std::dec<<"\n"<<
          "  address         = "<<address<<"\n"<<
          "  ident           = "<<ident<<"\n"<<
          "  cmdCode         = "<<cmdCode<<"\n"<<
          "  cmdStatus       = "<<cmdStatus<<"\n"<<
          "  params["<<std::hex<<"\n";
    //for( int i = 0; i< params.size(); ++i ) {
    //    cout<<params[i]<<" ";
    //}
    printByteArray(params);
    cout << "]\n"<<
            "  crc16           = 0x"<<crc16<<"\n"<<
            "  finishCondition = 0x"<<finishCondition<<std::dec<<"\n"<<
            "  paramsLen       = "<<params.length()<<"\n";
}

int MifareResponseFrame::size() const
{
    return params.size() + 8;
}

bool MifareResponseFrame::unpackFrame(const QByteArray & frame)
{
    QByteArray arr = unstaffBytes(frame);

/*    cout << std::hex;
    for (int i = 0 ;i < arr.length(); ++i) {
        cout << (ushort)(uchar)arr[i]<<" ";
    }
    cout << std::dec; */

    startCondition   = arr[0];
    address          = arr[1];
    ident            = arr[2];
    cmdCode          = arr[3];
    cmdStatus        = arr[4];
    QByteArray crcba = arr.right(3).left(2);
    crc16            = *reinterpret_cast<ushort*> (crcba.data());

    finishCondition  = arr.right(1)[0];

    params = arr.mid(5, arr.length() - 8);

    return true;
}

bool MifareResponseFrame::checkResponse(const MifareRequestFrame & req)
{
    bool ret = true;
    if (req.startCondition != startCondition) {
        qWarning() << "Check mifare responce: req.startCondition: "<<req.startCondition<<" != resp.startCondition: "<<startCondition;        
        ret = false;
    }

    if (req.address != address) {
        qWarning() << "Check mifare responce: req.address: "<<req.address<<" != resp.address: "<<address;
        ret = false;
    }

    if (req.ident != ident) {
        qWarning() << "Check mifare responce: req.ident: "<<req.ident<<" != resp.ident: "<<ident;
        ret = false;
    }

    if (req.cmdCode != cmdCode) {
        qWarning() << "Check mifare responce: req.cmdCode: "<<req.cmdCode<<" != resp.address: "<<cmdCode;
        ret = false;
    }

    if (req.finishCondition !=  finishCondition) {
        qWarning() << "Check mifare responce: req.finishCondition: "<<req.finishCondition<<" != resp.finishCondition: "<<finishCondition;
        ret = false;
    }

    if (!checkCrc()) {
        qWarning() << "Check mifare responce: crcCheck failed!";
        ret = false;
    }

    if (!ret) {
        qDebug() << "request: ";
        req.print();
        qDebug() << "response: ";
        print();
    }

    return ret;
}

QByteArray MifareResponseFrame::unstaffBytes(const QByteArray & arr)
{
    QByteArray ret;
    ret.reserve(arr.size());

    ret.append(arr[0]);
    for ( int i = 1; i<arr.length()-1; ++i)  {
      if ( (static_cast<uchar>(arr[i]) == 0xFF) && (arr[i+1] == 0x2) ) {
        ret.append(0xFD); ++i;
      }
      else if ( (static_cast<uchar>(arr[i]) == 0xFF) && (arr[i+1] == 0x1) ) {
        ret.append(0xFE); ++i;
      }
      else if ( (static_cast<uchar>(arr[i]) == 0xFF) && (arr[i+1] == 0x0) ) {
          ret.append(0xFF); ++i;
      }
      else {
        ret.append(arr[i]);
      }
    }
    ret.append( arr[arr.length() - 1] );

    return ret;
}

int ActivateCardISO14443A::id = qRegisterMetaType<ActivateCardISO14443A>();
int HostCodedKey::id          = qRegisterMetaType<HostCodedKey>();
int AuthKey::id               = qRegisterMetaType<AuthKey>();
int MifareRead::id            = qRegisterMetaType<MifareRead>();

BossnFactoryRegistrator<MifareReader> MifareReader::registator("MifareReader");

uchar MifareReader::frame_ident = 0;

QString MifareReader::errorMessage(uchar e)
{
    switch(e)
    {
    case 0x00: return "The operation completed successfully";
    case 0xFF: return "card does not respond";
    case 0xFE: return "checksum is wrong";
    case 0xFC: return "Invalid value of the key";
    case 0xFB: return "Parity Error";
    case 0xFA: return "Invalid exit code";
    case 0xF8: return "Invalid byte integrity of UID";
    case 0xF6: return "sector is not authenticated";
    case 0xF5: return "Wrong number of bits received";
    case 0xF4: return "Wrong number of bytes received";
    case 0xF1: return "data Write error";
    case 0xED: return "overflow of the clipboard";
    case 0xEB: return "Invalid start stop conditions";
    case 0xE9: return "Unknown Operation";
    case 0xE8: return "collision";
    case 0xE7: return "Error initializing reader";
    case 0xE6: return "error reset reader";
    case 0xE4: return "Error bitwise anticollision";
    case 0xE1: return "Incorrect coding of acknowledgment";
    case 0xD8: return "Required component is missing";
    case 0xD7: return "Unknown command";
    case 0xD6: return "this version of the command is not supported";
    case 0xD5: return "need to establish another mode";
    case 0xD1: return "transceiver has failed";
    case 0xCF: return "Map is not passed in HALT mode";
    case 0xCC: return "error 10-byte UID";
    case 0xCA: return "This baud rate is not supported";
    case 0xC4: return "Invalid parameter value";
    case 0x9C: return "unknown error occurred";
    case 0x90: return "the receiver buffer overflow";
    case 0x84: return "Invalid format block VALUE";
    };

    return QString("Unknown error code 0x%1").arg(QString().setNum(static_cast<ushort>(static_cast<uchar>(e)), 16));
}


MifareReader::MifareReader(const QVariantMap& conf)
{
    bool ok = false;
    address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;

    qDebug () << "MifareReader CREATED!";
}

MifareReader::~MifareReader()
{

}


QVariant MifareReader::doOn()
{    
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x10;

    auto arr = req_frame.packFrame();

    io_device()->write( arr );

    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader DO ON!!!!!";
        return QVariant(false);
    }

    qDebug() << "doOn finished correctrly";

    return QVariant(true);
}

QVariant MifareReader::doOff()
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x04;
    req_frame.params.append(0x80);
    req_frame.params.append(0x01);

    io_device()->write( req_frame.packFrame() );

    //const uchar ret_frame_length = 8;
    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader DO OFF!!!!!";
        return QVariant(false);
    }

    return QVariant(true);
}


QVariant MifareReader::doSound(const QVariant& cnt)
{
    uchar count = static_cast<uchar>(cnt.toUInt());

    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x05;
    req_frame.params.append(count);


    auto arr = req_frame.packFrame();

    io_device()->write( arr );

    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader DO SOUND!!!!!";
        return QVariant(false);
    }

    return QVariant(true);
}


QVariant MifareReader::activateIdleA()
{
    MifareRequestFrame req_frame;
    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x43;

    io_device()->write( req_frame.packFrame() );

    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader activateIdleA!!!!!";
        //sharedFromThis();
        return QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A());
    }

    if (resp_frame.cmdStatus != 0) {
        return QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A());
    }

    ActivateCardISO14443A ac;
    ac.ack = resp_frame.cmdStatus;
    ac.atq = resp_frame.params.left(2);
    ac.sak = resp_frame.params[2];
    uchar uid_len = static_cast<uchar>(resp_frame.params[3]);
    ac.uid = resp_frame.params.mid(4).left(uid_len);

    return QVariant::fromValue<ActivateCardISO14443A>(ac);
}

bool MifareReader::waitForAnswer()
{
    QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);

    if (tmp.isEmpty()) return false;

    return  static_cast<uchar>(tmp.right(1)[0]) == MifareRequestFrame::finishCondition ;
}

QVariant MifareReader::getHostCodedKey(const QVariant& key)//const QByteArray& key)
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x16;    
    req_frame.params   = key.toByteArray();


    io_device()->write(req_frame.packFrame());


    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"checkresponce: GOT ERROR IN MifareReader getHostCodeKey!!!!!";
        return QVariant::fromValue<HostCodedKey>(HostCodedKey());
    }

    if (resp_frame.cmdStatus != 0) {
        qWarning() << "cmdStatus: GOT ERROR IN MifareReader getHostCodeKey!!!!!" + errorMessage(resp_frame.cmdStatus);
        return QVariant::fromValue<HostCodedKey>(HostCodedKey());
    }

    HostCodedKey coded_key;
    coded_key.ack   = resp_frame.cmdStatus;
    coded_key.coded = resp_frame.params;

    return QVariant::fromValue<HostCodedKey>(coded_key);
}


QVariant MifareReader::doAuth(const QVariant& var_auth)//const AuthKey & auth)
{
    MifareRequestFrame req_frame;

    AuthKey auth = var_auth.value<AuthKey>();

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x18;
    req_frame.params.append( auth.keyType );
    req_frame.params.append( auth.snd );
    req_frame.params.append( auth.keys );
    req_frame.params.append( auth.block );

    //qDebug () << "params len: "<<req_frame.params.length();

    io_device()->write( req_frame.packFrame() );

    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"check_responce: GOT ERROR IN MifareReader doAuth!!!!!";
        return QVariant(false);
    }

    if (resp_frame.cmdStatus != 0  ) {
        qWarning()<<"cmd_status: GOT ERROR IN MifareReader doAuth!!!!!" + errorMessage(resp_frame.cmdStatus);
        return QVariant(false);
    }

    return QVariant(true);
}

QVariant MifareReader::readBlock(const QVariant& num)
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x19;
    req_frame.params.append(num.toUInt());

    io_device()->write(req_frame.packFrame());

    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"readBlock: check_responce: GOT ERROR IN MifareReader readBlock!!!!!";
        return QVariant::fromValue<MifareRead>(MifareRead());
    }

    if (resp_frame.cmdStatus != 0) {
        qWarning()<<"readBlock: cmdStatus: GOT ERROR IN MifareReader readBlock!!!!! : " + errorMessage(resp_frame.cmdStatus);
        return QVariant::fromValue<MifareRead>(MifareRead());
    }

    MifareRead ret;
    ret.result = resp_frame.cmdStatus;
    ret.data = resp_frame.params;
    ret.result = true;

    qDebug () << "readBlock OK!";

    return QVariant::fromValue<MifareRead>(ret);
}

QVariant MifareReader::writeBlock(const QVariant& block_num, const QVariant& data)
{
    MifareRequestFrame req_frame;

    req_frame.address = address;
    req_frame.ident   = frame_ident++;
    req_frame.cmdCode = 0x1A;
    req_frame.params.append(static_cast<uchar>(block_num.toUInt()));
    req_frame.params.append(data.toByteArray());

    io_device()->write(req_frame.packFrame());

    while (!waitForAnswer()) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;
    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"writeBlock: check_responce: GOT ERROR IN MifareReader writeBlock!!!!!";
        return QVariant(false);
    }

    if (resp_frame.cmdStatus != 0) {
        qWarning()<<"writeBlock: cmdStatus: GOT ERROR IN MifareReader writeBlock!!!!! : " + errorMessage(resp_frame.cmdStatus);
        return QVariant(false);    
    }

    qDebug () << "writeBlock OK!";

    return QVariant(true);
}
