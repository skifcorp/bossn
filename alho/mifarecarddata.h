#ifndef MIFARECARDDATA_H
#define MIFARECARDDATA_H

#include <QVariant>
#include <QBitArray>
#include <QDebug>

class MifareCardData : public QVariantMap
{
public:
    MifareCardData(){}
    ~MifareCardData(){}

    template <class T>
    void setMemberValue(const QString& mn, const T& v)
    {
        auto iter = find(mn);
        if (iter == end()) {
            qFatal(  qPrintable("setMemberValue: cant find: " + mn) );
        }

        *iter = QVariant::fromValue<T>(v);
    }

    void setMemberValue(const QString& mn, int bit_num, bool val)
    {
        auto iter = find(mn);
        if (iter == end()) {
            qFatal(  qPrintable("setMemberValue: cant find: " + mn) );
        }

        QBitArray cur_arr = iter->toBitArray();
        cur_arr.setBit(bit_num, val);
        *iter =  cur_arr; //QVariant::fromValue<T>(v);
    }


    template <class T>
    T memberValue(const QString& mn) const
    {
        auto iter = find(mn);
        if (iter == end()) {
            qFatal(  qPrintable("memberValue: cant find: " + mn) );
        }
        return iter->value<T>();
    }

    template <class T>
    bool checkMember(const QString& mn, const T& def_val) const
    {
        return memberValue<T>(mn) != def_val;
    }

    bool checkMember(const QString& mn, float def_val) const
    {
        return !qFuzzyCompare( memberValue<float>(mn) , def_val );
    }

    void clear();

    void setUid(const QByteArray& u) {uid_ = u;}
    QByteArray uid() {return uid_;}
private:
    QByteArray uid_;
};

#endif // MIFARECARDDATA_H
