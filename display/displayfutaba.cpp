#include "displayfutaba.h"
#include "settingstool.h"
#include "mifarereader.h"

#include <QByteArray>
#include <iostream>

using std::cout;

BossnFactoryRegistrator<DisplayFutaba> DisplayFutaba::registator("DisplayFutaba");
uchar DisplayFutaba::frame_ident(0);

DisplayFutaba::DisplayFutaba(const QVariantMap& p):address(0)
{
  if (!get_setting("address", p, address) ) {
    qWarning()<<"address for DisplayFutaba NOT SET!!!";
  }
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
  command.append(0x34);
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
      command.append(0x35);
      command.append( t1 );
    }
    else { //російський текст
      //Double size off
      command.append(0x1b);
      command.append(0x23);
      command.append(0x35);
      command.append(0x84);
      //qDebug()<<t1<<" cod "<<cr1<<" sumvol ";
      command.append( t1 );
    }
    if ( kvo == 10 ) {
      command.append(0xA);
      command.append(0xD);
    }
    else if ( kvo == 26 ) {
      command.append(0xA);
      command.append(0xD);
    }
  }
  io_device()->write(command);
  //Sleep(150); //пакишо такий спосіб уникнення помилки при частому записі в порт без цього помилка CommEvent overlapped write error: 170
  return QVariant(true);
}

