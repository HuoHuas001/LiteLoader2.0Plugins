#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Packet.hpp>
#include <MC/ReadOnlyBinaryStream.hpp>


THook(bool, "?_read@PurchaseReceiptPacket@@EEAA?AW4StreamReadResult@@AEAVReadOnlyBinaryStream@@@Z", Packet* a1, ReadOnlyBinaryStream* a2) {
	return 0;
}

THook(bool, "?_read@EmoteListPacket@@EEAA?AW4StreamReadResult@@AEAVReadOnlyBinaryStream@@@Z", Packet* a1, ReadOnlyBinaryStream* a2) {
    ReadOnlyBinaryStream pkt(a2->getData(), 0);
    pkt.getUnsignedVarInt();
    pkt.getUnsignedVarInt();
    if (pkt.getUnsignedVarInt() >= 0xfff)return 0;
    if (pkt.getUnsignedVarInt() >= 0xfff)return 0;
    return original(a1, a2);
}

THook(bool, "?_read@ClientCacheMissResponsePacket@@EEAA?AW4StreamReadResult@@AEAVReadOnlyBinaryStream@@@Z", Packet* a1, ReadOnlyBinaryStream* a2) {
    ReadOnlyBinaryStream pkt(a2->getData(), 0);
    pkt.getUnsignedVarInt();
    if (pkt.getUnsignedVarInt() >= 0xfff)return 0;
    return original(a1, a2);
}

THook(bool, "?_read@SetScoreboardIdentityPacket@@EEAA?AW4StreamReadResult@@AEAVReadOnlyBinaryStream@@@Z", Packet* a1, ReadOnlyBinaryStream* a2) {
    ReadOnlyBinaryStream pkt(a2->getData(), 0);
    std::cout<<  a2->getUnsignedChar() << std::endl;
    std::cout << a2->getUnsignedInt() << std::endl;
    std::cout << a2->getUnsignedInt() << std::endl;
    std::cout << a2->getUnsignedInt() << std::endl;
    return original(a1, a2);
}