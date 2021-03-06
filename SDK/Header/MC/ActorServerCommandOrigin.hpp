// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"
#include "Json.hpp"
#include "ActorCommandOrigin.hpp"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class ActorServerCommandOrigin : public ActorCommandOrigin {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_ACTORSERVERCOMMANDORIGIN
public:
    class ActorServerCommandOrigin& operator=(class ActorServerCommandOrigin const&) = delete;
    ActorServerCommandOrigin(class ActorServerCommandOrigin const&) = delete;
    ActorServerCommandOrigin() = delete;
#endif

public:
    /*0*/ virtual ~ActorServerCommandOrigin();
    /*1*/ virtual std::string const& getRequestId() const;
    /*2*/ virtual class Level* getLevel() const;
    /*3*/ virtual enum CommandPermissionLevel getPermissionsLevel() const;
    /*4*/ virtual std::unique_ptr<class CommandOrigin> clone() const;
    /*5*/ virtual class std::optional<class BlockPos> getCursorHitBlockPos() const;
    /*6*/ virtual class std::optional<class Vec3> getCursorHitPos() const;
    /*7*/ virtual bool canUseAbility(enum AbilitiesIndex) const;
    /*8*/ virtual bool canUseCommandsWithoutCheatsEnabled() const;
    /*9*/ virtual bool isSelectorExpansionAllowed() const;
    /*10*/ virtual unsigned char getSourceSubId() const;
    /*11*/ virtual class CommandOrigin const& getOutputReceiver() const;
    /*12*/ virtual enum CommandOriginType getOriginType() const;
    /*13*/ virtual class mce::UUID const& getUUID() const;
    /*14*/ virtual void handleCommandOutputCallback(class Json::Value&&) const;
    /*15*/ virtual class CompoundTag serialize() const;
    /*16*/ virtual bool isValid() const;
    MCAPI ActorServerCommandOrigin(class Actor&);
    MCAPI void setTargetOther(struct ActorUniqueID);
    MCAPI static std::unique_ptr<class ActorServerCommandOrigin> load(class CompoundTag const&, class Level&);

protected:

private:

};