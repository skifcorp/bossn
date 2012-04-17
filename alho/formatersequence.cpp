#include "formatersequence.h"
#include "func.h"
#include "mifarecard.h"
#include "mainsequence.h"
#include "settingstool.h"
#include "cardstructs.h"

#include <QByteArray>
#include <QBitArray>

void FormaterSequence::start()
{
    QByteArray default_card_code;
    QByteArray card_code;

    if (!get_setting("card_code"        , options, card_code)) return ;
    if (!get_setting("default_card_code", options, default_card_code)) return ;

    uint sleepnb_timeout = 100;
    get_setting("sleepnb_timeout", options, sleepnb_timeout);

    uint data_block = 128;
    get_setting("data_block", options, data_block);

    uint password_block = MifareCard::passwordBlock(data_block);

    QVector<QByteArray> formated_cards;

    while (true) {       
        ActivateCardISO14443A act = tags["reader1"]->func("activateIdleA").value<ActivateCardISO14443A>();
        MifareCard card(tags["reader1"], act);

        if ( !card.active() ) {
            qDebug() << "card not active!!!";

            sleepnb(sleepnb_timeout); continue;
        }

        if (formated_cards.contains(card.uid())) {
            qDebug() << "card already used: ";
            printByteArray(card.uid());
            sleepnb(sleepnb_timeout); continue;
        }

        if ( !card.autorize(default_card_code, password_block) ) {
            qDebug() << "fail to autorize with default code !!!!: ";//<<(ushort)(uchar)default_card_code[0];

            sleepnb(sleepnb_timeout); continue;
        }

        formated_cards.append(card.uid());

        MifareRead mr = tags["reader1"]->func("readBlock", Q_ARG(const QVariant&, QVariant(password_block))).value<MifareRead>();
        if (!mr.result) {
            qWarning() << "cant read block for formatting!"; sleepnb(sleepnb_timeout); continue;
        }

        mr.data.replace(0, card_code.length(), card_code);

        if (!tags["reader1"]->func("writeBlock", Q_ARG(const QVariant&, QVariant(password_block)),
                                                    Q_ARG(const QVariant& , QVariant (mr.data))).toBool() ) {
            qWarning()<<"cant write data while formatting!!!"; sleepnb(sleepnb_timeout); continue;
        }




        bool format_with_default_data;

        if (get_setting("format_with_default_data", options, format_with_default_data, false) && format_with_default_data)  {
            qDebug () << "adding default data";

            //bill_conf(options);
            QVariantMap bill;
            bill["billNumber"] = 111;
            bill["flags"] = QBitArray(16, false);
            bill["numField"] = 1;
            bill["realNumField"] = 2;
            bill["numLoader"] = 1;
            bill["dateOfLoad"] = 11111111;
            bill["driver"] = 1;
            bill["pointOfAnal"] = 1;

            bill["bruttoWeight"] = 10000;
            bill["dateOfBrutto"] = 11111111;
            bill["taraWeight"] = 1000;
            bill["normTaraWeight"] = 1000;
            bill["dateOfTara"] = 11111111;

            bill["impurity"] = 1;
            bill["shugarContent"] = 12;
            bill["greenWeight"] = 1;
            bill["bum"] = 1;
            bill["bumFact"] = 1;
            bill["kagat"] = 1;
            bill["dateOfUnload"] = 11111111;

            card.writeStruct(bill_conf(options), bill);
        }

        sleepnb(sleepnb_timeout);
    }
}
