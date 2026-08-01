#include <gtest/gtest.h>
#include <lkCommon/Utils/UUID.hpp>

#include <sstream>

lkCommon::Utils::UUID ZERO_UUID;
const std::string ZERO_UUID_STR = "00000000-0000-0000-0000-000000000000";

const std::string INVALID_UUID_STR = "thisisnotUUID";

const std::string SMALLER_UUID_STR = "00000000-0000-0000-0000-000000000001";
const std::string LARGER_UUID_STR = "00000000-0000-0000-0000-000000000002";


TEST(UUID, Constructor)
{
    lkCommon::Utils::UUID uuid;
    ASSERT_EQ(ZERO_UUID, uuid);
}

TEST(UUID, Generate)
{
    lkCommon::Utils::UUID uuid;
    EXPECT_EQ(ZERO_UUID, uuid);

    uuid.Generate();
    ASSERT_NE(ZERO_UUID, uuid);
}

TEST(UUID, FromString)
{
    lkCommon::Utils::UUID uuid;
    EXPECT_EQ(ZERO_UUID, uuid);

    uuid.Generate();
    ASSERT_NE(ZERO_UUID, uuid);

    ASSERT_TRUE(uuid.FromString(ZERO_UUID_STR));
    ASSERT_EQ(ZERO_UUID, uuid);
}

TEST(UUID, FromStringInvalid)
{
    lkCommon::Utils::UUID uuid;
    EXPECT_EQ(ZERO_UUID, uuid);

    ASSERT_FALSE(uuid.FromString(INVALID_UUID_STR));
    ASSERT_EQ(ZERO_UUID, uuid);
}

TEST(UUID, ToString)
{
    lkCommon::Utils::UUID uuid;
    EXPECT_EQ(ZERO_UUID, uuid);

    ASSERT_EQ(ZERO_UUID_STR, uuid.ToString());
}

TEST(UUID, Equal)
{
    lkCommon::Utils::UUID uuid;
    ASSERT_EQ(ZERO_UUID, uuid);
}

TEST(UUID, NotEqual)
{
    lkCommon::Utils::UUID uuid;
    uuid.Generate();
    ASSERT_NE(ZERO_UUID, uuid);
}

TEST(UUID, LessThan)
{
    lkCommon::Utils::UUID uuidSmall, uuidLarge;

    ASSERT_TRUE(uuidSmall.FromString(SMALLER_UUID_STR));
    ASSERT_TRUE(uuidLarge.FromString(LARGER_UUID_STR));

    ASSERT_LT(uuidSmall, uuidLarge);
}

TEST(UUID, MoreThan)
{
    lkCommon::Utils::UUID uuidSmall, uuidLarge;

    ASSERT_TRUE(uuidSmall.FromString(SMALLER_UUID_STR));
    ASSERT_TRUE(uuidLarge.FromString(LARGER_UUID_STR));

    ASSERT_GT(uuidLarge, uuidSmall);
}

TEST(UUID, OperatorStream)
{
    lkCommon::Utils::UUID uuid;
    ASSERT_EQ(ZERO_UUID, uuid);

    std::stringstream stream;

    stream << uuid;

    ASSERT_EQ(ZERO_UUID_STR, stream.str());
}
