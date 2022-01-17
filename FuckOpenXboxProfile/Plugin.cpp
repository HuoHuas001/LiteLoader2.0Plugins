#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <MC/Packet.hpp>
#include <MC/ReadOnlyBinaryStream.hpp>
 TClasslessInstanceHook(
    void,
    "?_sendInternal@NetworkHandler@@AEAAXAEBVNetworkIdentifier@@AEBVPacket@@AEBV?$basic_string@DU?$char_traits@D@std@@"
    "V?$allocator@D@2@@std@@@Z",
    NetworkIdentifier const& id, Packet& pkt, std::string& data) {

    auto stream  = ReadOnlyBinaryStream(data, 0i64);
    auto pktid   = stream.getUnsignedVarInt();
    switch ((int)pkt.getId()) {
        case 123:
            std::cout << (int)dAccess<char, 48>(&pkt) << std::endl;
            std::cout << dAccess<float, 52>(&pkt) << std::endl;
            std::cout << dAccess<float, 56>(&pkt) << std::endl;
            std::cout << dAccess<float, 60>(&pkt) << std::endl;
            std::cout << dAccess<int, 64>(&pkt) << std::endl;
            std::cout << (int)dAccess<bool, 104>(&pkt) << std::endl;
            std::cout << (int)dAccess<bool, 105>(&pkt) << std::endl;
            return original(this, id, pkt, data);;
        default:
            break;
    }
    original(this, id, pkt, data);
}

void PluginInit()
{


}