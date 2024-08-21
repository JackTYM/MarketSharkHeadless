//
// Created by jack on 8/13/24.
//

#ifndef MCLIB_FLIPITEM_H
#define MCLIB_FLIPITEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "mclib/common/Json.h"
#include "mclib/inventory/Slot.h"

#include <locale>
#include <codecvt>
#include <chrono>

class FlipItem {
public:
    static std::unordered_map<std::string, FlipItem> flipMap;
    static std::vector<FlipItem> flipItems;

    std::string displayName;
    std::string strippedDisplayName;
    mc::inventory::Slot* slot;
    std::string uuid;
    long buyPrice = 0;
    long coflWorth = 0;
    long sellPrice = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> buyTime;
    int buySpeed = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> auctionStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> purchaseAt;
    std::string auctionId;
    std::string username;
    std::string sellerUuid;
    std::string skyblockId;
    bool bed = false;
    std::string buyer;
    bool sold = false;
    bool bought = false;
    std::string skipReason;
    int buyClicks = 0;
    bool closed = false;
    std::string finder;
    bool bedClicking = false;
    //bool skipped = false;

    FlipItem();

    static FlipItem getFlipItem(mc::inventory::Slot* slot);
    void setSlot(mc::inventory::Slot* slot);
    nlohmann::json serialize() const;
    static std::string getUuid(mc::inventory::Slot* stack);
    static void saveFlipData();
    static void loadFlipData();
    static std::string getItemStrings();
};


#endif //MCLIB_FLIPITEM_H
