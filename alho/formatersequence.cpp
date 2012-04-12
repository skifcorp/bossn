#include "formatersequence.h"
#include "func.h"
#include "mifarecard.h"
#include "mainsequence.h"
#include "settingstool.h"

#include <QByteArray>

void FormaterSequence::start()
{
    QByteArray default_card_code(6, 0xFF);
    QByteArray card_code;

    if (!get_setting("card_code", options, card_code)) return ;

    uint sleepnb_timeout = 100;
    get_setting("sleepnb_timeout", options, sleepnb_timeout);

    uint data_block = 128;
    get_setting("data_block", options, data_block);

    uint password_block = 0;
    if (data_block < 128) {
        password_block = data_block / 4 * 4 + 3;
    }
    else {
        password_block = 128 + (data_block - 128) / 16 * 16 + 15;
    }

    //qDebug() << "pass " << password_block;

    QVector<QByteArray> formated_cards;

    while (true) {
        //qDebug () << "1";

        ActivateCardISO14443A act = tags["reader1"]->func("activateIdleA").value<ActivateCardISO14443A>();
        MifareCard card(tags["reader1"], act);

        if ( !card.active() ) {
            qDebug() << "card not active!!!";

            sleepnb(sleepnb_timeout); continue;
        }

        //qDebug()<< "card active!";

        if (formated_cards.contains(card.uid())) {
            qDebug() << "card already formated: ";
            printByteArray(card.uid());
            sleepnb(sleepnb_timeout); continue;
        }

        if ( !card.autorize(default_card_code, password_block) ) {
            qDebug() << "fail to autorize with default code !!!!: ";//<<(ushort)(uchar)default_card_code[0];

            sleepnb(sleepnb_timeout); continue;
        }

        //card.format();
        //MifareRead r = card.readBlock(127+16);
        MifareRead mr = tags["reader1"]->func("readBlock", Q_ARG(const QVariant&, QVariant(password_block))).value<MifareRead>();
        if (mr.ack != 0) {
            qWarning() << "cant read block for formatting!"; sleepnb(sleepnb_timeout); continue;
        }

        mr.data.replace(0, card_code.length(), card_code);

        if (!tags["reader1"]->func("writeBlock", Q_ARG(const QVariant&, QVariant(password_block)),  Q_ARG(const QByteArray& , mr.data)).toBool() ) {
            qWarning()<<"cant write while formatting!!!";
        }

        formated_cards.append(card.uid());
        sleepnb(sleepnb_timeout);
    }
}
