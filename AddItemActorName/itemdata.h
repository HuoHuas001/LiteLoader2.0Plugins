#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "pch.h"


namespace DataTypeMap {

    template <typename T>
    DataItemType typeFor;

    template <>
    inline DataItemType typeFor<int8_t> = DataItemType::BYTE;
    template <>
    inline DataItemType typeFor<int16_t> = DataItemType::SHORT;
    template <>
    inline DataItemType typeFor<int32_t> = DataItemType::INT;
    template <>
    inline DataItemType typeFor<float> = DataItemType::FLOAT;
    template <>
    inline DataItemType typeFor<std::string> = DataItemType::STRING;
    template <>
    inline DataItemType typeFor<CompoundTag> = DataItemType::NBT;
    template <>
    inline DataItemType typeFor<BlockPos> = DataItemType::POS;
    template <>
    inline DataItemType typeFor<int64_t> = DataItemType::LONG;
    template <>
    inline DataItemType typeFor<Vec3> = DataItemType::VEC3;

    template <typename T>
    struct neqFor {
        static bool neq(T const& lhs, T const& rhs) {
            return lhs != rhs;
        }
    };
    template <typename T>
    struct copyFor {
        static void copy(T& lhs, T const& rhs) {
            lhs = rhs;
        }
    };

} // namespace DataTypeMap

#include <MC/DataItem.hpp>

template <typename T>
class DataItem2 : public DataItem {
public:
    T value;
    DataItem2(uint16_t id, T const& value)
        : DataItem(DataTypeMap::typeFor<T>, id, true)
        , value(value) {
    }
};

template <>
class DataItem2<int8_t> : public DataItem {
public:
    int8_t value;
    DataItem2(uint16_t id, int8_t const& value)
        : DataItem(DataTypeMap::typeFor<int8_t>, id, true)
        , value(value) {
    }

    inline bool getFlag(int32_t bit) const {
        return (1 << bit) & value;
    }
    inline void setFlag(int32_t bit) {
        value |= 1 << bit;
    }
    inline void clearFlag(int32_t bit) {
        value &= ~(1 << bit);
    }
};

template <>
class DataItem2<int64_t> : public DataItem {
public:
    int64_t value;
    DataItem2(uint16_t id, int64_t const& value)
        : DataItem(DataTypeMap::typeFor<int64_t>, id, true)
        , value(value) {
    }

    inline bool getFlag(int32_t bit) const {
        return (1LL << bit) & value;
    }
    inline void setFlag(int32_t bit) {
        value |= 1LL << bit;
    }
    inline void clearFlag(int32_t bit) {
        value &= ~(1LL << bit);
    }
};