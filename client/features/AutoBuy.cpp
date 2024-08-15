//
// Created by jack on 8/13/24.
//

#include "AutoBuy.h"

#include <utility>

bool AutoBuy::isWindowOpened = false;

int buyWindowId = 0;
int confirmWindowId = 0;

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
    std::cout << "Timeout! No window opened within 5 seconds." << std::endl;
    QueueManager::endCurrentTask("AutoBuy");
}

void AutoBuy::HandlePacket(mc::protocol::packets::in::OpenWindowPacket *packet) {
    isWindowOpened = true;
    Objects::openWindowId = packet->GetWindowId();
    if (packet->GetWindowTitle().contains(L"BIN Auction View")) {
        std::cout << "Buy Window Opened!" << std::endl;
        buyWindowId = packet->GetWindowId();

        item.startTime = std::chrono::high_resolution_clock::now();
    } else if (packet->GetWindowTitle().contains(L"Confirm Purchase")) {
        std::cout << "Confirm Window Opened!" << std::endl;
        confirmWindowId = packet->GetWindowId();
    } else if (packet->GetWindowTitle().contains(L"Auction View")) {
        std::cout << "Normal Auction Opened. Closing!" << std::endl;
        Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(packet->GetWindowId()));
        Objects::openWindowId = 0;

        QueueManager::endCurrentTask("AutoBuy");
    }
}

void AutoBuy::HandlePacket(mc::protocol::packets::in::SetSlotPacket *packet) {
    if (QueueManager::getCurrentActionName() != "AutoBuy") {
        return;
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    if (packet->GetSlot().GetNBT().HasData()) {
        const mc::inventory::Slot slot = packet->GetSlot();
        const mc::nbt::TagCompound root = slot.GetNBT().GetRoot();
        mc::nbt::TagCompound *display = root.GetTag<mc::nbt::TagCompound>(L"display");

        std::wstring itemName = display->GetTag<mc::nbt::TagString>(L"Name")->GetValue();
        int itemId = static_cast<int>(slot.GetItemId());


        if (packet->GetWindowId() == buyWindowId) {
            switch (packet->GetSlotIndex()) {
                case 13: {
                    std::cout << "Item Name - " << converter.to_bytes(itemName) << std::endl;
                    break;
                }
                case 31: {
                    if (itemId == 355) {
                        std::cout << "Bed! (Closing temp)" << std::endl;
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                        Objects::openWindowId = 0;

                        QueueManager::endCurrentTask("AutoBuy");
                    } else if (itemId == 371 || (itemName == L"Buy Item Right Now" && itemId != 394)) {
                        std::cout << "Nugget!" << std::endl;

                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::ClickWindowPacket(packet->GetWindowId(),
                                                                              packet->GetSlotIndex(), 0,
                                                                              Objects::actionNumber++, 0, slot));

                        std::cout << "Clicked!" << std::endl;
                    } else {
                        std::cout << "Lost! Closing Flip" << std::endl;
                        Objects::m_Connection->SendPacket(
                                mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
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

                    std::cout << "Clicked Confirm!" << std::endl;
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
        std::size_t pos = message.find((char) 0xA7);

        while (pos != std::string::npos) {
            message.erase(pos - 1, 3);
            pos = message.find((char) 0xA7);
        }

        message.erase(std::remove_if(message.begin(), message.end(), [](char c) {
            return c < 32 || c > 126;
        }), message.end());
        if (!message.empty()) {
            if (message.contains("Putting coins in escrow...")) {
                item.buyTime = std::chrono::high_resolution_clock::now();
                item.buySpeed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(item.buyTime - item.startTime).count());

                Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                Objects::openWindowId = 0;
            } else if (message.contains("You purchased")) {
                std::cout << "Won Item in " << item.buySpeed << "ms" << std::endl;

                Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
                Objects::openWindowId = 0;

                QueueManager::endCurrentTask("AutoBuy");
            }
        }
    }
}

void AutoBuy::autoBuy(FlipItem i) {
    startTimeout();
    item = std::move(i);
}