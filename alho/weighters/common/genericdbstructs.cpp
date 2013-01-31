#include "genericdbstructs.h"

#include "qxorm_pch.h"
#include "func.h"

#if 0

QX_REGISTER_CPP_EXPORT_DLL(bad_cards)


namespace qx {
    template <> void register_class(QxClass<bad_cards> & t)
    {
        t.id  (&bad_cards::id   , "id");
        t.data(&bad_cards::uid  , "uid");
    }
}

#endif
