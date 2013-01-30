#ifndef CODESHACKS_H
#define CODESHACKS_H

inline int carCodeFromDriver(uint dr)
{
    return dr / 10;
}

inline int kontrCodeFromField( uint f )
{
    return f / 100;
}

#endif // CODESHACKS_H
