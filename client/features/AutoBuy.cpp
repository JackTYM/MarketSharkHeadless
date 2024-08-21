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
    std::cout << "Timeout! No window opened within 5 seconds." << Colors::End;
    QueueManager::endCurrentTask("AutoBuy");
}

void AutoBuy::clickBed(const mc::inventory::Slot &slot) {
    if (QueueManager::getCurrentActionName() == "AutoBuy" && item.bedClicking && !item.closed) {
        std::this_thread::sleep_for(std::chrono::milliseconds(Objects::getBedSpamDelay()));

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
            std::cout << Colors::Black << "Buy Window Opened!" << Colors::End;
        }
        buyWindowId = packet->GetWindowId();

        item.startTime = std::chrono::high_resolution_clock::now();
    } else if (packet->GetWindowTitle().contains(L"Confirm Purchase")) {
        if (Objects::getDebug()) {
            std::cout << Colors::Black << "Confirm Window Opened!" << Colors::End;
        }
        confirmWindowId = packet->GetWindowId();
        item.bedClicking = false;
    } else if (packet->GetWindowTitle().contains(L"Auction View")) {
        if (Objects::getDebug()) {
            std::cout << Colors::Black << "Normal Auction Opened. Closing!" << Colors::End;
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
                    item.uuid = item.getUuid(const_cast<mc::inventory::Slot*>(&slot));

                    if (Objects::getDebug()) {
                        std::cout << Colors::Black << "Item Name - " << item.displayName << Colors::End;
                    }
                    break;
                }
                case 31: {
                    if (itemId == 355) {
                        std::cout << Colors::Cyan << "Opened Bed!" << Colors::End;

                        item.bed = true;
                        auto bedSpamStartDelay = std::chrono::milliseconds(Objects::getBedSpamStartDelay());
                        std::chrono::time_point<std::chrono::high_resolution_clock> bedTime = item.auctionStart + bedSpamStartDelay;
                        if (Objects::getBafSocket()) {
                            bedTime = item.purchaseAt - std::chrono::seconds (20) + std::chrono::milliseconds(Objects::getBedSpamStartDelay());
                        }

                        auto now = std::chrono::high_resolution_clock::now();
                        auto delay = duration_cast<std::chrono::milliseconds>(bedTime - now).count();

                        std::cout << Colors::Cyan << "Spamming item " << item.strippedDisplayName << " in " << delay << "ms" << Colors::End;

                        item.bedClicking = true;

                        std::thread([slot, delay]() {
                            std::this_thread::sleep_for(std::chrono::milliseconds(delay));

                            AutoBuy::clickBed(slot);

                            std::thread([ currentItem = item]() {
                                std::this_thread::sleep_for(std::chrono::seconds(10));
                                if (QueueManager::getCurrentActionName() == "AutoBuy" && item.uuid == currentItem.uuid && item.buyPrice == currentItem.buyPrice) {
                                    std::cout << "Bed buy failed! Closing!" << Colors::End;

                                    Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                                    item.closed = true;
                                    Objects::openWindowId = 0;

                                    QueueManager::endCurrentTask("AutoBuy");
                                }
                            }).detach();
                        }).detach();
                    } else if (itemId == 371 || (itemName == L"Buy Item Right Now" && itemId != 394)) {
                        std::cout << Colors::Cyan << "Opened Nugget" << Colors::End;

                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::ClickWindowPacket(packet->GetWindowId(),
                                                                              packet->GetSlotIndex(), 0,
                                                                              Objects::actionNumber++, 0, slot));

                        if (Objects::getDebug()) {
                            std::cout << Colors::Black << "Clicked!" << Colors::End;
                        }

                        std::thread([ currentItem = item]() {
                            std::this_thread::sleep_for(std::chrono::seconds(5));
                            if (QueueManager::getCurrentActionName() == "AutoBuy" && item.uuid == currentItem.uuid && item.buyPrice == currentItem.buyPrice) {
                                std::cout << "Buy click not registered! Closing!" << Colors::End;

                                Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                                item.closed = true;
                                Objects::openWindowId = 0;

                                QueueManager::endCurrentTask("AutoBuy");
                            }
                        }).detach();

                        /*if (Objects::skip && exampleConfirmItem.GetNBT().HasData()) {
                            mc::nbt::NBT confirmNbt = exampleConfirmItem.GetNBT();
                            mc::nbt::TagCompound confirmRoot = confirmNbt.GetRoot();
                            mc::nbt::TagCompound *confirmDisplay = confirmRoot.GetTag<mc::nbt::TagCompound>(L"display");
                            mc::nbt::TagList *confirmLore = confirmDisplay->GetTag<mc::nbt::TagList>(L"Lore");

                            mc::nbt::TagList *lore = display->GetTag<mc::nbt::TagList>(L"Lore");
                            auto buyPtr = std::dynamic_pointer_cast<mc::nbt::TagString>(lore->GetList()[1]);

                            if (buyPtr) {
                                std::wstring itemCostLine = buyPtr->GetValue();

                                auto purchasePtr = std::dynamic_pointer_cast<mc::nbt::TagString>(
                                        confirmLore->GetList()[0]);
                                if (purchasePtr) {
                                    //purchasePtr->SetValue(L"§7Purchasing: " + item.displayName);
                                    //std::cout << "FakeL - " << converter.to_bytes(L"§7Purchasing: " + item.displayName)
                                    //          << Colors::End;

                                    auto costPtr = std::dynamic_pointer_cast<mc::nbt::TagString>(
                                            confirmLore->GetList()[1]);
                                    if (costPtr) {
                                        costPtr->SetValue(L"§7Cost: " + itemCostLine.substr(9));
                                        std::cout << "FakeL - "
                                                  << converter.to_bytes(L"§7Cost: " + itemCostLine.substr(9))
                                                  << Colors::End;

                                        std::cout << Colors::Bold << Colors::Yellow << "SKIPPING WITH "
                                                  << Objects::skipDelay << "MS DOTN LEAK DUMB FUCK" << Colors::End;

                                        mc::nbt::TagPtr fakeConfirmDisplay = std::make_shared<mc::nbt::TagCompound>(
                                                *confirmDisplay);

                                        confirmRoot.SetTag(L"display", fakeConfirmDisplay);
                                        confirmNbt.SetRoot(confirmRoot);
                                        exampleConfirmItem.SetNBT(confirmNbt);

                                        Objects::openWindowId = packet->GetWindowId() + 1;
                                        auto sendPacketWithDelay = [packet]() {
                                            std::this_thread::sleep_for(std::chrono::milliseconds(Objects::skipDelay));
                                            Objects::m_Connection->SendPacket(
                                                    mc::protocol::packets::out::ClickWindowPacket(
                                                            packet->GetWindowId() + 1,
                                                            11, 0,
                                                            Objects::actionNumber++, 0, exampleConfirmItem
                                                    )
                                            );
                                        };
                                        std::thread(sendPacketWithDelay).detach();

                                        if (Objects::getDebug()) {
                                            std::cout << Colors::Black << "Clicked Confirm (Skip)!" << Colors::End;
                                        }
                                    }
                                }
                            }
                        }*/
                    } else if (itemId == 394) {
                        std::cout << Colors::Red << "Not Enough Coins (Poisonous Potato)! Closing Flip" << Colors::End;
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId == 392) {
                        std::cout << Colors::Red << "Lost (Potato)! Closing Flip" << Colors::End;
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId == 166) {
                        std::cout << Colors::Red << "Auction Cancelled (Barrier)! Closing Flip" << Colors::End;
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        item.closed = true;
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId != 288) {
                        std::cout << Colors::Red << "Lost! Closing Flip" << Colors::End;
                        if (Objects::getDebug()) {
                            std::cout << Colors::Black << "Item ID - " << itemId << " Item Name - "
                                      << converter.to_bytes(itemName) << Colors::End;
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
                if (itemId == 159) {
                    Objects::m_Connection->SendPacket(
                            mc::protocol::packets::out::ClickWindowPacket(packet->GetWindowId(),
                                                                          packet->GetSlotIndex(), 0,
                                                                          Objects::actionNumber++, 0, slot));

                    if (Objects::getDebug()) {
                        std::cout << Colors::Black << "Clicked Confirm!" << Colors::End;
                    }

                    std::thread([ currentItem = item]() {
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        if (QueueManager::getCurrentActionName() == "AutoBuy" && item.uuid == currentItem.uuid && item.buyPrice == currentItem.buyPrice) {
                            std::cout << "Confirm click not registered! Closing!" << Colors::End;

                            Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                            item.closed = true;
                            Objects::openWindowId = 0;

                            QueueManager::endCurrentTask("AutoBuy");
                        }
                    }).detach();

                    if (slot.GetNBT().HasData()) {
                        mc::nbt::TagList *lore = slot.GetNBT().GetRoot().GetTag<mc::nbt::TagCompound>(L"display")->GetTag<mc::nbt::TagList>(L"Lore");

                        auto costPtr = std::dynamic_pointer_cast<mc::nbt::TagString>(
                                lore->GetList()[1]);

                        if (costPtr) {
                            std::string costLine = converter.to_bytes(costPtr->GetValue()).substr(12);

                            costLine.erase(std::remove(costLine.begin(), costLine.end(), ','), costLine.end());
                            costLine.erase(costLine.length() - 5);

                            item.buyPrice = std::stol(costLine);
                        }
                    }

                    exampleConfirmItem = slot;
                }

                //mc::nbt::TagString purchasing = lore->GetList()[0];
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
                item.buyTime = std::chrono::high_resolution_clock::now();
                item.buySpeed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                        item.buyTime - item.startTime).count());

                Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                item.closed = true;
                Objects::openWindowId = 0;
            } else if (message.contains("You purchased")) {
                std::cout << Colors::Yellow << "Won " << Colors::convertColorCodes(item.displayName) << Colors::Yellow << " in " << Colors::Green << item.buySpeed << "ms" << Colors::Yellow << " with a (no fee) profit of " << Colors::Magenta << (item.coflWorth - item.buyPrice) << Colors::Yellow << " coins" << Colors::End;

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