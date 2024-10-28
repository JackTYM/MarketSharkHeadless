//
// Created by jack on 8/13/24.
//

#include "AutoBuy.h"

#include <utility>

bool AutoBuy::isWindowOpened = false;

int buyWindowId = 0;
int confirmWindowId = 0;
mc::inventory::Slot exampleConfirmItem;
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
std::wstring priceStr;

FlipItem item;

AutoBuy::AutoBuy(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::OpenWindow, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::SetSlot, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Chat, this);
}

void AutoBuy::startTimeout() {
    isWindowOpened = false;

    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        if (!isWindowOpened) {
            endTaskIfTimedOut();
        }
    }).detach();
}

void AutoBuy::endTaskIfTimedOut() {
    Logger::log(ColorConfig::Debug + "Timeout! No window opened within 5 seconds." + Colors::End);
    QueueManager::endCurrentTask("AutoBuy");
}

void AutoBuy::clickBed(const mc::inventory::Slot &slot) {
    if (QueueManager::getCurrentActionName() == "AutoBuy" && item.bedClicking && !item.closed) {
        std::this_thread::sleep_for(std::chrono::milliseconds(Objects::getBedSpamDelay()));

        //Logger::log("Spam at " + duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() + Colors::End);
        Objects::m_Connection->SendPacket(
                mc::protocol::packets::out::ClickWindowPacket(buyWindowId,
                                                              31, 0,
                                                              Objects::actionNumber++, 0, slot));

        clickBed(slot);
    }
}

void AutoBuy::HandlePacket(mc::protocol::packets::in::OpenWindowPacket *packet) {
    isWindowOpened = true;
    Objects::openWindowId = packet->GetWindowId();
    if (packet->GetWindowTitle().contains(L"BIN Auction View")) {
        if (Objects::getDebug()) {
            Logger::log(ColorConfig::Debug + "Buy Window Opened!" + Colors::End);
        }
        buyWindowId = packet->GetWindowId();

        item.startTime = duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    } else if (packet->GetWindowTitle().contains(L"Confirm Purchase")) {
        if (Objects::getDebug() && !item.skipped) {
            Logger::log(ColorConfig::Debug + "Confirm Window Opened!" + Colors::End);
        }
        confirmWindowId = packet->GetWindowId();
        item.bedClicking = false;
    } else if (packet->GetWindowTitle().contains(L"Auction View")) {
        if (Objects::getDebug()) {
            Logger::log(ColorConfig::Debug + "Normal Auction Opened. Closing!" + Colors::End);
        }
        Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(packet->GetWindowId()));
        item.closed = true;
        Objects::openWindowId = 0;

        QueueManager::endCurrentTask("AutoBuy");
    }
}

