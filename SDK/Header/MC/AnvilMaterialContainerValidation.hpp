// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"
#include "ContainerValidationBase.hpp"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class AnvilMaterialContainerValidation : public ContainerValidationBase {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_ANVILMATERIALCONTAINERVALIDATION
public:
    class AnvilMaterialContainerValidation& operator=(class AnvilMaterialContainerValidation const&) = delete;
    AnvilMaterialContainerValidation(class AnvilMaterialContainerValidation const&) = delete;
    AnvilMaterialContainerValidation() = delete;
#endif

public:
    /*0*/ virtual ~AnvilMaterialContainerValidation();
    /*1*/ virtual bool isValidSlotForContainer(class ContainerScreenContext const&, class Container const&, int) const;
    /*2*/ virtual void __unk_vfn_0();
    /*3*/ virtual void __unk_vfn_1();
    /*4*/ virtual void __unk_vfn_2();

protected:

private:

};