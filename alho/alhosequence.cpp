#include "alhosequence.h"

#include <QDebug>

AlhoSequence::AlhoSequence(const QString& n): Coroutine2( n.toStdString() )
{

}
