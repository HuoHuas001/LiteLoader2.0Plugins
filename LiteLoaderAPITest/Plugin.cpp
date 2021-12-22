#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <MC/GameMode.hpp>
#include <MC/ServerPlayer.hpp>
#include <MC/Container.hpp>
#include <mc/CompoundTag.hpp>
#include <MC/Level.hpp>
#include <MC/BlockSource.hpp>
#include <MC/Block.hpp>
Logger test("LiteLoaderAPITest");

#define TEST(API)                        \
     try {                               \
        test.info("{} = {}", #API, API); \
      }                                  \
    catch (...)  \
    {                                    \
        test.error("API Boom!");         \
        test.error("In : {}" #API);       \
         return false;                    \
    }                                     \


THook(bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z", GameMode* a1,
	 ItemStack* item, BlockPos* blockpos, __int8 a4,  Vec3* a5,  Block* block) {
	ServerPlayer* player = a1->getPlayer();

	test.info("=========Now Test PlayerAPI=========");
	TEST(player->getRealName());
	TEST(player->getAvgPing());
	TEST(player->getLastPing());
	TEST(player->getIP());
	TEST(player->getLanguageCode());
	TEST(player->getServerAddress());
	TEST(player->getDeviceTypeName());
	TEST(player->sendText(player->getNbt()->toSNBT()));
	TEST(player->talkAs("test sendtestpacket"));
	TEST(player->giveItem(item));
	TEST(player->getName());
	TEST(player->getEnderChestContainer()->addItem_s(item));
	TEST(player->getRespawnPosition().first.toString());//bad?
	TEST(player->getNbt()->toString());
	TEST(player->getUuid());
	TEST(player->getAvgPacketLoss());
	TEST(player->getClientId());
	TEST(player->getDeviceType());
	test.info("=========Test PlayerAPI END=========");
	test.info("=========Now Test ActorAPI=========");
	TEST(player->getPosition().toString());
	TEST(player->isSimulatedPlayer());
	TEST(player->isPlayer());
	TEST(player->isItemActor());
	TEST(player->isOnGround());
	TEST(player->getDeviceType());
	TEST(player->getTypeName());
	TEST(player->hurtEntity(1));
	TEST(player->getBlockPos().toString());
	TEST(player->getBlockStandingOn().getPosition().toString());
	TEST(player->getActorUniqueId().get());
	TEST(player->teleport(player->getPos().add(0,1,0), player->getDimensionId()));
	TEST(player->getHandSlot()->getTypeName());
	TEST(player->rename("aaa"));
	TEST(player->getNbt()->toString());
	TEST(player->refreshActorData());
	TEST(player->setOnFire(2,1));
	TEST(player->getCameraPos().toString());
	//TEST(player->getLastTick());
	//TEST(player->getBlockFromViewVector().getBlock()->getNameString());
	//TEST(player->getActorFromViewVector(1.11)->getNameTag());
	TEST(player->addEffect(MobEffect::EffectType::NightVision,1000,2));
	test.info("=========Test ActorAPI END=========");
	test.info("=========Now Test ItemAPI=========");
	TEST(item->getTypeName());
	TEST(item->getAux());
	TEST(item->getCount());
	TEST(item->setItem(item));
	TEST(item->getNbt()->toString());
	test.info("=========Test ItemAPI END=========");

	test.info("=========Now Test BlockAPI=========");
	TEST(block->getTypeName());
	TEST(block->getId());
	TEST(block->getTileData());
	TEST(block->getNbt()->toString());
	test.info("=========Test BlockAPI END=========");


	return original(a1,  item,  blockpos, a4, a5,  block);
}

THook(bool, "?attack@Player@@UEAA_NAEAVActor@@AEBW4ActorDamageCause@@@Z",
	Player* _this, Actor* ac, int* damageCause)
{
	_this->sendPlaySoundPacket("random.glass", _this->getPos(), 1.00, 1.00);
	return original(_this, ac, damageCause);
}

void PluginInit()
{
	test.info("Loaded");
}