void AutoBuy::HandlePacket(mc::protocol::packets::in::SetSlotPacket *packet) {
    if (QueueManager::getCurrentActionName() != "AutoBuy") {
        return;
    }

    if (packet->GetSlot().GetNBT().HasData()) {
        const mc::inventory::Slot slot = packet->GetSlot();
        const mc::nbt::TagCompound root = slot.GetNBT().GetRoot();
        mc::nbt::TagCompound *display = root.GetTag<mc::nbt::TagCompound>(L"display");

        std::wstring itemName = display->GetTag<mc::nbt::TagString>(L"Name")->GetValue();
        int itemId = static_cast<int>(slot.GetItemId());


        if (packet->GetWindowId() == buyWindowId) {
            switch (packet->GetSlotIndex()) {
                case 13: {
                    item.displayName = Objects::replaceAll(converter.to_bytes(itemName), "§f§f", "§a§f");
                    item.strippedDisplayName = Colors::stripColorCodes(item.displayName);
                    item.uuid = item.getUuid(const_cast<mc::inventory::Slot *>(&slot));

                    if (Objects::getDebug()) {
                        Logger::log(ColorConfig::Debug + "Item Name - " + item.displayName + Colors::End);
                    }
                    break;
                }
                case 31: {
                    if (itemId == 355) {
                        Logger::log(ColorConfig::FlipInfo + "Opened Bed!" + Colors::End);

                        item.bed = true;
                        int64_t bedTime = item.auctionStart + Objects::getBedSpamStartDelay();
                        if (Objects::getBafSocket()) {
                            bedTime = item.purchaseAt - 20000 + Objects::getBedSpamStartDelay();
                        }

                        long now = duration_cast<std::chrono::milliseconds>(
                                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                        auto delay = bedTime - now;

                        Logger::log(ColorConfig::FlipInfo + "Spamming item " + item.strippedDisplayName + " in " + std::to_string(delay) + "ms" + Colors::End);

                        item.bedClicking = true;

                        std::thread([slot, delay]() {
                            auto now = std::chrono::system_clock::now();
                            auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    now.time_since_epoch()).count();

                            std::this_thread::sleep_for(std::chrono::milliseconds(delay));

                            AutoBuy::clickBed(slot);

                            std::thread([currentItem = item]() {
                                std::this_thread::sleep_for(std::chrono::seconds(10));
                                if (QueueManager::getCurrentActionName() == "AutoBuy" &&
                                    item.uuid == currentItem.uuid && item.buyPrice == currentItem.buyPrice) {
                                    Logger::log("Bed buy failed! Closing!" + Colors::End);

                                    Objects::m_Connection->SendPacket(
                                            mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                                    item.closed = true;
                                    Objects::openWindowId = 0;

                                    QueueManager::endCurrentTask("AutoBuy");
                                }
                            }).detach();
                        }).detach();
                    } else if (itemId == 371 || (itemName == L"Buy Item Right Now" && itemId != 394)) {
                        priceStr = std::dynamic_pointer_cast<mc::nbt::TagString>(
                                display->GetTag<mc::nbt::TagList>(L"Lore")->GetList()[1])->GetValue();

                        Logger::log(ColorConfig::FlipInfo + "Opened Nugget" + Colors::End);
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::ClickWindowPacket(packet->GetWindowId(),
                                                                              packet->GetSlotIndex(), 0,
                                                                              Objects::actionNumber++, 0, slot));

                        if (Objects::getDebug()) {
                            Logger::log(ColorConfig::Debug + "Clicked!" + Colors::End);
                        }

                        std::thread([currentItem = item]() {
                            std::this_thread::sleep_for(std::chrono::seconds(5));
                            if (QueueManager::getCurrentActionName() == "AutoBuy" && item.uuid == currentItem.uuid &&
                                item.buyPrice == currentItem.buyPrice) {
                                Logger::log("Buy click not registered! Closing!" + Colors::End);

                                Objects::m_Connection->SendPacket(
                                        mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                                item.closed = true;
                                Objects::openWindowId = 0;

                                QueueManager::endCurrentTask("AutoBuy");
                            }
                        }).detach();

                        if (Objects::getSkip()) {
                            item.skipped = true;
                            std::thread([windowId = packet->GetWindowId()]() {
                                if (Objects::getDebug()) {
                                    Logger::log(ColorConfig::Debug + "Confirm Window Opened!" + Colors::End);
                                }
                                std::this_thread::sleep_for(std::chrono::milliseconds(Objects::getSkipDelay()));
                                mc::inventory::Slot fakeConfirm = mc::inventory::Slot(159, 1, 13);

                                auto overrideMeta = std::make_shared<mc::nbt::TagByte>("overrideMeta", 1);
                                fakeConfirm.AddItem(overrideMeta->GetType(), overrideMeta);

                                auto fakeDisplay = std::make_shared<mc::nbt::TagCompound>(L"display");

                                auto loreList = std::make_shared<mc::nbt::TagList>(L"Lore", mc::nbt::TagType::String);
                                // Memory Error? Look into later
                                //loreList->AddItem(std::make_shared<mc::nbt::TagString>("", "§7Purchasing: " + item.displayName));
                                //loreList->AddItem(std::make_shared<mc::nbt::TagString>(L"", L"§7Cost: " + priceStr.substr(9)));

                                fakeDisplay->AddItem(mc::nbt::TagType::String, loreList);
                                fakeDisplay->AddItem(mc::nbt::TagType::String,
                                                     std::make_shared<mc::nbt::TagString>(L"Name", L"§aConfirm"));

                                fakeConfirm.SetTag(L"display", fakeDisplay);

                                Objects::m_Connection->SendPacket(
                                        mc::protocol::packets::out::ClickWindowPacket(windowId + 1, 11, 0,
                                                                                      Objects::actionNumber++, 0,
                                                                                      fakeConfirm));
                            }).detach();
                        }
                    } else if (itemId == 394) {
                        Logger::log(ColorConfig::Important + "Not Enough Coins (Poisonous Potato)! Closing Flip" + Colors::End);
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId == 392) {
                        Logger::log(ColorConfig::FlipInfo + "Lost (Potato)! Closing Flip" + Colors::End);
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId == 166) {
                        Logger::log(ColorConfig::FlipInfo + "Auction Cancelled (Barrier)! Closing Flip" + Colors::End);
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId != 288) {
                        Logger::log(ColorConfig::FlipInfo + "Lost! Closing Flip" + Colors::End);
                        if (Objects::getDebug()) {
                            Logger::log(ColorConfig::Debug + "Item ID - " + std::to_string(itemId) + " Item Name - "
                                      + converter.to_bytes(itemName) + Colors::End);
                        }
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    }
                    break;
                }
            }
        } else if (packet->GetWindowId() == confirmWindowId) {
            if (packet->GetSlotIndex() == 11) {
                if (item.skipped) {
                    return;
                }
                if (itemId == 159) {
                    Objects::m_Connection->SendPacket(
                            mc::protocol::packets::out::ClickWindowPacket(packet->GetWindowId(),
                                                                          packet->GetSlotIndex(), 0,
                                                                          Objects::actionNumber++, 0, slot));

                    if (Objects::getDebug()) {
                        Logger::log(ColorConfig::Debug + "Clicked Confirm!" + Colors::End);
                    }

                    std::thread([currentItem = item]() {
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        if (QueueManager::getCurrentActionName() == "AutoBuy" && item.uuid == currentItem.uuid &&
                            item.buyPrice == currentItem.buyPrice) {
                            Logger::log("Confirm click not registered! Closing!" + Colors::End);

                            Objects::m_Connection->SendPacket(
                                    mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                            item.closed = true;
                            Objects::openWindowId = 0;

                            QueueManager::endCurrentTask("AutoBuy");
                        }
                    }).detach();
                }
            }
        }
    }
}

void AutoBuy::HandlePacket(mc::protocol::packets::in::ChatPacket *packet) {
    if (QueueManager::getCurrentActionName() != "AutoBuy") {
        return;
    }

    if (packet->GetChatPosition() == mc::protocol::packets::in::ChatPacket::ChatPosition::ChatBox) {
        const nlohmann::json &root = packet->GetChatData();

        std::string message = mc::util::ParseChatNode(root);
        message = Colors::stripColorCodes(message);

        if (!message.empty()) {
            if (message.contains("Putting coins in escrow...")) {
                item.buyTime = duration_cast<std::chrono::milliseconds>(
                        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                item.buySpeed = item.buyTime - item.startTime;

                Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                item.closed = true;
                Objects::openWindowId = 0;
            } else if (message.contains("You purchased")) {
                Logger::log(ColorConfig::Won + "Won " + Colors::convertColorCodes(item.displayName)
                          + ColorConfig::Won
                          + " in " + ColorConfig::BuySpeed + std::to_string(item.buySpeed) + "ms" + ColorConfig::Won
                          + " with a (no fee) profit of " + ColorConfig::Coins + std::to_string((item.coflWorth - item.buyPrice))
                          + ColorConfig::Won + " coins" + Colors::End);

                Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                item.closed = true;
                Objects::openWindowId = 0;

                Objects::sendToWebsocket("FlipBought", item.serialize().dump());

                QueueManager::endCurrentTask("AutoBuy");
            } else if (message.contains("There was an error with the auction house!")) {
                QueueManager::endCurrentTask("AutoBuy");
            }
        }
    }
}

void AutoBuy::autoBuy(FlipItem i) {
    startTimeout();
    item = std::move(i);
}