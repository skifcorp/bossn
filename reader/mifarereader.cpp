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
    return crc16 == (uchar)~calc_crc;
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

    startCondition   = arr[0];
    address          = arr[1];
    ident            = arr[2];
    cmdCode          = arr[3];
    cmdStatus        = arr[4];
    //crc16            = *static_cast<ushort*>(&arr[ arr.length() - 3 ]);
    crc16            = *reinterpret_cast<ushort*>(&arr.data()[arr.length() - 3]);
    finishCondition  = arr[7];

    params = arr.mid(5, arr.length() - 8);

    return true;
}

bool MifareResponseFrame::checkResponce(const MifareRequestFrame & req)
{
    if (req.startCondition != startCondition) {
        return false;
    }

    if (req.address != address) {
        return false;
    }

    if (req.ident != ident) {
        return false;
    }

    if (req.cmdCode != cmdCode) {
        return false;
    }

    if (req.finishCondition !=  finishCondition) {
        return false;
    }

    if (!checkCrc()) {
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
