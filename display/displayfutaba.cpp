#include "displayfutaba.h"
#include "settingstool.h"
#include "mifarereader.h"
#include "func.h"

#include <QByteArray>
#include <iostream>

using std::cout;

BossnFactoryRegistrator<DisplayFutaba> DisplayFutaba::registator("DisplayFutaba");
uchar DisplayFutaba::frame_ident(0);

DisplayFutaba::DisplayFutaba(const QVariantMap& p)//:address(0)
{
  /*if (!get_setting("address", p, address) ) {
    qWarning()<<"address for DisplayFutaba NOT SET!!!";
  }*/
}

QVariant DisplayFutaba::printText(const QVariant & txt)
{
    QString text = txt.toString();
    int kvo;
    //screen mode
    QByteArray command;
    //очистити екран
    command.append(0x1b);
    command.append(0x5b);
    command.append(0x32);
    command.append(0x4a);
    //Screen Mode
    command.append(0x1b);
    command.append(0x5c);
    command.append(0x3f);
    command.append(0x4c);
    command.append(0x53);
    command.append(0x32); //32 35
    //Kursor Home Position
    command.append(0x1b);
    command.append(0x5b);
    command.append(0x48);
    command.append(0x27);
    kvo = 0;
    for ( int i = 0; i < text.length(); ++i ) {
      int t1 = RusToFutaba(text.at(i));
      kvo = kvo + 1;
      if ( t1 == 127 ) { //англійський текст
        //Double size on
        command.append(0x1b);
        command.append(0x23);
        command.append(0x36);
        command.append(text.at(i));
      }
      else if ( t1 == 105 ) { // для укр букв "i"
        command.append(0x1b);
        command.append(0x23);
        command.append(0x36);
        command.append( t1 );
      }
      else { //російський текст
        //Double size off
        command.append(0x1b);
        command.append(0x23);
        command.append(0x35);
        command.append(0x84);
        command.append( t1 );
      }
      if ( kvo == 16 ) {
        command.append(0xA);
        command.append(0xD);
      }
      else if ( kvo == 32 ) {
        command.append(0xA);
        command.append(0xD);
      }
      else if ( kvo == 48 ) {
        command.append(0xA);
        command.append(0xD);
      }
      else if ( kvo == 64 ) {
        command.append(0xA);
        command.append(0xD);
      }
    }
    io_device()->write(command);
    return QVariant(true);

}

int DisplayFutaba::RusToFutaba(QChar s)
{
   ushort cod;
   cod = (ushort)s.unicode();
    //unicod to Shift-JIS
   if (cod >= 1040 && cod < 1046 ) {
     return cod - 976;
   }
   else if (cod >= 1046 && cod < 1072 ) {
     return cod - 975;
   }
   else if (cod >= 1072 && cod < 1078 ) {
     return cod - 960;
   }
   else if (cod >= 1078 && cod < 1085 ) {
     return cod - 959;
   }
   else if (cod >= 1085 && cod < 1104 ) {
     return cod - 958;
   }
   //ураїнська і
   else if ( cod == 1110 ) {
     return 105;
   }
   else return 127;
}




