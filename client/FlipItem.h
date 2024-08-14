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
#include <mclib/common/Json.h>
#include <mclib/inventory/Slot.h>

#include <locale>
#include <codecvt>

class FlipItem {
public:
    static std::unordered_map<std::string, FlipItem> flipMap;
    static std::vector<FlipItem> flipItems;

    std::string displayName;
    std::string strippedDisplayName;
    mc::inventory::Slot* slot;
    std::string uuid;
    long buyPrice;
    long coflWorth;
    long sellPrice;
    long startTime;
    long buyTime;
    int buySpeed;
    long auctionStart;
    std::string auctionId;
    std::string username;
    std::string sellerUuid;
    std::string skyblockId;
    bool bed;
    std::string buyer;
    bool sold;
    bool bought;
    std::string skipReason;
    int buyClicks;
    bool closed;
    std::string finder;
    bool bedClicking;
    bool skipped;

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
