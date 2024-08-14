//
// Created by root on 8/11/24.
//

#ifndef MCLIB_AUTOOPEN_H
#define MCLIB_AUTOOPEN_H

#include <mclib/core/Connection.h>
#include "QueueManager.h"

class AutoOpen {
private:
    static mc::core::Connection* m_Connection;
public:
    MCLIB_API AutoOpen(mc::core::Connection* connection);

    static void OpenAuction(std::string message);
};


#endif //MCLIB_AUTOOPEN_H
