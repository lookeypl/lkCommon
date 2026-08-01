#include <gtest/gtest.h>

#include <Linux/XConnection.hpp>

#include <xcb/xcb.h>


TEST(XConnection, ConnectionPointer)
{
    ASSERT_NE(nullptr, lkCommon::Internal::XConnection::Instance().GetConnection());
    ASSERT_EQ(0, xcb_connection_has_error(lkCommon::Internal::XConnection::Instance().GetConnection()));
}

TEST(XConnection, ScreenPointer)
{
    ASSERT_NE(nullptr, lkCommon::Internal::XConnection::Instance().GetScreen());
}
