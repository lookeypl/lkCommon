#include "lkCommon/Utils/ArenaObject.hpp"
#include "lkCommon/Utils/ArenaAllocator.hpp"
#include <gtest/gtest.h>

using namespace lkCommon::Utils;

namespace {

class Object: public ArenaObject
{
public:
    uint32_t mA;
    uint32_t mB;

    Object()
        : mA(0), mB(0)
    {
    }

    Object(uint32_t a, uint32_t b)
        : mA(a), mB(b)
    {
    }
};

const uint32_t VALUE_A = 0x0000000F;
const uint32_t VALUE_B = 42;

const uint32_t DEAD_AREA_MAGIC = 0xDEADBEEF;

} // namespace


TEST(ArenaObject, CreateObject)
{
    ArenaAllocator::Instance().FreeChunks();

    Object* a = new Object(VALUE_A, VALUE_B);

    ASSERT_NE(nullptr, a);
    EXPECT_EQ(VALUE_A, a->mA);
    EXPECT_EQ(VALUE_B, a->mB);

    Object* bPtr = nullptr;

    {
        std::unique_ptr<Object> b = std::make_unique<Object>(VALUE_A, VALUE_B);
        bPtr = b.get();

        ASSERT_NE(nullptr, b);
        EXPECT_EQ(VALUE_A, b->mA);
        EXPECT_EQ(VALUE_B, b->mB);
    }

    EXPECT_NE(DEAD_AREA_MAGIC, *(reinterpret_cast<uint32_t*>(a)));
    EXPECT_EQ(DEAD_AREA_MAGIC, *(reinterpret_cast<uint32_t*>(bPtr)));
}
