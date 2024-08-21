#ifndef MARKETSHARKCPP_FLIPDATA_H
#define MARKETSHARKCPP_FLIPDATA_H

#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "ChatMessageData.h"
#include "SoundData.h"
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class FlipData {
public:
    std::vector<ChatMessageData> Messages;
    std::string Id;
    int Worth;
    int Target;
    SoundData Sound;
    std::string Render;
    std::string Finder;
    std::string Uuid;
    std::string SellerUuid;
    std::string SkyblockId;
    std::chrono::time_point<std::chrono::high_resolution_clock> AuctionStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> PurchaseAt;

    FlipData() {}

    FlipData(const std::vector<ChatMessageData>& messages, const std::string& id, int worth, int target, const SoundData& sound,
             const std::string& render, const std::string& finder, const std::string& uuid, const std::string& sellerUuid,
             const std::string& skyblockId, const std::chrono::time_point<std::chrono::high_resolution_clock>& auctionStart,
             const std::chrono::time_point<std::chrono::high_resolution_clock>& purchaseAt)
            : Messages(messages), Id(id), Worth(worth), Target(target), Sound(sound), Render(render), Finder(finder),
              Uuid(uuid), SellerUuid(sellerUuid), SkyblockId(skyblockId), AuctionStart(auctionStart), PurchaseAt(purchaseAt) {}

    friend void to_json(json& j, const FlipData& f) {
        j = json{{"messages", f.Messages}, {"id", f.Id}, {"worth", f.Worth}, {"target", f.Target}, {"sound", f.Sound},
                 {"render", f.Render}, {"finder", f.Finder}, {"uuid", f.Uuid}, {"sellerUuid", f.SellerUuid},
                 {"skyblockId", f.SkyblockId}, {"auctionStart", std::chrono::duration_cast<std::chrono::milliseconds>(f.AuctionStart.time_since_epoch()).count()},
                 {"purchaseAt", std::chrono::duration_cast<std::chrono::milliseconds>(f.PurchaseAt.time_since_epoch()).count()}};
    }

    friend void from_json(const json& j, FlipData& f) {
        if (j.find("messages") != j.end() && !j.at("messages").is_null()) {
            j.at("messages").get_to(f.Messages);
        }

        if (j.find("id") != j.end() && !j.at("id").is_null()) {
            j.at("id").get_to(f.Id);
        } else {
            f.Id = "";
        }

        if (j.find("worth") != j.end() && !j.at("worth").is_null()) {
            j.at("worth").get_to(f.Worth);
        } else {
            f.Worth = 0;
        }

        if (j.find("target") != j.end() && !j.at("target").is_null()) {
            j.at("target").get_to(f.Target);
        } else {
            f.Target = 0;
        }

        if (j.find("sound") != j.end() && !j.at("sound").is_null()) {
            j.at("sound").get_to(f.Sound);
        }

        if (j.find("render") != j.end() && !j.at("render").is_null()) {
            j.at("render").get_to(f.Render);
        } else {
            f.Render = "";
        }

        if (j.find("finder") != j.end() && !j.at("finder").is_null()) {
            j.at("finder").get_to(f.Finder);
        } else {
            f.Finder = "";
        }

        if (j.find("uuid") != j.end() && !j.at("uuid").is_null()) {
            j.at("uuid").get_to(f.Uuid);
        } else {
            f.Uuid = "";
        }

        if (j.find("sellerUuid") != j.end() && !j.at("sellerUuid").is_null()) {
            j.at("sellerUuid").get_to(f.SellerUuid);
        } else {
            f.SellerUuid = "";
        }

        if (j.find("skyblockId") != j.end() && !j.at("skyblockId").is_null()) {
            j.at("skyblockId").get_to(f.SkyblockId);
        } else {
            f.SkyblockId = "";
        }

        if (j.find("auction") != j.end() && j.at("auction").find("start") != j.at("auction").end() && !j.at("auction").at("start").is_null()) {
            std::string auctionStartStr = j.at("auction").at("start").get<std::string>();
            std::istringstream in{auctionStartStr};
            std::tm tm = {};
            in >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
            if (!in.fail()) {
                auto auctionStartTime = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                f.AuctionStart = std::chrono::time_point_cast<std::chrono::high_resolution_clock::duration>(
                        auctionStartTime);
            }
        }

        if (j.find("purchaseAt") != j.end() && !j.at("purchaseAt").is_null()) {
            std::string auctionStartStr = j.at("purchaseAt").get<std::string>();
            std::istringstream in{auctionStartStr};
            std::tm tm = {};
            in >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
            if (!in.fail()) {
                auto auctionStartTime = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                f.PurchaseAt = std::chrono::time_point_cast<std::chrono::high_resolution_clock::duration>(auctionStartTime);
            }
        }
    }
};

#endif //MARKETSHARKCPP_FLIPDATA_H
