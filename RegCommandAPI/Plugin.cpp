#include "pch.h"
#include <EventAPI.h>
#include <MC/Block.hpp>

class TestCommand : public Command {
public:
    void execute(CommandOrigin const& ori, CommandOutput& output) const override {
     
    }
    static void setup(CommandRegistry* registry) {
        registry->registerCommand(
            "test","test",
            CommandPermissionLevel::Any,
            { (CommandFlagValue)0 },
            { (CommandFlagValue)0x80 }
        );
        registry->registerOverload<TestCommand>("test");
    }
};



void PluginInit()
{
    Event::PlayerDestroyBlockEvent::subscribe([](Event::PlayerDestroyBlockEvent ev) {
        auto a = ev.mBlockInstance;
        a.getBlock()->destroy(*a.getBlockSource(), a.getPosition(), ev.mPlayer);
        return true;
        });

}