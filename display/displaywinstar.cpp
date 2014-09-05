#include "displaywinstar.h"
#include "settingstool.h"
#include "func.h"

#include <QByteArray>
#include <iostream>

using std::cout;

BossnFactoryRegistrator<DisplayWinstar> DisplayWinstar::registator("DisplayWinstar");
uchar DisplayWinstar::frame_ident(0);

DisplayWinstar::DisplayWinstar(const QVariantMap& p)
{
  /*if (!get_setting("address", p, address) ) {
    qWarning()<<"address for DisplayWinstar NOT SET!!!";
  }*/

}

QVariant DisplayWinstar::printText(const QVariant & txt)
{
    QString text = txt.toString().toUpper();
    QByteArray command;
    
    command.append(0xe7);
    command.append(0x42);
    command.append(0x01);
    command.append(0x7e);
    io_device()->write(command);
    //QByteArray answ = io_device()->readAll();
    command.clear();
      while ( io_device()->bytesAvailable() < 4 ) {
        yield();
    }
    QByteArray answ = io_device()->readAll();
    answ.clear();
    command.append(0xe7);
    command.append(0x43);
    command.append(0x01);
    command.append(0x7e);
    io_device()->write(command);
    //QByteArray answ2 = io_device()->readAll();
    command.clear();
      while ( io_device()->bytesAvailable() < 4 ) {
        yield();
    }
    answ = io_device()->readAll();
    answ.clear();
    //char* str_c = text.toAscii().data();
    QByteArray str_c = text.toAscii();
    for (int i = 0; i < text.length(); ++i) {
        command.append(0xe7);
        command.append(0x44);
        int num = text[i].toAscii();

        if (num>0){
            command.append(str_c[i]);
            //qDebug() << "num = " << num;
            //qDebug() << "str_c[i] = " << str_c[i];
        }
        else{
            command.append(RusToDisplay(num));
        }
        command.append(0x7e);
        io_device()->write(command);
        command.clear();
        while ( io_device()->bytesAvailable() < 4 ) {
          yield();
          }
        answ = io_device()->readAll();
        answ.clear();

    }

       return QVariant(true);

}

int DisplayWinstar::RusToDisplay(int symv)
{
    if (symv==-64)      
        return 0x41;
    else if (symv==-63) 
        return 0xA0;
    else if (symv==-62) 
        return 0x42;
    else if (symv==-61) 
        return 0xA1;
    else if (symv==-60) 
        return 0xE0;
    else if (symv==-59) 
        return 0x45;
    else if (symv==-58) 
        return 0xA3;
    else if (symv==-57) 
        return 0xA4;
    else if (symv==-56) 
        return 0xA5;
    else if (symv==-55) 
        return 0xA6;
    else if (symv==-54) 
        return 0x4B;
    else if (symv==-53) 
        return 0xA7;
    else if (symv==-52) 
        return 0x4D;
    else if (symv==-51) 
        return 0x48;
    else if (symv==-50) 
        return 0x4F;
    else if (symv==-49) 
        return 0xA8;
    else if (symv==-48)  
        return 0x50;
    else if (symv==-47) 
        return 0x43;
    else if (symv==-46) 
        return 0x54;
    else if (symv==-45) 
        return 0xA9;
    else if (symv==-44) 
        return 0xAA;
    else if (symv==-43) 
        return 0x58;
    else if (symv==-42) 
        return 0xE1;
    else if (symv==-41) 
        return 0xAB;
    else if (symv==-40) 
        return 0xAC;
    else if (symv==-39) 
        return 0xE2;
    else if (symv==-38) 
        return 0xAD;
    else if (symv==-37) 
        return 0xAE;
    else if (symv==-36) 
        return 0x62;
    else if (symv==-35) 
        return 0xAF;
    else if (symv==-34) 
        return 0xB0;
    else if (symv==-33) 
        return 0xB1;
    else if (symv==-86) 
        return 0x45;
    else if (symv==-81) 
        return 0x49;
    else if (symv==-78) 
        return 0x49;
    else if (symv==-88) 
        return 0xA2;
    else
    {   qDebug() << symv;
        return 0x20;
    }
}




