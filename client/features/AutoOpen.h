//
// Created by root on 8/11/24.
//

#ifndef MCLIB_AUTOOPEN_H
#define MCLIB_AUTOOPEN_H

#include <mclib/core/Connection.h>
#include "QueueManager.h"
#include "AutoBuy.h"

#include "../util/FlipItem.h"
#include "../util/Objects.h"
#include <Colors.h>

class AutoOpen {
public:
    static void OpenAuction(FlipItem item);
};


#endif //MCLIB_AUTOOPEN_H
