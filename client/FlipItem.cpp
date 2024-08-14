#include "FlipItem.h"
std::unordered_map<std::string, FlipItem> FlipItem::flipMap;
std::vector<FlipItem> FlipItem::flipItems;

FlipItem::FlipItem() {
    flipItems.push_back(*this);
    //this->username = Main::mc.getSession().getUsername();
}

FlipItem FlipItem::getFlipItem(mc::inventory::Slot* slot) {
    std::string uuid = getUuid(slot);
    if (!uuid.empty()) {
        auto it = flipMap.find(uuid);
        if (it != flipMap.end()) {
            return it->second;
        }
    }
    FlipItem item;
    item.setSlot(slot);
    return item;
}

void FlipItem::setSlot(mc::inventory::Slot* slot) {
    try {
        this->slot = slot;
        //this->displayName = this->slot->GetNBT().GetRoot().
        //this->strippedDisplayName = ChatUtils::stripColor(this->displayName);
        this->uuid = getUuid(this->slot);
        flipMap[uuid] = *this;
    } catch (const std::exception& e) {
        //BugLogger::logError(e.what());
    }
}

nlohmann::json FlipItem::serialize() const {
    nlohmann::json jsonObject;
    jsonObject["displayName"] = this->displayName;
    jsonObject["strippedDisplayName"] = this->strippedDisplayName;
    jsonObject["uuid"] = this->uuid;
    jsonObject["buyPrice"] = this->buyPrice;
    jsonObject["sellPrice"] = this->sellPrice;
    jsonObject["coflWorth"] = this->coflWorth;
    jsonObject["startTime"] = this->startTime;
    jsonObject["buyTime"] = this->buyTime;
    jsonObject["buySpeed"] = this->buySpeed;
    jsonObject["auctionStart"] = this->auctionStart;
    jsonObject["auctionId"] = this->auctionId;
    jsonObject["username"] = this->username;
    jsonObject["sellerUuid"] = this->sellerUuid;
    jsonObject["skyblockId"] = this->skyblockId;
    jsonObject["bed"] = this->bed;
    jsonObject["buyer"] = this->buyer;
    jsonObject["sold"] = this->sold;
    jsonObject["bought"] = this->bought;
    jsonObject["skipReason"] = this->skipReason;
    jsonObject["sendBought"] = true;
    jsonObject["sendSold"] = true;
    jsonObject["finder"] = this->finder;
    return jsonObject;
}

std::string FlipItem::getUuid(mc::inventory::Slot* stack) {
    if (stack != nullptr) {
        auto uuidTag = stack->GetNBT().GetTag<mc::nbt::TagCompound>(L"tag")->GetTag<mc::nbt::TagCompound>(L"ExtraAttributes")->GetTag<mc::nbt::Tag>(L"uuid");
        if (uuidTag != nullptr) {
            std::wstring wstr = uuidTag->GetName();
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::string uuidStr = converter.to_bytes(wstr);

            // Remove all occurrences of backslash
            size_t pos;
            while ((pos = uuidStr.find('\\')) != std::string::npos) {
                uuidStr.erase(pos, 1);
            }

            return uuidStr;
        }
    }
    return "";
}

void FlipItem::saveFlipData() {
    /*nlohmann::json jsonArray(Json::arrayValue);
    for (const auto& item : flipItems) {
        if (item.bought && !item.sold) {
            try {
                jsonArray.append(item.serialize());
            } catch (const std::exception& e) {
                BugLogger::logError(e.what());
            }
        }
    }

    std::ofstream file("config/flipitems.json");
    if (file.is_open()) {
        file << jsonArray.toStyledString();
        file.close();
    } else {
        BugLogger::logError("Failed to open file for writing");
    }*/
}

void FlipItem::loadFlipData() {
    /*std::ifstream file("config/flipitems.json");
    if (!file.is_open()) {
        BugLogger::logError("Failed to open file for reading");
        return;
    }

    nlohmann::json jsonArray;
    file >> jsonArray;
    file.close();

    for (const auto& element : jsonArray) {
        try {
            FlipItem item;
            item.displayName = element.get("displayName", "").asString();
            item.strippedDisplayName = element.get("strippedDisplayName", "").asString();
            item.uuid = element.get("uuid", "").asString();
            item.buyPrice = element.get("buyPrice", 0L).asInt64();
            item.sellPrice = element.get("sellPrice", 0L).asInt64();
            item.coflWorth = element.get("coflWorth", 0L).asInt64();
            item.startTime = element.get("startTime", 0L).asInt64();
            item.buyTime = element.get("buyTime", 0L).asInt64();
            item.buySpeed = element.get("buySpeed", 0).asInt();
            item.auctionStart = element.get("auctionStart", 0L).asInt64();
            item.auctionId = element.get("auctionId", "").asString();
            item.username = element.get("username", "").asString();
            item.sellerUuid = element.get("sellerUuid", "").asString();
            item.skyblockId = element.get("skyblockId", "").asString();
            item.bed = element.get("bed", false).asBool();
            item.buyer = element.get("buyer", "").asString();
            item.sold = element.get("sold", false).asBool();
            item.bought = element.get("bought", false).asBool();
            item.finder = element.get("finder", "").asString();
            if (item.bought && !item.sold) {
                if (!item.uuid.empty()) {
                    flipMap[item.uuid] = item;
                }
            } else {
                flipItems.erase(std::remove(flipItems.begin(), flipItems.end(), item), flipItems.end());
            }
        } catch (const std::exception& e) {
            BugLogger::logError(e.what());
        }
    }*/
}

std::string FlipItem::getItemStrings() {
    std::string result;
    for (const auto& item : flipItems) {
        //result += item.serialize().toStyledString() + ",";
    }
    return result;
}
