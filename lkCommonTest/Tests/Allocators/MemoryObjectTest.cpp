#include "lkCommon/Allocators/MemoryObject.hpp"
#include "lkCommon/Allocators/Memory.hpp"
#include <gtest/gtest.h>

using namespace lkCommon::Allocators;

namespace {

template <class Allocator>
class Object: public MemoryObject<Allocator>
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


template <typename Allocator>
void TestMemoryObject()
{
    Memory<Allocator> memory;

    Object<Allocator>* a = new (memory) Object<Allocator>(VALUE_A, VALUE_B);

    ASSERT_NE(nullptr, a);
    EXPECT_EQ(VALUE_A, a->mA);
    EXPECT_EQ(VALUE_B, a->mB);

    Object<Allocator>::operator delete(a, memory);

    #ifdef _DEBUG
        EXPECT_EQ(DEAD_AREA_MAGIC, *(reinterpret_cast<uint32_t*>(a)));
    #endif // _DEBUG
}


TEST(MemoryObject, ArenaAllocator)
{
    TestMemoryObject<ArenaAllocator>();
}
