// This Header is auto generated by BDSLiteLoader Toolchain
#pragma once
#define AUTO_GENERATED
#include "../Global.h"

#define BEFORE_EXTRA
// Include Headers or Declare Types Here

#undef BEFORE_EXTRA

class Vec4 {

#define AFTER_EXTRA
// Add Member There

#undef AFTER_EXTRA

#ifndef DISABLE_CONSTRUCTOR_PREVENTION_VEC4
public:
    class Vec4& operator=(class Vec4 const&) = delete;
    Vec4(class Vec4 const&) = delete;
    Vec4() = delete;
#endif

public:
    MCAPI static class Vec4 const MAX;
    MCAPI static class Vec4 const MIN;
    MCAPI static class Vec4 const ONE;
    MCAPI static class Vec4 const ZERO;

protected:

private:

};