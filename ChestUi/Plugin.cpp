#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/ItemStack.hpp>
#include <MC/Item.hpp>
#include <MC/Dimension.hpp>
#include <SendPacketAPI.h>
#include <MC/BlockActorDataPacket.hpp>
#include <MC/UpdateBlockPacket.hpp>
#include <MC/BlockSource.hpp>
#include <MC/ContainerOpenPacket.hpp>
#include <MC/ContainerClosePacket.hpp>
#include <MC/ChestBlockActor.hpp>
#include <MC/VanillaBlocks.hpp>
#include <MC/CompoundTag.hpp>
#include <MC/BlockInstance.hpp>
#include <MC/ServerPlayer.hpp>
#include <MC/BlockActor.hpp>
#include <ScheduleAPI.h>
#include <MC/FillingContainer.hpp>
#include <MC/ListTag.hpp>
#include <RegCommandAPI.h>
#include <LoggerAPI.h>
#include <MC/Level.hpp>
#include <MC/ItemStackRequestPacket.hpp>

//Yaml
#include "../SDK/Header/third-party/yaml-cpp/yaml.h"

static_assert(sizeof(UpdateBlockPacket) == 72);
static_assert(sizeof(BlockActorDataPacket) == 88);
static_assert(sizeof(ContainerOpenPacket) == 72);

using namespace std;
using namespace LL;

Logger logger("ChestUi");
static unordered_map<ServerPlayer*, BlockPos> RecordedInfo;

void updateBlock(ServerPlayer* sp, BlockPos a1)
{
    string nbt = u8"{\"Findable\":0b,\"Items\":[],\"id\":\"Chest\",\"isMovable\":1b,\"x\":-9,\"y\":120,\"z\":48}";
    auto nbt1 = CompoundTag::fromSNBT(nbt);
    //auto nbt1 = CompoundTag::create();
    nbt1->putString("id", "Chest");
    nbt1->putInt("x", a1.x);
    nbt1->putInt("y", a1.y);
    nbt1->putInt("z", a1.z);
    nbt1->putInt("pairx", a1.x - 1);
    nbt1->putInt("pairz", a1.z);
    nbt1->putByte("pairlead", 1);
    nbt1->putString("CustomName", u8"CHEST MENU");

    auto EntityNBT = CompoundTag::create();


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
}

void open(ServerPlayer* sp, BlockPos pos)
{
    Schedule::delay([sp,pos] {
        auto pkt = ContainerOpenPacket(ContainerID::Inventory, ContainerType::CONTAINER, pos, -1);
        RecordedInfo[sp] = pos;
        sp->sendNetworkPacket(pkt);
    }, 0.1);
}

class openCommand : public Command {
public:
    void execute(CommandOrigin const& ori, CommandOutput& output) const override {//执行部分
        ServerPlayer* sp = ori.getPlayer();
        updateBlock(sp, sp->getBlockPos().add(0, 2, 0));
        open(sp, sp->getBlockPos().add(0, 2, 0));
    }

    static void setup(CommandRegistry* registry) {//注册部分(推荐做法)
        registry->registerCommand("open", "open chest ui", CommandPermissionLevel::Any, { (CommandFlagValue)0 }, { (CommandFlagValue)0x80 });
        registry->registerOverload<openCommand>("open");
    }
};

bool useitemon(const Event::PlayerUseItemOnEvent& ev) {
    auto sp = (ServerPlayer*)ev.mPlayer;
    auto bi = ev.mBlockInstance;
    if(bi.getBlockEntity()) std::cout << bi.getBlockEntity()->getNbt()->toSNBT() << std::endl;
    return true;
}

bool regcmd(const Event::RegCmdEvent& ev) {
    openCommand::setup(ev.mCommandRegistry);
    return true;
}

bool sendNetBlock(BlockSource& bs, BlockPos bp, const unsigned int runtimeId)
{
    auto dim = Global<Level>->getDimension(bs.getDimensionId());
    BinaryStream wp;
    wp.writeVarInt(bp.x);
    wp.writeUnsignedVarInt(bp.y);
    wp.writeVarInt(bp.z);
    wp.writeUnsignedVarInt(runtimeId);
    wp.writeUnsignedVarInt(3);
    wp.writeUnsignedVarInt(0);
    std::shared_ptr<Packet> pkt = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateBlock);
    pkt->read(wp);
    dim->sendPacketForPosition({ bp.x, bp.y, bp.z }, *pkt, nullptr);
    return true;
}

void EventInit() {
    Event::RegCmdEvent::subscribe(regcmd);
}

//关闭箱子触发
THook(void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVContainerClosePacket@@@Z", 
    ServerNetworkHandler* _this, NetworkIdentifier* a2, ContainerClosePacket* a3) {
    ServerPlayer* sp = _this->getServerPlayer(*a2,0);
    BlockSource& bs = sp->getRegion();
    BlockPos bp = RecordedInfo[sp];
    sendNetBlock(bs, bp, bs.getBlock(bp).getRuntimeId());
    return original(_this,a2,a3);
}

//选择物品触发
THook(void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVItemStackRequestPacket@@@Z",
    ServerNetworkHandler* _this,
    NetworkIdentifier* a2,
    ItemStackRequestPacket* a3) {
    ServerPlayer* sp = _this->getServerPlayer(*a2, 0);
    BlockSource& bs = sp->getRegion();
    BlockPos bp = RecordedInfo[sp];
    return original(_this, a2,a3);
}

void PluginInit()
{
	EventInit();
}

