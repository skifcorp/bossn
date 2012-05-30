#ifndef CODESHACKS_H
#define CODESHACKS_H


inline uint carCodeFromDriver(uint dr)
{
    return dr / 10;
}

inline uint kontrCodeFromField( uint f )
{
    return f / 100;
}

#endif // CODESHACKS_H
