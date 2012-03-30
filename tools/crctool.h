#ifndef CRCTOOL_H
#define CRCTOOL_H

#include <QByteArray>

ushort doCrc16(const QByteArray& buff, int start_byte = 0)
{
  ushort CRC = 0xFFFF;
  for ( int j = start_byte; j< buff.length(); ++j ) {
    uchar i = 0;
    ushort w = 0;
    w = ( buff[j] ^ CRC) & 0xFF;
    i = 8;
    do {
          if ( w & 1 )
          {
               w >>= 1;
               w ^= 0x8408;
          }
          else
          {
               w >>= 1;
          }
     } while( --i );
     CRC = w ^ ( CRC >> 8 );
  }

  return CRC;
}

#endif // CRCTOOL_H
