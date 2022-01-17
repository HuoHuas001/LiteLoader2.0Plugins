#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>

#include <MC/ItemStack.hpp>
#include <MC/Item.hpp>
#include <MC/Block.hpp>
#include <MC/Dimension.hpp>
#include <SendPacketAPI.h>
#include <MC/BlockActorDataPacket.hpp>
#include <MC/UpdateBlockPacket.hpp>
#include <MC/BlockSource.hpp>
#include <MC/ContainerOpenPacket.hpp>
#include <MC/VanillaBlocks.hpp>
#include <MC/CompoundTag.hpp>
#include <MC/BlockInstance.hpp>
#include <MC/ServerPlayer.hpp>
#include <MC/BlockActor.hpp>
#include <ScheduleAPI.h>
#include <MC/FillingContainer.hpp>
#include <MC/ListTag.hpp>
static_assert(sizeof(UpdateBlockPacket) == 72);
static_assert(sizeof(BlockActorDataPacket) == 88);
static_assert(sizeof(ContainerOpenPacket) == 72);

void updateBlock(ServerPlayer* sp, BlockPos a1)
{

    auto nbt1 = CompoundTag::create();
    nbt1->putString("id", "Chest");
    nbt1->putInt("x", a1.x);
    nbt1->putInt("y", a1.y);
    nbt1->putInt("z", a1.z);
    nbt1->putInt("pairx", a1.x - 1);
    nbt1->putInt("pairz", a1.z);
    nbt1->putByte("pairlead", 1);
    nbt1->putString("CustomName", u8"Ïä×Óui²âÊÔ");

    BinaryStream bs;
    bs.writeVarInt(a1.x);
    bs.writeUnsignedVarInt(a1.y);
    bs.writeVarInt(a1.z);
    bs.writeUnsignedVarInt(VanillaBlocks::mChest->getRuntimeId());
    bs.writeUnsignedVarInt(3);          // flag
    bs.writeUnsignedVarInt(0);          // layer
    NetworkPacket<21> pkt(bs.getRaw()); // updateBlock Packet
    auto* newpkt1 = (BlockActorDataPacket*)new char[sizeof(BlockActorDataPacket)];
    auto out1 = SymCall("??0BlockActorDataPacket@@QEAA@AEBVBlockPos@@VCompoundTag@@@Z", BlockActorDataPacket*, BlockActorDataPacket*, BlockPos, CompoundTag*)(newpkt1, a1, nbt1.get());
    sp->sendNetworkPacket(pkt);
    sp->sendNetworkPacket(*out1);


    auto nbt2 = CompoundTag::create();
    nbt2->putString("id", "Chest");
    nbt2->putInt("x", a1.x - 1);
    nbt2->putInt("y", a1.y);
    nbt2->putInt("z", a1.z);
    nbt2->putInt("pairx", a1.x);
    nbt2->putInt("pairz", a1.z);
    nbt2->putByte("pairlead", 1);
    nbt2->putString("CustomName", u8"Ïä×Óui²âÊÔ");

    BinaryStream bs2;
    bs2.writeVarInt(a1.x - 1);
    bs2.writeUnsignedVarInt(a1.y);
    bs2.writeVarInt(a1.z);
    bs2.writeUnsignedVarInt(VanillaBlocks::mChest->getRuntimeId());
    bs2.writeUnsignedVarInt(3); // flag
    bs2.writeUnsignedVarInt(0); // layer
    NetworkPacket<21> pkt2(bs2.getRaw());

    auto* newpkt2 = (BlockActorDataPacket*)new char[sizeof(BlockActorDataPacket)];
    auto out2 = SymCall("??0BlockActorDataPacket@@QEAA@AEBVBlockPos@@VCompoundTag@@@Z", BlockActorDataPacket*, BlockActorDataPacket*, BlockPos, CompoundTag*)(newpkt2, a1.add(-1), nbt1.get());
    sp->sendNetworkPacket(pkt2);
    sp->sendNetworkPacket(*out2);
}

void open(ServerPlayer* sp, BlockPos pos)
{
    Schedule::delay([sp,pos] {
        auto pkt = ContainerOpenPacket(ContainerID::Inventory, ContainerType::CONTAINER, pos, -1);
        sp->sendNetworkPacket(pkt);
        }, 5);
}

bool useitem(const Event::PlayerUseItemEvent & ev) {
	auto sp = (ServerPlayer*)ev.mPlayer;
    updateBlock(sp, sp->getBlockPos().add(2, 4, 0));
    open(sp, sp->getBlockPos().add(2, 4, 0));
    return true;
}

bool useitemon(const Event::PlayerUseItemOnEvent& ev) {
    auto sp = (ServerPlayer*)ev.mPlayer;
    auto bi = ev.mBlockInstance;
    if(bi.getBlockEntity()) std::cout << bi.getBlockEntity()->getNbt()->toSNBT() << std::endl;
    return true;
}

void EventInit() {
	Event::PlayerUseItemEvent::subscribe(useitem);
    Event::PlayerUseItemOnEvent::subscribe(useitemon);
}

void PluginInit()
{
	EventInit();
}