int DisplayFutaba::RusToFutaba(QChar s)
{
  ushort cod;
  cod = (ushort)s.unicode();
  //unicod to Shift-JIS

  if ( cod == 1040 ) { //U+0410	А	d090  1040  cod  'А'
    return 64;
  }
  else if ( cod == 1041 ) { // U+0411 Б	d091 1041  cod  'Б'
    return 65;
  }
  else if ( cod == 1042 ) { //U+0412	В	d092	 3 1042  cod  'В'
    return 66;
  }
  else if ( cod == 1043 ) { //U+0413	Г	d093	 4 1043  cod  'Г'
    return 67;
  }
  else if ( cod == 1044 ) { //  U+0414	Д	d094	 5  1044  cod  'Д'
    return 68;
  }
  else if ( cod == 1045 ) { //U+0415	Е	d095	E 6  1045  cod  'Е'
    return 69;
  }
  else if ( cod == 1046 ) { //U+0416	Ж	d096	 7  1046  cod  'Ж'
    return 71;
  }
  else if ( cod == 1047 ) { //U+0417	З	d097	 8  1047  cod  'З'
    return 72;
  }
  else if ( cod == 1048 ) { //U+0418	И	d098	 9  1048  cod  'И'
    return 73;
  }
  else if ( cod == 1049 ) { //U+0419	Й	d099	 10 1049  cod  'Й'
    return 74;
  }
  else if ( cod == 1050 ) { //U+041A	К	d0 9a	 11   1050  cod  'К'
    return 75;
  }
  else if ( cod == 1051 ) { //U+041B	Л	d0 9b	 12  1051  cod  'Л'
    return 76;
  }
  else if ( cod == 1052 ) { //U+041C	М	d0 9c	 13  1052  cod  'М'
    return 77;
  }
  else if ( cod == 1053 ) { //U+041D	Н	d0 9d	 14  1053  cod  'Н'
    return 78;
  }
  else if ( cod == 1054 ) { //U+041E	О	d0 9e	 15  1054  cod  'О'
    return 79;
  }
  else if ( cod == 1055 ) { //U+041F	П	d0 9f	 16  1055  cod  'П'
    return 80;
  }
  else if ( cod == 1056 ) { //U+0420	Р	d0 a0	 17  1056  cod  'Р'
    return 81;
  }
  else if ( cod == 1057 ) { //U+0421	С	d0 a1	 18  1057  cod  'С'
    return 82;
  }
  else if ( cod == 1058 ) { //U+0422	Т	d0 a2	 19  1058  cod  'Т'
    return 83;
  }
  else if ( cod == 1059 ) { //U+0423	У	d0 a3	 20  1059  cod  'У'
    return 84;
  }
  else if ( cod == 1060 ) { //U+0424	Ф	d0 a4	 21  1060  cod  'Ф'
    return 85;
  }
  else if ( cod == 1061 ) { //U+0425	Х	d0 a5	 22  1061  cod  'Х'
    return 86;
  }
  else if ( cod == 1062 ) { //U+0426	Ц	d0 a6	 23  1062  cod  'Ц'
    return 87;
  }
  else if ( cod == 1063 ) { //U+0427	Ч	d0 a7	 24  1063  cod  'Ч'
    return 88;
  }
  else if ( cod == 1064 ) { //U+0428	Ш	d0 a8	 25  1064  cod  'Ш'
    return 89;
  }
  else if ( cod == 1065 ) { //U+0429	Щ	d0 a9	 26  1065  cod  'Щ'
    return 90;
  }
  else if ( cod == 1066 ) { //U+042A	Ъ	d0 aa	 27  1066  cod  'Ъ'
    return 91;
  }
  else if ( cod == 1067 ) { //U+042B	Ы	d0 ab	 28  1067  cod  'Ы'
    return 92;
  }
  else if ( cod == 1068 ) { //U+042C	Ь	d0 ac	 29 1068  cod  'Ь'
    return 93;
  }
  else if ( cod == 1069 ) { //U+042D	Э	d0 ad	 30  1069  cod  'Э'
    return 94;
  }
  else if ( cod == 1070 ) { //U+042E	Ю	d0 ae	 31 1070  cod  'Ю'
    return 95;
  }
  else if ( cod == 1071 ) { //U+042F	Я	d0 af	 32 1071  cod  'Я'
    return 96;
  }
  else if ( cod == 1072 ) { //U+0430	а	d0 b0	 33  1072  cod  'а'
    return 112;
  }
  else if ( cod == 1073 ) { //U+0431	б	d0 b1	 34 1073  cod  'б'
    return 113;
  }
  else if ( cod == 1074 ) { //U+0432	в	d0 b2	 35  1074  cod  'в'
    return 114;
  }
  else if ( cod == 1075 ) { //U+0433	г	d0 b3	 36   1075  cod  'г'
    return 115;
  }
  else if ( cod == 1076 ) { //U+0434	д	d0 b4	 37    1076  cod  'д'
    return 116;
  }
  else if ( cod == 1077 ) { //U+0435	е	d0 b5	 38  1077  cod  'е'
    return 117;
  }
  else if ( cod == 1078 ) { //U+0436	ж	d0 b6	 39  1078  cod  'ж'
    return 119;
  }
  else if ( cod == 1079 ) { //U+0437	з	d0 b7	 40  1079  cod  'з'
    return 120;
  }
  else if ( cod == 1080 ) { //U+0438	и	d0 b8	 41  1080  cod  'и'
    return 121;
  }
  else if ( cod == 1081 ) { //U+0439	й	d0 b9	 42  1081  cod  'й'
    return 122;
  }
  else if ( cod == 1082 ) { //U+043A	к	d0 ba	 43  1082  cod  'к'
    return 123;
  }
  else if ( cod == 1083 ) { //U+043B	л	d0 bb	 44  1083  cod  'л'
    return 124;
  }
  else if ( cod == 1084 ) { //U+043C	м	d0 bc	 45  1084  cod  'м'
    return 125;
  }
  else if ( cod == 1085 ) { //U+043D	н	d0 bd	 46  1085  cod  'н'
    return 126;
  }
  else if ( cod == 1086 ) { //U+043E	о	d0 be	 47  1086  cod  'о'
    return 128;
  }
  else if ( cod == 1087 ) { //U+043F	п	d0 bf	 48  1087  cod  'п'
    return 129;
  }
  else if ( cod == 1088 ) { //U+0440	р	d1 80	 49  1088  cod  'р'
    return 130;
  }
  else if ( cod == 1089 ) { //U+0441	с	d1 81	 50  1089  cod  'с'
    return 131;
  }
  else if ( cod == 1090 ) { //U+0442	т	d1 82	 51  1090  cod  'т'
    return 132;
  }
  else if ( cod == 1091 ) { //U+0443	у	d1 83	 52  1091  cod  'у'
    return 133;
  }
  else if ( cod == 1092 ) { //U+0444	ф	d1 84	 53  1092  cod  'ф'
    return 134;
  }
  else if ( cod == 1093 ) { //U+0445	х	d1 85	 54  1093  cod  'х'
    return 135;
  }
  else if ( cod == 1094 ) { //U+0446	ц	d1 86	 55  1094  cod  'ц'
    return 136;
  }
  else if ( cod == 1095 ) { //U+0447	ч	d1 87	 56  1095  cod  'ч'
    return 137;
  }
  else if ( cod == 1096 ) { //U+0448	ш	d1 88	 57  1096  cod  'ш'
    return 138;
  }
  else if ( cod == 1097 ) { //U+0449	щ	d1 89	 58  1097  cod  'щ'
    return 139;
  }
  else if ( cod == 1098 ) { //U+044A	ъ	d1 8a	 59  1098  cod  'ъ'
    return 140;
  }
  else if ( cod == 1099 ) { //U+044B	ы	d1 8b	 60  1099  cod  'ы'
    return 141;
  }
  else if ( cod == 1100 ) { //U+044C	ь	d1 8c	 61  1100  cod  'ь'
    return 142;
  }
  else if ( cod == 1101 ) { //U+044D	э	d1 8d	 62  1101  cod  'э'
    return 143;
  }
  else if ( cod == 1102 ) { //U+044E	ю	d1 8e	 63 1102  cod  'ю'
    return 144;
  }
  else if ( cod == 1103 ) { //U+044F	я	d1 8f	 64 1103  cod  'я'
    return 145;
  }
  //ураїнська і
  else if ( cod == 1110 ) {//1110  cod  'і'
    return 105;
    //return 1110;
  }
  /*
  else if ( cod == 1030 ) {//1110  cod  'і'
    return 105;
    //return 1110;
  }
  */
  else return 127;
}




