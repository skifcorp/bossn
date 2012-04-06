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
  //�������� �����
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
    if ( t1 == 127 ) { //���������� �����
      //Double size on
      command.append(0x1b);
      command.append(0x23);
      command.append(0x36);
      command.append(text.at(i));
    }
    else if ( t1 == 105 ) { // ��� ��� ���� "i"
      command.append(0x1b);
      command.append(0x23);
      command.append(0x35);
      command.append( t1 );
    }
    else { //��������� �����
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
  //Sleep(150); //������ ����� ����� ��������� ������� ��� ������� ����� � ���� ��� ����� ������� CommEvent overlapped write error: 170
  return QVariant(true);
}

int DisplayFutaba::RusToFutaba(QChar s)
{
  ushort cod;
  cod = (ushort)s.unicode();
  //unicod to Shift-JIS

  if ( cod == 1040 ) { //U+0410	�	d090  1040  cod  '�'
    return 64;
  }
  else if ( cod == 1041 ) { // U+0411 �	d091 1041  cod  '�'
    return 65;
  }
  else if ( cod == 1042 ) { //U+0412	�	d092	 3 1042  cod  '�'
    return 66;
  }
  else if ( cod == 1043 ) { //U+0413	�	d093	 4 1043  cod  '�'
    return 67;
  }
  else if ( cod == 1044 ) { //  U+0414	�	d094	 5  1044  cod  '�'
    return 68;
  }
  else if ( cod == 1045 ) { //U+0415	�	d095	E 6  1045  cod  '�'
    return 69;
  }
  else if ( cod == 1046 ) { //U+0416	�	d096	 7  1046  cod  '�'
    return 71;
  }
  else if ( cod == 1047 ) { //U+0417	�	d097	 8  1047  cod  '�'
    return 72;
  }
  else if ( cod == 1048 ) { //U+0418	�	d098	 9  1048  cod  '�'
    return 73;
  }
  else if ( cod == 1049 ) { //U+0419	�	d099	 10 1049  cod  '�'
    return 74;
  }
  else if ( cod == 1050 ) { //U+041A	�	d0 9a	 11   1050  cod  '�'
    return 75;
  }
  else if ( cod == 1051 ) { //U+041B	�	d0 9b	 12  1051  cod  '�'
    return 76;
  }
  else if ( cod == 1052 ) { //U+041C	�	d0 9c	 13  1052  cod  '�'
    return 77;
  }
  else if ( cod == 1053 ) { //U+041D	�	d0 9d	 14  1053  cod  '�'
    return 78;
  }
  else if ( cod == 1054 ) { //U+041E	�	d0 9e	 15  1054  cod  '�'
    return 79;
  }
  else if ( cod == 1055 ) { //U+041F	�	d0 9f	 16  1055  cod  '�'
    return 80;
  }
  else if ( cod == 1056 ) { //U+0420	�	d0 a0	 17  1056  cod  '�'
    return 81;
  }
  else if ( cod == 1057 ) { //U+0421	�	d0 a1	 18  1057  cod  '�'
    return 82;
  }
  else if ( cod == 1058 ) { //U+0422	�	d0 a2	 19  1058  cod  '�'
    return 83;
  }
  else if ( cod == 1059 ) { //U+0423	�	d0 a3	 20  1059  cod  '�'
    return 84;
  }
  else if ( cod == 1060 ) { //U+0424	�	d0 a4	 21  1060  cod  '�'
    return 85;
  }
  else if ( cod == 1061 ) { //U+0425	�	d0 a5	 22  1061  cod  '�'
    return 86;
  }
  else if ( cod == 1062 ) { //U+0426	�	d0 a6	 23  1062  cod  '�'
    return 87;
  }
  else if ( cod == 1063 ) { //U+0427	�	d0 a7	 24  1063  cod  '�'
    return 88;
  }
  else if ( cod == 1064 ) { //U+0428	�	d0 a8	 25  1064  cod  '�'
    return 89;
  }
  else if ( cod == 1065 ) { //U+0429	�	d0 a9	 26  1065  cod  '�'
    return 90;
  }
  else if ( cod == 1066 ) { //U+042A	�	d0 aa	 27  1066  cod  '�'
    return 91;
  }
  else if ( cod == 1067 ) { //U+042B	�	d0 ab	 28  1067  cod  '�'
    return 92;
  }
  else if ( cod == 1068 ) { //U+042C	�	d0 ac	 29 1068  cod  '�'
    return 93;
  }
  else if ( cod == 1069 ) { //U+042D	�	d0 ad	 30  1069  cod  '�'
    return 94;
  }
  else if ( cod == 1070 ) { //U+042E	�	d0 ae	 31 1070  cod  '�'
    return 95;
  }
  else if ( cod == 1071 ) { //U+042F	�	d0 af	 32 1071  cod  '�'
    return 96;
  }
  else if ( cod == 1072 ) { //U+0430	�	d0 b0	 33  1072  cod  '�'
    return 112;
  }
  else if ( cod == 1073 ) { //U+0431	�	d0 b1	 34 1073  cod  '�'
    return 113;
  }
  else if ( cod == 1074 ) { //U+0432	�	d0 b2	 35  1074  cod  '�'
    return 114;
  }
  else if ( cod == 1075 ) { //U+0433	�	d0 b3	 36   1075  cod  '�'
    return 115;
  }
  else if ( cod == 1076 ) { //U+0434	�	d0 b4	 37    1076  cod  '�'
    return 116;
  }
  else if ( cod == 1077 ) { //U+0435	�	d0 b5	 38  1077  cod  '�'
    return 117;
  }
  else if ( cod == 1078 ) { //U+0436	�	d0 b6	 39  1078  cod  '�'
    return 119;
  }
  else if ( cod == 1079 ) { //U+0437	�	d0 b7	 40  1079  cod  '�'
    return 120;
  }
  else if ( cod == 1080 ) { //U+0438	�	d0 b8	 41  1080  cod  '�'
    return 121;
  }
  else if ( cod == 1081 ) { //U+0439	�	d0 b9	 42  1081  cod  '�'
    return 122;
  }
  else if ( cod == 1082 ) { //U+043A	�	d0 ba	 43  1082  cod  '�'
    return 123;
  }
  else if ( cod == 1083 ) { //U+043B	�	d0 bb	 44  1083  cod  '�'
    return 124;
  }
  else if ( cod == 1084 ) { //U+043C	�	d0 bc	 45  1084  cod  '�'
    return 125;
  }
  else if ( cod == 1085 ) { //U+043D	�	d0 bd	 46  1085  cod  '�'
    return 126;
  }
  else if ( cod == 1086 ) { //U+043E	�	d0 be	 47  1086  cod  '�'
    return 128;
  }
  else if ( cod == 1087 ) { //U+043F	�	d0 bf	 48  1087  cod  '�'
    return 129;
  }
  else if ( cod == 1088 ) { //U+0440	�	d1 80	 49  1088  cod  '�'
    return 130;
  }
  else if ( cod == 1089 ) { //U+0441	�	d1 81	 50  1089  cod  '�'
    return 131;
  }
  else if ( cod == 1090 ) { //U+0442	�	d1 82	 51  1090  cod  '�'
    return 132;
  }
  else if ( cod == 1091 ) { //U+0443	�	d1 83	 52  1091  cod  '�'
    return 133;
  }
  else if ( cod == 1092 ) { //U+0444	�	d1 84	 53  1092  cod  '�'
    return 134;
  }
  else if ( cod == 1093 ) { //U+0445	�	d1 85	 54  1093  cod  '�'
    return 135;
  }
  else if ( cod == 1094 ) { //U+0446	�	d1 86	 55  1094  cod  '�'
    return 136;
  }
  else if ( cod == 1095 ) { //U+0447	�	d1 87	 56  1095  cod  '�'
    return 137;
  }
  else if ( cod == 1096 ) { //U+0448	�	d1 88	 57  1096  cod  '�'
    return 138;
  }
  else if ( cod == 1097 ) { //U+0449	�	d1 89	 58  1097  cod  '�'
    return 139;
  }
  else if ( cod == 1098 ) { //U+044A	�	d1 8a	 59  1098  cod  '�'
    return 140;
  }
  else if ( cod == 1099 ) { //U+044B	�	d1 8b	 60  1099  cod  '�'
    return 141;
  }
  else if ( cod == 1100 ) { //U+044C	�	d1 8c	 61  1100  cod  '�'
    return 142;
  }
  else if ( cod == 1101 ) { //U+044D	�	d1 8d	 62  1101  cod  '�'
    return 143;
  }
  else if ( cod == 1102 ) { //U+044E	�	d1 8e	 63 1102  cod  '�'
    return 144;
  }
  else if ( cod == 1103 ) { //U+044F	�	d1 8f	 64 1103  cod  '�'
    return 145;
  }
  //�������� �
  else if ( cod == 1110 ) {//1110  cod  '�'
    return 105;
    //return 1110;
  }
  /*
  else if ( cod == 1030 ) {//1110  cod  '�'
    return 105;
    //return 1110;
  }
  */
  else return 127;
}




