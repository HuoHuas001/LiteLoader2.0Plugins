#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <RegCommandAPI.h>
#include <MC/ServerPlayer.hpp>
#include <MC/Level.hpp>
#include <MC/MapItem.hpp>
#include <MC/CompoundTag.hpp>
#include <MC/MapItemSavedData.hpp>
#include "Color.h"
#include <Utils/Bstream.h>
#include <Utils/useful.h>
using namespace std;



using namespace RegisterCommandHelper;

inline static ServerPlayer* MakeSP(CommandOrigin const& ori) {
	ServerPlayer* sp = (ServerPlayer*)ori.getEntity();
	if (sp) {
		return { sp };
	}
	return nullptr;
}

class CustomMapCommand : public Command {
    string file;

public:
    void execute(CommandOrigin const& ori, CommandOutput& outp) const override {
		ServerPlayer* sp = MakeSP(ori);
		const ItemStack& item = sp->getCarriedItem();
		ActorUniqueID id = MapItem::getMapId(item.getUserData());
		MapItemSavedData* mapd = Global<Level>->getMapSavedData(id);
		if (!mapd) { outp.error(u8"not a filled map"); return; }
		mapd->setLocked();//locked
		auto& pix = dAccess<std::vector<unsigned int>, 0x30>(mapd);//getPixels()
		ifstream ifs("plugins/CustomMap/mapbin/" + file, std::ios::binary);
		auto str = ifs2str(ifs);
		RBStream rs{ str };
		if (ifs.fail()) {
			outp.error(u8"file open error");
			return;
		}
		for (int i = 0; i < 16384; ++i) {
			unsigned int val;
			rs.apply(val);
			printf("%p\n", val);
			pix[i] = 0xff000000 | val;
		}
		for (int i = 0; i < 128; ++i)
			for (int j = 0; j < 128; ++j) {
				//auto color = 
				mapd->setPixelDirty( i, j);
			}
		mapd->save(*Global<LevelStorage>);
		outp.success(u8"set map success");
		return;
    }

    static void setup(CommandRegistry* registry) {
        registry->registerCommand( "custommap", "custommap",CommandPermissionLevel::Console,{ (CommandFlagValue)0 },{ (CommandFlagValue)0x80 });

        registry->registerOverload<CustomMapCommand>("custommap", 
            makeMandatory(&CustomMapCommand::file, "optional"));
    }
};


void RegisterCommands() {
    Event::RegCmdEvent::subscribe([](Event::RegCmdEvent ev) { // Register commands
        CustomMapCommand::setup(ev.mCommandRegistry);
        return true;
        });
}

void PluginInit()
{
    RegisterCommands();
}