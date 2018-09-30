#include <gtest/gtest.h>
#include <lkCommon/Utils/StringConv.hpp>

const std::string STRING = "Testąęćżźół";
const std::wstring WSTRING = L"Testąęćżźół";


TEST(StringConv, StringToWStringTest)
{
    const std::string input = STRING;
    std::wstring output;

    ASSERT_TRUE(lkCommon::Utils::StringToWString(input, output));
    EXPECT_EQ(WSTRING, output);
}

TEST(StringConv, WStringToStringTest)
{
    const std::wstring input = WSTRING;
    std::string output;

    ASSERT_TRUE(lkCommon::Utils::WStringToString(input, output));
    EXPECT_EQ(STRING, output);
}
