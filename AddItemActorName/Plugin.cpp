#include "pch.h"
#include "itemdata.h"
#include <MC/Level.hpp>
#include <MC/SynchedActorData.hpp>
#include <MC/Packet.hpp>
#include <MC/ItemStack.hpp>
#include <MC/ItemActor.hpp>
class SetActorDataPacket : public Packet {
public:
    ActorRuntimeID                         rid;
    std::vector<std::unique_ptr<DataItem>> items;

    inline SetActorDataPacket() {
    }
    MCAPI SetActorDataPacket(ActorUniqueID, SynchedActorData&, bool);
    inline ~SetActorDataPacket() {
    }
    MCAPI virtual MinecraftPacketIds getId() const;
    MCAPI virtual std::string        getName() const;
    MCAPI virtual void               write(BinaryStream&) const;

private:
    MCAPI virtual StreamReadResult _read(ReadOnlyBinaryStream&);
};

static_assert(offsetof(SetActorDataPacket, rid) == 48);
class AddItemActorPacket : public Packet {
public:
    std::vector<std::unique_ptr<DataItem>> items;
    uint64_t type = 0;
    ActorUniqueID uid;
    ActorRuntimeID rid;
    ItemStack stack;
    Vec3 pos, speed;
    bool from_fishing = false;

    inline ~AddItemActorPacket() {}
    MCAPI virtual MinecraftPacketIds getId() const;
    MCAPI virtual std::string getName() const;
    MCAPI virtual void write(BinaryStream&) const;
    MCAPI virtual StreamReadResult read(ReadOnlyBinaryStream&);

};

static_assert(offsetof(AddItemActorPacket, type) == 72);
static_assert(offsetof(AddItemActorPacket, uid) == 80);
static_assert(offsetof(AddItemActorPacket, rid) == 88);
static_assert(offsetof(AddItemActorPacket, stack) == 96);
#include <mc/Player.hpp>
THook(AddItemActorPacket**, "?tryCreateAddActorPacket@ItemActor@@UEAA?AV?$unique_ptr@VAddActorBasePacket@@U?$default_delete@VAddActorBasePacket@@@std@@@std@@XZ", struct ItemActor* a1, AddItemActorPacket** a2)
{
    AddItemActorPacket* v4; // rax
    AddItemActorPacket* v5; // rdi

    v4 = (AddItemActorPacket*)operator new(0x100ui64);
    v5 = v4;
    if (v4)
    {
        v4 = SymCall("??0AddItemActorPacket@@QEAA@AEAVItemActor@@@Z", AddItemActorPacket*, AddItemActorPacket*, ItemActor&)(v4, *a1);
        v4->items.emplace_back(std::make_unique<DataItem2<string>>(ActorDataIDs::NAMETAG, a1->getTypeName()));
        v4->items.emplace_back(std::make_unique<DataItem2<int8_t>>(ActorDataIDs::ALWAYS_SHOW_NAMETAG, true));
        v4->items.emplace_back(std::make_unique<DataItem2<float>>(ActorDataIDs::SCALE, 2.0));
    }

    *a2 = v4;
    return a2;
}

THook(void, "?write@AddItemActorPacket@@UEBAXAEAVBinaryStream@@@Z", AddItemActorPacket* pkt, __int64* bs)
{
    std::cout << dAccess<std::vector<std::unique_ptr<DataItem>>, 48>(pkt).size() << std::endl;
    return original(pkt, bs);
}
