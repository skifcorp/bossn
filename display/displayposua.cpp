#include "displayposua.h"
#include "settingstool.h"
#include "mifarereader.h"
#include "func.h"

#include <QByteArray>
#include <iostream>

using std::cout;

BossnFactoryRegistrator<DisplayPosua> DisplayPosua::registator("DisplayPosua");
uchar DisplayPosua::frame_ident(0);

DisplayPosua::DisplayPosua(const QVariantMap& p)//:address(0)
{
  /*if (!get_setting("address", p, address) ) {
    qWarning()<<"address for DisplayPosua NOT SET!!!";
  }*/
}

QVariant DisplayPosua::printText(const QVariant & txt)
{
    QString text = txt.toString().toUpper();
    int kvoSymb;

    QByteArray command;
    //очистити 
    command.append(0x0c);
    //друкуємо текст
    if (text.size() > 20){
    kvoSymb = 20;
    }
    else {
    kvoSymb = text.size();
    }
    for ( int i = 0; i < kvoSymb; ++i ) {
 
      int t1 = RusToDisplay(text.at(i));
      //qDebug() << "symB:"<< t1;
      if (t1 == 127)
		command.append( text.at(i) );
	  else	
        command.append( t1 );
    }
    io_device()->write(command);
    return QVariant(true);

}

int DisplayPosua::RusToDisplay(QChar s)
{
   ushort cod;
   cod = (ushort)s.unicode();
   //qDebug() << "ch:" << s << "cod:" << cod;
    //unicod to Shift-JIS
	
   //ураїнська I
   if ( cod == 1030 || cod == 178) {
     return 124;
   } //ураїнська Ї
   else if (cod == 1031 || cod == 175) {
     return 255;
   }
   
   if (cod >= 1040  ) {
       if (cod==1068)
        return cod - 914;
       else
        return cod - 912;
   }
   else if (cod > 127  && cod < 224) {
       if (cod==220)
        return cod - 66;
       else
        return cod - 64;
   }
   else return 127;
}




