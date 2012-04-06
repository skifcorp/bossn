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
          "  params["<<std::hex;
    for( int i = 0; i< params.size(); ++i ) {
        cout<<params[i]<<" ";
    }
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
          "  params["<<std::hex;
    for( int i = 0; i< params.size(); ++i ) {
        cout<<params[i]<<" ";
    }
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

bool MifareResponseFrame::checkResponce(const MifareRequestFrame & req)
{
    if (req.startCondition != startCondition) {
        qWarning() << "Check mifare responce: req.startCondition: "<<req.startCondition<<" != resp.startCondition: "<<startCondition;
        return false;
    }

    if (req.address != address) {
        qWarning() << "Check mifare responce: req.address: "<<req.address<<" != resp.address: "<<address;
        return false;
    }

    if (req.ident != ident) {
        qWarning() << "Check mifare responce: req.ident: "<<req.ident<<" != resp.ident: "<<ident;
        return false;
    }

    if (req.cmdCode != cmdCode) {
        qWarning() << "Check mifare responce: req.cmdCode: "<<req.cmdCode<<" != resp.address: "<<cmdCode;
        return false;
    }

    if (req.finishCondition !=  finishCondition) {
        qWarning() << "Check mifare responce: req.finishCondition: "<<req.finishCondition<<" != resp.finishCondition: "<<finishCondition;
        return false;
    }

    if (!checkCrc()) {
        qWarning() << "Check mifare responce: crcCheck failed!";
        return false;
    }

    return true;
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


BossnFactoryRegistrator<MifareReader> MifareReader::registator("MifareReader");

uchar MifareReader::frame_ident = 0;


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
/*    cout<<std::hex;
    for ( int i = 0; i<arr.size(); ++i ) {
        cout << (ushort)(uchar)arr[i]<<" ";
    }
    cout << "\n"<<std::dec;*/

    io_device()->write( arr );

    //const uchar ret_frame_length = 8;

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);

        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponce(req_frame)) {
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
    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponce(req_frame)) {
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

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponce(req_frame)) {
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

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }

    QByteArray answ = io_device()->readAll();


    MifareResponseFrame resp_frame;


    resp_frame.unpackFrame(answ);


    if (!resp_frame.checkResponce(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader activateIdleA!!!!!";
        //sharedFromThis();
        return QVariant::fromValue<MifareCard>(MifareCard(this, ActivateCardISO14443A() ));
    }

    if (resp_frame.cmdStatus != 0) {
        return QVariant::fromValue<MifareCard>(MifareCard(this, ActivateCardISO14443A() ));
    }

    ActivateCardISO14443A ac;
    ac.ack = resp_frame.cmdStatus;
    ac.atq = resp_frame.params.left(2);
    ac.sak = resp_frame.params[2];
    uchar uid_len = static_cast<uchar>(resp_frame.params[3]);
    ac.uid = resp_frame.params.mid(4).left(uid_len);

    return QVariant::fromValue<MifareCard>( MifareCard(this, ac ) );
}

bool MifareReader::waitForAnswer()
{
    QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);



    if (tmp.isEmpty()) return false;
    qDebug () << "after";

        qDebug() << "size: " << tmp.size();

    return  static_cast<uchar>(tmp.right(1)[0]) == MifareRequestFrame::finishCondition ;
}

HostCodedKey MifareReader::getHostCodedKey(const QByteArray& key)
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x16;
    req_frame.params   = key;

    qDebug() << "1111";

    io_device()->write(req_frame.packFrame());

    qDebug() << "222";

    while (!waitForAnswer()) {
        //QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        //if ( static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;
        qDebug() << "111111111111";
        yield();
        qDebug() << "222222222222";
    }
qDebug() << "333";
    QByteArray answ = io_device()->readAll();



    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponce(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader getHostCodeKey!!!!!";
        return HostCodedKey();
    }

    qDebug() << "status: " << resp_frame.cmdStatus;

    if (resp_frame.cmdStatus != 0) {
        return HostCodedKey();
    }

    HostCodedKey coded_key;
    coded_key.ack   = resp_frame.cmdStatus;
    coded_key.coded = resp_frame.params;

    return coded_key;
}


bool MifareReader::doAuth(const AuthKey & auth)
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x18;
    req_frame.params.append( auth.keyType );
    req_frame.params.append( auth.snd );
    req_frame.params.append( auth.keys );
    req_frame.params.append( auth.sector );

    /*memcpy( &(req_frame.params[1]), &(auth->snd[0]),   4  );
    memcpy( &(req_frame.params[5]), &(auth->keys[0]),  12 );
    req_frame.params[17] = auth->sector;
    req_frame.prepareFrame();*/

    io_device()->write( req_frame.packFrame() );

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }

    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponce(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader doAuth!!!!!";
        return false;
    }

    qDebug() << "doAuth finished status: " << resp_frame.cmdStatus;


    return resp_frame.cmdStatus != 0;
}

MifareRead MifareReader::readBlock(int num)
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x19;
    req_frame.params.append(num);

    io_device()->write(req_frame.packFrame());

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }


    QByteArray answ = io_device()->readAll();

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponce(req_frame)) {
        qWarning()<<"GOT ERROR IN MifareReader doAuth!!!!!";
        return MifareRead();
    }

    MifareRead ret;
    ret.ack = resp_frame.cmdStatus;
    ret.data = resp_frame.params;

    return ret;
}
