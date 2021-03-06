// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"
#include "Json.hpp"
#include "CommandOrigin.hpp"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class ClientAutomationCommandOrigin : public CommandOrigin {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_CLIENTAUTOMATIONCOMMANDORIGIN
public:
    class ClientAutomationCommandOrigin& operator=(class ClientAutomationCommandOrigin const&) = delete;
    ClientAutomationCommandOrigin(class ClientAutomationCommandOrigin const&) = delete;
    ClientAutomationCommandOrigin() = delete;
#endif

public:
    /*0*/ virtual ~ClientAutomationCommandOrigin();
    /*1*/ virtual std::string const& getRequestId() const;
    /*2*/ virtual std::string getName() const;
    /*3*/ virtual class BlockPos getBlockPosition() const;
    /*4*/ virtual class Vec3 getWorldPosition() const;
    /*5*/ virtual class Level* getLevel() const;
    /*6*/ virtual class Dimension* getDimension() const;
    /*7*/ virtual class Actor* getEntity() const;
    /*8*/ virtual enum CommandPermissionLevel getPermissionsLevel() const;
    /*9*/ virtual std::unique_ptr<class CommandOrigin> clone() const;
    /*10*/ virtual class std::optional<class BlockPos> getCursorHitBlockPos() const;
    /*11*/ virtual class std::optional<class Vec3> getCursorHitPos() const;
    /*12*/ virtual bool canUseAbility(enum AbilitiesIndex) const;
    /*13*/ virtual bool canUseCommandsWithoutCheatsEnabled() const;
    /*14*/ virtual bool isSelectorExpansionAllowed() const;
    /*15*/ virtual unsigned char getSourceSubId() const;
    /*16*/ virtual class CommandOrigin const& getOutputReceiver() const;
    /*17*/ virtual enum CommandOriginType getOriginType() const;
    /*18*/ virtual struct CommandOriginData toCommandOriginData() const;
    /*19*/ virtual class mce::UUID const& getUUID() const;
    /*20*/ virtual void handleCommandOutputCallback(class Json::Value&&) const;
    /*21*/ virtual class CompoundTag serialize() const;
    /*22*/ virtual bool isValid() const;
    MCAPI ClientAutomationCommandOrigin(std::string const&);

protected:

private:

};