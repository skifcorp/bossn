#include "displaycaptain.h"
#include "settingstool.h"
#include "mifarereader.h"

#include <QByteArray>

#include <iostream>

using std::cout;

BossnFactoryRegistrator<DisplayCaptain> DisplayCaptain::registator("DisplayCaptain");
uchar DisplayCaptain::frame_ident(0);



DisplayCaptain::DisplayCaptain(const QVariantMap& p):address(0)
{
    address = get_setting<uchar>("address", p);
}

QVariant DisplayCaptain::printText(const QVariant & txt)
{
    MifareRequestFrame req_frame;

    req_frame.address  = address;
    req_frame.ident    = frame_ident++;
    req_frame.cmdCode  = 0x82;

    req_frame.params.append('0');
    req_frame.params.append('0');
    req_frame.params.append(txt.toString().toAscii());

    io_device()->write( req_frame.packFrame() );

    const uchar ret_frame_length = 8;

    while (true) {
        QByteArray tmp = io_device()->peek(MifareRequestFrame::paramsBuffLen);
        if (static_cast<uchar>(tmp[tmp.length() - 1]) == MifareRequestFrame::finishCondition ) break;

        yield();
    }

    QByteArray answ = io_device()->read(ret_frame_length);

    MifareResponseFrame resp_frame;

    resp_frame.unpackFrame(answ);

    if (!resp_frame.checkResponse(req_frame)) {
        qWarning()<<"GOT ERROR IN TABLO RESPONCE!!!!!";
        return QVariant(false);
    }
    return QVariant(true);
}
