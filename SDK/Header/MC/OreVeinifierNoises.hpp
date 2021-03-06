// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class OreVeinifierNoises {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_OREVEINIFIERNOISES
public:
    class OreVeinifierNoises& operator=(class OreVeinifierNoises const&) = delete;
    OreVeinifierNoises(class OreVeinifierNoises const&) = delete;
    OreVeinifierNoises() = delete;
#endif

public:
    MCAPI ~OreVeinifierNoises();
    MCAPI static std::unique_ptr<class OreVeinifierNoises> make(class XoroshiroPositionalRandomFactory const&);

protected:

private:
    MCAPI OreVeinifierNoises(class NormalNoiseImpl<0, class MultiOctaveNoiseImpl<0, class ParityImprovedNoiseImpl<0> > >, class NormalNoiseImpl<0, class MultiOctaveNoiseImpl<0, class ParityImprovedNoiseImpl<0> > >, class NormalNoiseImpl<0, class MultiOctaveNoiseImpl<0, class ParityImprovedNoiseImpl<0> > >, class NormalNoiseImpl<0, class MultiOctaveNoiseImpl<0, class ParityImprovedNoiseImpl<0> > >, class XoroshiroPositionalRandomFactory);

};