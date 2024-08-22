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
    std::string ItemName;
    int64_t AuctionStart;
    int64_t PurchaseAt;

    FlipData() {}

    FlipData(const std::vector<ChatMessageData>& messages, const std::string& id, int worth, int target, const SoundData& sound,
             const std::string& render, const std::string& finder, const std::string& uuid, const std::string& sellerUuid,
             const std::string& skyblockId, std::string itemName, const int64_t& auctionStart,
             const int64_t& purchaseAt)
            : Messages(messages), Id(id), Worth(worth), Target(target), Sound(sound), Render(render), Finder(finder),
              Uuid(uuid), SellerUuid(sellerUuid), SkyblockId(skyblockId), ItemName(itemName), AuctionStart(auctionStart), PurchaseAt(purchaseAt) {}

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

        if (j.find("itemName") != j.end() && !j.at("itemName").is_null()) {
            j.at("itemName").get_to(f.ItemName);
        } else {
            f.ItemName = "";
        }

        if (j.find("auction") != j.end() && j.at("auction").find("start") != j.at("auction").end() && !j.at("auction").at("start").is_null()) {
            std::string auctionStartStr = j.at("auction").at("start").get<std::string>();

            if (auctionStartStr.contains("Z")) {
                std::tm tm = {};
                std::istringstream ss(auctionStartStr);
                ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
                if (ss.fail()) {
                    throw std::runtime_error("Failed to parse date time");
                }

                std::time_t time = timegm(&tm);  // Use timegm for UTC conversion
                std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);

                // Extract the fractional seconds part
                size_t dot_pos = auctionStartStr.find('.');
                int milliseconds = 0;
                if (dot_pos != std::string::npos) {
                    std::string fractional_part = auctionStartStr.substr(dot_pos + 1);
                    fractional_part = fractional_part.substr(0, 3); // Consider only up to 3 digits
                    milliseconds = std::stoi(fractional_part);
                }

                f.AuctionStart = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() + milliseconds;
            } else {
                // Handle case with offset, e.g., "2024-08-22T03:11:37.043+00:00"
                std::string date_time_part = auctionStartStr.substr(0, auctionStartStr.find('+'));
                std::string offset_part = auctionStartStr.substr(auctionStartStr.find('+'));

                std::tm tm = {};
                std::istringstream ss(date_time_part);
                ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
                if (ss.fail()) {
                    throw std::runtime_error("Failed to parse date time");
                }

                std::time_t time = timegm(&tm);  // Convert to time_t in UTC

                // Parse the offset and adjust time accordingly
                int offset_hours = 0;
                int offset_minutes = 0;
                if (offset_part.size() >= 6) {
                    offset_hours = std::stoi(offset_part.substr(1, 2));
                    offset_minutes = std::stoi(offset_part.substr(4, 2));
                    if (offset_part[0] == '-') {
                        offset_hours = -offset_hours;
                        offset_minutes = -offset_minutes;
                    }
                }
                time -= (offset_hours * 3600 + offset_minutes * 60);  // Adjust the time by the offset

                std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);

                // Extract the fractional seconds part
                size_t dot_pos = auctionStartStr.find('.');
                int milliseconds = 0;
                if (dot_pos != std::string::npos) {
                    std::string fractional_part = auctionStartStr.substr(dot_pos + 1, 3); // Consider only up to 3 digits
                    milliseconds = std::stoi(fractional_part);
                }

                f.AuctionStart = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() + milliseconds;
            }
        }

        if (j.find("purchaseAt") != j.end() && !j.at("purchaseAt").is_null()) {
            std::string auctionStartStr = j.at("purchaseAt").get<std::string>();

            std::tm tm = {};
            std::istringstream ss(auctionStartStr);
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
            if (ss.fail()) {
                throw std::runtime_error("Failed to parse date time");
            }

            // Convert to time_t in UTC
            std::time_t time = timegm(&tm);  // Use timegm for UTC conversion
            std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);

            // Extract the fractional seconds part
            size_t dot_pos = auctionStartStr.find('.');
            int milliseconds = 0;
            if (dot_pos != std::string::npos) {
                std::string fractional_part = auctionStartStr.substr(dot_pos + 1);
                fractional_part = fractional_part.substr(0, 3); // Consider only up to 3 digits
                milliseconds = std::stoi(fractional_part);
            }

            f.PurchaseAt = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() + milliseconds;
        }
    }
};

#endif //MARKETSHARKCPP_FLIPDATA_H
