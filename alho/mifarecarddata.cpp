#include "mifarecarddata.h"
#include "func.h"

void MifareCardData::clear()
{
    const QDateTime zero_date_time = timeShitToDateTime(0);

    setMemberValue("billNumber"    ,0);
    setMemberValue("flags"         ,QBitArray(16));
    setMemberValue("numField"      ,0);
    setMemberValue("realNumField"  ,0);
    setMemberValue("numLoader"     ,0);
    setMemberValue("dateOfLoad"    ,zero_date_time);
    //setMemberValue("driver"        ,0, bill);
    setMemberValue("pointOfAnal"   ,0);

    setMemberValue("bruttoWeight"  ,0);
    setMemberValue("dateOfBrutto"  ,zero_date_time);
    setMemberValue("taraWeight"    ,0);
    //setMemberValue("normTaraWeight",0, bill);
    setMemberValue("dateOfTara"    ,zero_date_time);

    setMemberValue("impurity"      ,0);
    setMemberValue("shugarContent" ,0);
    setMemberValue("greenWeight"   ,0);
    setMemberValue("bum"           ,0);
    setMemberValue("bumFact"       ,0);
    setMemberValue("kagat"         ,0);
    setMemberValue("dateOfUnload"  ,zero_date_time);
}
