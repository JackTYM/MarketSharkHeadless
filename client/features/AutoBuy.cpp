//
// Created by jack on 8/13/24.
//

#include "AutoBuy.h"

int AutoBuy::openWindow = 0;

AutoBuy::AutoBuy(mc::protocol::packets::PacketDispatcher* dispatcher)
    : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::SetSlot, this);
}

void AutoBuy::HandlePacket(mc::protocol::packets::in::SetSlotPacket *packet) {

    if (packet->GetSlot().GetNBT().HasData()) {
        const mc::nbt::TagCompound root = packet->GetSlot().GetNBT().GetRoot();

        if (root.GetName())
    }

    /*mc::nbt::TagCompound* display = packet->GetSlot().GetNBT().GetTag<mc::nbt::TagCompound>(L"display");
    if (display.)

    std::wstring wstr = packet->GetSlot().GetNBT().GetTag<mc::nbt::TagCompound>(L"display")
            ->GetTag<mc::nbt::TagString>(L"Name")->GetValue();
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string itemName = converter.to_bytes(wstr);
    std::cout << "Slot | WindowID - " << packet->GetWindowId() << "SlotId - " << packet->GetSlotIndex() << "Item Name - " << itemName << std::endl;
*/
}

void AutoBuy::autoBuy(FlipItem item) {
    QueueManager::addTaskToStart("AutoBuy", [&item]() {
        Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(AutoBuy::openWindow));
        AutoBuy::autoBuy(item);
        Objects::m_Connection->SendPacket(mc::protocol::packets::out::ChatPacket("/viewauction " + item.auctionId));
    });
}