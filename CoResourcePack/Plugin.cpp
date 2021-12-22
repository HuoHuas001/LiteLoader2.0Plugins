#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>

using namespace std;
Logger test("CoResourcePack");
THook(void*, "?write@ResourcePacksInfoPacket@@UEBAXAEAVBinaryStream@@@Z", void* a, void* b)
{
    dAccess<bool, 48>(a) = true;
    return original(a, b);
}

void PluginInit()
{
    test.info("Client Resource unlocked.");
}