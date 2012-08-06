#include "genericdbstructs.h"

#include <QxOrm.h>
#include <QxMemLeak.h>
#include "func.h"

QX_REGISTER_CPP_EXPORT_DLL(bad_cards)


namespace qx {
    template <> void register_class(QxClass<bad_cards> & t)
    {
        t.id  (&bad_cards::id   , "id");
        t.data(&bad_cards::uid  , "uid");
    }
}

