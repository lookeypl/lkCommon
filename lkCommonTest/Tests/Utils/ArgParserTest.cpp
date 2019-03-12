#include <gtest/gtest.h>
#include <lkCommon/Utils/ArgParser.hpp>

#include <array>


template <size_t N>
using ArgPair = std::array<const char*, N>;

const char ARG_A_SHORT_NAME = 'a';
const char ARG_B_SHORT_NAME = 'b';
const char ARG_C_SHORT_NAME = 'c';
const char ARG_D_SHORT_NAME = 'd';
const std::string ARG_A_NAME("aa");
const std::string ARG_B_NAME("bvery_long_arg");
const std::string ARG_C_NAME("ccc");
const std::string ARG_D_NAME("ddd");
const std::string ARG_A_FULL_NAME("a,aa");
const std::string ARG_B_FULL_NAME("bvery_long_arg,b");
const std::string ARG_C_FULL_NAME("c,ccc");
const std::string ARG_D_FULL_NAME("d,ddd");
const std::string ARG_INVALID_A_FULL_NAME("a,a");
const std::string ARG_INVALID_B_FULL_NAME(",bbbb");
const std::string ARG_INVALID_C_FULL_NAME(",c");
const std::string ARG_INVALID_D_FULL_NAME("d,");
const std::string ARG_INVALID_E_FULL_NAME("eeeeee,");
const std::string ARG_INVALID_F_FULL_NAME("fff,fffff");

const char* EXE = "stub_program_name.exe.bat.420.nope";
const char* ARG_A_FLAG = "--aa";
const char* ARG_B_VALUE = "--bvery_long_arg=42";
const char* ARG_C = "--ccc";
const char* ARG_C_STRING = "blah";
const char* ARG_D_TIGHT = "-d4";
const char* ARG_D_WITH_EQ = "-d=4";
const char* ARG_D_SPARSE_1 = "-d";
const char* ARG_D_SPARSE_2 = "4";
const char* ARG_D_LONG_WITH_EQ = "--ddd=4";
const char* ARG_D_LONG_SPARSE_1 = "--ddd";
const char* ARG_D_LONG_SPARSE_2 = "4";

const bool A_FLAG = true;
const int32_t B_VALUE = 42;
const std::string C_STRING("blah");
const int32_t D_VALUE = 4;

const ArgPair<5> TEST_ARGS_ALL = {
    EXE,
    ARG_A_FLAG,
    ARG_B_VALUE,
    ARG_C, ARG_C_STRING,
};

const ArgPair<4> TEST_ARGS_MISSING_A = {
    EXE,
    ARG_B_VALUE,
    ARG_C, ARG_C_STRING,
};

const ArgPair<4> TEST_ARGS_MISSING_B = {
    EXE,
    ARG_A_FLAG,
    ARG_C, ARG_C_STRING,
};

const ArgPair<3> TEST_ARGS_MISSING_C = {
    EXE,
    ARG_A_FLAG,
    ARG_B_VALUE,
};

const ArgPair<2> TEST_ARGS_D_TIGHT = {
    EXE,
    ARG_D_TIGHT,
};

const ArgPair<2> TEST_ARGS_D_WITH_EQ = {
    EXE,
    ARG_D_WITH_EQ,
};

const ArgPair<3> TEST_ARGS_D_SPARSE = {
    EXE,
    ARG_D_SPARSE_1, ARG_D_SPARSE_2,
};

const ArgPair<2> TEST_ARGS_D_LONG_WITH_EQ = {
    EXE,
    ARG_D_LONG_WITH_EQ,
};

const ArgPair<3> TEST_ARGS_D_LONG_SPARSE = {
    EXE,
    ARG_D_LONG_SPARSE_1, ARG_D_LONG_SPARSE_2,
};

template <size_t N>
void TestArgCollection(const ArgPair<N>& args, bool aPresent, bool bPresent, bool cPresent)
{
    lkCommon::Utils::ArgParser a;

    ASSERT_TRUE(a.Add(ARG_A_FULL_NAME, lkCommon::Utils::ArgType::FLAG, !aPresent));
    ASSERT_TRUE(a.Add(ARG_B_FULL_NAME, lkCommon::Utils::ArgType::VALUE, !bPresent));
    ASSERT_TRUE(a.Add(ARG_C_FULL_NAME, lkCommon::Utils::ArgType::STRING, !cPresent));

    int32_t val = 0;
    std::string str;

    ASSERT_TRUE(a.Parse(static_cast<int>(args.size()), args.data()));

    ASSERT_TRUE(a.GetFlag(ARG_A_SHORT_NAME) == aPresent);
    ASSERT_TRUE(a.GetValue(ARG_B_NAME, val) == bPresent);
    ASSERT_TRUE(a.GetString(ARG_C_SHORT_NAME, str) == cPresent);

    if (bPresent)
    {
        EXPECT_EQ(B_VALUE, val);
    }

    if (cPresent)
    {
        EXPECT_EQ(C_STRING, str);
    }
}

