#include "mifarereader.h"
#include "crctool.h"
#include "iostream"


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

    ushort calc_crc = doCrc16(arr);

    return crc16 == (ushort)~calc_crc;
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
      if ( (arr[i] == 0xFF) && (arr[i+1] == 0x2) ) {
        ret.append(0xFD); ++i;
      }
      else if ( (arr[i] == 0xFF) && (arr[i+1] == 0x1) ) {
        ret.append(0xFE); ++i;
      }
      else if ( (arr[i] == 0xFF) && (arr[i+1] == 0x0) ) {
        ret.append(0xFF); ++i;
      }
      else {;
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
    qDebug() << "doOn!!!";

    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x10;

    auto arr = req_frame.packFrame();
    cout<<std::hex;
    for ( int i = 0; i<arr.size(); ++i ) {
        cout << (ushort)(uchar)arr[i]<<" ";
    }
    cout << "\n"<<std::dec;

    io_device()->write( arr );

    //const uchar ret_frame_length = 8;

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        qDebug() << "len: " << tmp.length();

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
