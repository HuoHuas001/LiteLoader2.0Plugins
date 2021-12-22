#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <MC/LevelStorage.hpp>

Logger test("NoPlayerNull");

THook(void, "?save@LevelStorage@@QEAAXAEAVActor@@@Z", LevelStorage* a1) {
	return;
}

void PluginInit()
{
	test.info("Loaded success");
}