TEST(ArgParser, AllExist)
{
    TestArgCollection(TEST_ARGS_ALL, true, true, true);
}

TEST(ArgParser, MissingA)
{
    TestArgCollection(TEST_ARGS_MISSING_A, false, true, true);
}

TEST(ArgParser, MissingB)
{
    TestArgCollection(TEST_ARGS_MISSING_B, true, false, true);
}

TEST(ArgParser, MissingC)
{
    TestArgCollection(TEST_ARGS_MISSING_C, true, true, false);
}

TEST(ArgParser, PrintUsage)
{
    lkCommon::Utils::ArgParser a;

    ASSERT_TRUE(a.Add(ARG_A_FULL_NAME, lkCommon::Utils::ArgType::FLAG, true, "flag"));
    ASSERT_TRUE(a.Add(ARG_B_FULL_NAME, lkCommon::Utils::ArgType::VALUE, false, "ababababa"));
    ASSERT_TRUE(a.Add(ARG_C_FULL_NAME, lkCommon::Utils::ArgType::STRING, true, "aaaaa"));

    a.PrintUsage();
}

TEST(ArgParser, InvalidArgNames)
{
    lkCommon::Utils::ArgParser a;

    EXPECT_FALSE(a.Add(ARG_INVALID_A_FULL_NAME, lkCommon::Utils::ArgType::FLAG, false));
    EXPECT_FALSE(a.Add(ARG_INVALID_B_FULL_NAME, lkCommon::Utils::ArgType::VALUE, false));
    EXPECT_FALSE(a.Add(ARG_INVALID_C_FULL_NAME, lkCommon::Utils::ArgType::FLAG, false));
    EXPECT_FALSE(a.Add(ARG_INVALID_D_FULL_NAME, lkCommon::Utils::ArgType::STRING, false));
    EXPECT_FALSE(a.Add(ARG_INVALID_E_FULL_NAME, lkCommon::Utils::ArgType::VALUE, false));
}

TEST(ArgParser, MissingRequiredArg)
{
    lkCommon::Utils::ArgParser a;

    ASSERT_TRUE(a.Add(ARG_A_FULL_NAME, lkCommon::Utils::ArgType::FLAG, false));
    ASSERT_TRUE(a.Add(ARG_B_FULL_NAME, lkCommon::Utils::ArgType::VALUE, false));
    ASSERT_TRUE(a.Add(ARG_C_FULL_NAME, lkCommon::Utils::ArgType::STRING, false));

    ASSERT_FALSE(a.Parse(static_cast<int>(TEST_ARGS_MISSING_A.size()), TEST_ARGS_MISSING_A.data()));
}

template <size_t N>
void TestArgVariant(const ArgPair<N>& arglist)
{
    lkCommon::Utils::ArgParser a;

    ASSERT_TRUE(a.Add(ARG_D_FULL_NAME, lkCommon::Utils::ArgType::VALUE, false));

    ASSERT_TRUE(a.Parse(static_cast<int>(arglist.size()), arglist.data()));

    int32_t val;
    EXPECT_TRUE(a.GetValue(ARG_D_NAME, val));
    EXPECT_EQ(D_VALUE, val);
}

TEST(ArgParser, ShortArgV1_Tight)
{
    TestArgVariant(TEST_ARGS_D_TIGHT);
}

TEST(ArgParser, ShortArgV2_WithEquals)
{
    TestArgVariant(TEST_ARGS_D_WITH_EQ);
}

TEST(ArgParser, ShortArgV3_Sparse)
{
    TestArgVariant(TEST_ARGS_D_SPARSE);
}

TEST(ArgParser, LongArgV1_WithEquals)
{
    TestArgVariant(TEST_ARGS_D_LONG_WITH_EQ);
}

TEST(ArgParser, LongArgV2_Sparse)
{
    TestArgVariant(TEST_ARGS_D_LONG_SPARSE);
}
