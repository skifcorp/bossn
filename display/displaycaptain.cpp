#include "displaycaptain.h"
#include "settingstool.h"
#include "mifarereader.h"

#include <QByteArray>
#include <QTextCodec>

#include <iostream>

using std::cout;

BossnFactoryRegistrator<DisplayCaptain> DisplayCaptain::registator("DisplayCaptain");
uchar DisplayCaptain::frame_ident(0);



const QString DisplayCaptain::end_of_str = " " + QString(QChar::fromAscii(0x02)) + "600 ";

DisplayCaptain::DisplayCaptain(const QVariantMap& p):address(0)
{
    address = get_setting<uchar>("address", p);
}

QVariant DisplayCaptain::printText(const QVariant & txt)
{    
    //qDebug() << "Display 1 begin{ ";
    MifareRequestFrame txt_req = printTextFrame(txt.toString() + end_of_str);

    io_device()->write( txt_req.packFrame() );

    if ( !processAnswer(txt_req) ) {
        qWarning() << "fail with write text";
        return QVariant(false);
    }

    MifareRequestFrame scroll_req = scrollTextFrame();

    io_device()->write( scroll_req.packFrame() );

    if (!processAnswer(scroll_req)) {
        qWarning() << "fail with scroll text";
        return QVariant(false);
    }

    //qDebug() << "} Display 1 end";

    return QVariant(true);
}


MifareRequestFrame DisplayCaptain::printTextFrame(const QString& txt) const
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x80;

    req_frame.params.append('0');
    req_frame.params.append('0');
    req_frame.params.append( QTextCodec::codecForCStrings()->fromUnicode(txt) );

    return req_frame;
}

MifareRequestFrame DisplayCaptain::scrollTextFrame() const
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x82;

    req_frame.params.append('0');
    req_frame.params.append('0');

    return req_frame;
}

bool DisplayCaptain::processAnswer(const MifareRequestFrame &req_frame)
{
    //const uchar ret_frame_length = 8;

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (tmp.isEmpty() || static_cast<uchar>(tmp.right(1)[0])!= MifareRequestFrame::finishCondition)
            yield();
        else
            break;
    }

    QByteArray answ = io_device()->readAll();//ret_frame_length);

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"GOT ERROR IN TABLO RESPONCE!!!!!";
        return false;
    }
    return true;
}
