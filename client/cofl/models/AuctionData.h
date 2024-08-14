//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_AUCTIONDATA_H
#define MARKETSHARKCPP_AUCTIONDATA_H

#include <string>
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class AuctionData {
private:
    std::string auctionId;
    std::string itemId;

public:
    AuctionData() {}

    AuctionData(const std::string& auctionId, const std::string& itemId)
            : auctionId(auctionId), itemId(itemId) {}

    std::string getAuctionId() const {
        return auctionId;
    }

    void setAuctionId(const std::string& auctionId) {
        this->auctionId = auctionId;
    }

    std::string getItemId() const {
        return itemId;
    }

    void setItemId(const std::string& itemId) {
        this->itemId = itemId;
    }

    friend void to_json(json& j, const AuctionData& a) {
        j = json{{"auctionId", a.auctionId}, {"itemId", a.itemId}};
    }

    friend void from_json(const json& j, AuctionData& a) {
        j.at("auctionId").get_to(a.auctionId);
        j.at("itemId").get_to(a.itemId);
    }
};

#endif //MARKETSHARKCPP_AUCTIONDATA_H
