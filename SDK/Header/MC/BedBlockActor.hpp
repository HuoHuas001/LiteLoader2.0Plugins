// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"
#include "BlockActor.hpp"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class BedBlockActor : public BlockActor {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_BEDBLOCKACTOR
public:
    class BedBlockActor& operator=(class BedBlockActor const&) = delete;
    BedBlockActor(class BedBlockActor const&) = delete;
    BedBlockActor() = delete;
#endif

public:
    /*0*/ virtual ~BedBlockActor();
    /*1*/ virtual void load(class Level&, class CompoundTag const&, class DataLoadHelper&);
    /*2*/ virtual bool save(class CompoundTag&) const;
    /*3*/ virtual bool saveItemInstanceData(class CompoundTag&);
    /*4*/ virtual void saveBlockData(class CompoundTag&, class BlockSource&) const;
    /*5*/ virtual void loadBlockData(class CompoundTag const&, class BlockSource&, class DataLoadHelper&);
    /*6*/ virtual void onCustomTagLoadDone(class BlockSource&);
    /*7*/ virtual void tick(class BlockSource&);
    /*8*/ virtual void onChanged(class BlockSource&);
    /*9*/ virtual void onPlace(class BlockSource&);
    /*10*/ virtual void __unk_vfn_0();
    /*11*/ virtual void onRemoved(class BlockSource&);
    /*12*/ virtual void triggerEvent(int, int);
    /*13*/ virtual void clearCache();
    /*14*/ virtual void __unk_vfn_1();
    /*15*/ virtual float getShadowRadius(class BlockSource&) const;
    /*16*/ virtual bool hasAlphaLayer() const;
    /*17*/ virtual class BlockActor* getCrackEntity(class BlockSource&, class BlockPos const&);
    /*18*/ virtual std::string const& getCustomName() const;
    /*19*/ virtual std::string getName() const;
    /*20*/ virtual std::string getImmersiveReaderText(class BlockSource&);
    /*21*/ virtual int getRepairCost() const;
    /*22*/ virtual class PistonBlockActor* getOwningPiston(class BlockSource&);
    /*23*/ virtual void __unk_vfn_2();
    /*24*/ virtual void __unk_vfn_3();
    /*25*/ virtual float getDeletionDelayTimeSeconds() const;
    /*26*/ virtual void __unk_vfn_4();
    /*27*/ virtual void __unk_vfn_5();
    /*28*/ virtual void __unk_vfn_6();
    /*29*/ virtual std::unique_ptr<class BlockActorDataPacket> _getUpdatePacket(class BlockSource&);
    /*30*/ virtual void _onUpdatePacket(class CompoundTag const&, class BlockSource&);
    /*31*/ virtual bool _playerCanUpdate(class Player const&) const;
    MCAPI BedBlockActor(class BlockPos const&);
    MCAPI bool claimPetSleepOnBed(struct ActorUniqueID);
    MCAPI void startSleepingOn();
    MCAPI void stopSleepingOn(class BlockSource&, bool);

protected:

private:

};