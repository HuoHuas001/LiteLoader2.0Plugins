// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"
#include "Actor.hpp"
#include "Monster.hpp"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class Piglin : public Monster {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_PIGLIN
public:
    class Piglin& operator=(class Piglin const&) = delete;
    Piglin(class Piglin const&) = delete;
    Piglin() = delete;
#endif

public:
    /*0*/ virtual ~Piglin();
    /*1*/ virtual bool isRuntimePredictedMovementEnabled() const;
    /*2*/ virtual void __unk_vfn_0();
    /*3*/ virtual class Vec3 getInterpolatedRidingOffset(float) const;
    /*4*/ virtual void __unk_vfn_1();
    /*5*/ virtual float getRidingHeight();
    /*6*/ virtual void __unk_vfn_2();
    /*7*/ virtual void __unk_vfn_3();
    /*8*/ virtual float getCameraOffset() const;
    /*9*/ virtual void __unk_vfn_4();
    /*10*/ virtual void __unk_vfn_5();
    /*11*/ virtual void playerTouch(class Player&);
    /*12*/ virtual void __unk_vfn_6();
    /*13*/ virtual void __unk_vfn_7();
    /*14*/ virtual bool isDamageBlocked(class ActorDamageSource const&) const;
    /*15*/ virtual void __unk_vfn_8();
    /*16*/ virtual void __unk_vfn_9();
    /*17*/ virtual void __unk_vfn_10();
    /*18*/ virtual void __unk_vfn_11();
    /*19*/ virtual bool isValidTarget(class Actor*) const;
    /*20*/ virtual void adjustDamageAmount(int&) const;
    /*21*/ virtual void onTame();
    /*22*/ virtual void onFailedTame();
    /*23*/ virtual void vehicleLanded(class Vec3 const&, class Vec3 const&);
    /*24*/ virtual void onBounceStarted(class BlockPos const&, class Block const&);
    /*25*/ virtual float getPickRadius();
    /*26*/ virtual void awardKillScore(class Actor&, int);
    /*27*/ virtual class HashedString const& queryEntityRenderer() const;
    /*28*/ virtual struct ActorUniqueID getSourceUniqueID() const;
    /*29*/ virtual int getPortalWaitTime() const;
    /*30*/ virtual bool canChangeDimensions() const;
    /*31*/ virtual void __unk_vfn_12();
    /*32*/ virtual struct ActorUniqueID getControllingPlayer() const;
    /*33*/ virtual bool canPickupItem(class ItemStack const&) const;
    /*34*/ virtual bool canBePulledIntoVehicle() const;
    /*35*/ virtual void __unk_vfn_13();
    /*36*/ virtual bool canSynchronizeNewEntity() const;
    /*37*/ virtual void __unk_vfn_14();
    /*38*/ virtual void __unk_vfn_15();
    /*39*/ virtual bool isWorldBuilder() const;
    /*40*/ virtual void __unk_vfn_16();
    /*41*/ virtual bool isAdventure() const;
    /*42*/ virtual bool getInteraction(class Player&, class ActorInteraction&, class Vec3 const&);
    /*43*/ virtual bool canDestroyBlock(class Block const&) const;
    /*44*/ virtual void setAuxValue(int);
    /*45*/ virtual void stopSpinAttack();
    /*46*/ virtual void __unk_vfn_17();
    /*47*/ virtual void __unk_vfn_18();
    /*48*/ virtual void updateEntitySpecificMolangVariables(class RenderParams&);
    /*49*/ virtual void __unk_vfn_19();
    /*50*/ virtual void readAdditionalSaveData(class CompoundTag const&, class DataLoadHelper&);
    /*51*/ virtual void addAdditionalSaveData(class CompoundTag&);
    /*52*/ virtual void __unk_vfn_20();
    /*53*/ virtual void __unk_vfn_21();
    /*54*/ virtual void spawnAnim();
    /*55*/ virtual void aiStep();
    /*56*/ virtual bool checkSpawnRules(bool);
    /*57*/ virtual float getItemUseStartupProgress() const;
    /*58*/ virtual float getItemUseIntervalProgress() const;
    /*59*/ virtual void __unk_vfn_22();
    /*60*/ virtual bool isAlliedTo(class Mob*);
    /*61*/ virtual void __unk_vfn_23();
    /*62*/ virtual void sendArmorDamage(class std::bitset<4> const&);
    /*63*/ virtual void onBorn(class Actor&, class Actor&);
    /*64*/ virtual int getAttackTime();
    /*65*/ virtual void __unk_vfn_24();
    /*66*/ virtual void _serverAiMobStep();
    /*67*/ virtual void __unk_vfn_25();
    /*
    inline bool canExistInPeaceful() const{
        bool (Piglin::*rv)() const;
        *((void**)&rv) = dlsym("?canExistInPeaceful@Piglin@@UEBA_NXZ");
        return (this->*rv)();
    }
    inline void updateEntitySpecificMolangVariables(class RenderParams& a0){
        void (Piglin::*rv)(class RenderParams&);
        *((void**)&rv) = dlsym("?updateEntitySpecificMolangVariables@Piglin@@UEAAXAEAVRenderParams@@@Z");
        return (this->*rv)(std::forward<class RenderParams&>(a0));
    }
    */
    MCAPI Piglin(class ActorDefinitionGroup*, struct ActorDefinitionIdentifier const&, class OwnerPtrT<struct EntityRefTraits> const&);

protected:

private:

};