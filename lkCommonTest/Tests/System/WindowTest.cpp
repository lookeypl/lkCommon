#include <gtest/gtest.h>
#include <lkCommon/System/Window.hpp>

#include <thread>
#include <chrono>


const uint32_t TEST_WINDOW_WIDTH = 200;
const uint32_t TEST_WINDOW_HEIGHT = 200;
const std::string TEST_WINDOW_NAME = "lkCommonTest window";
const float TEST_WINDOW_UPDATE_DELTA_TIME = 1.0f;

class CallbackTestWindow: public lkCommon::Window
{
    bool mOnInitCalled;
    bool mOnOpenCalled;
    bool mOnCloseCalled;
    bool mOnUpdateCalled;

protected:
    void OnInit() override
    {
        mOnInitCalled = true;
    }

    void OnOpen() override
    {
        mOnOpenCalled = true;
    }

    void OnClose() override
    {
        mOnCloseCalled = true;
    }

    void OnUpdate(float deltaTime) override
    {
        EXPECT_EQ(deltaTime, TEST_WINDOW_UPDATE_DELTA_TIME);
        mOnUpdateCalled = true;
    }

    /* TODO testing event callbacks requires a platform-specific test class
            which will emit these events to our Window class
    void OnKeyDown(KeyCode key) override
    {
    }

    void OnKeyUp(KeyCode key) override
    {
    }

    void OnMouseDown(int key) override
    {
    }

    void OnMouseMove(int x, int y, int deltaX, int deltaY) override
    {
    }

    void OnMouseUp(int key) override
    {
    }
    */

public:
    CallbackTestWindow()
        : lkCommon::Window()
        , mOnInitCalled(false)
        , mOnOpenCalled(false)
        , mOnCloseCalled(false)
        , mOnUpdateCalled(false)
    {
    }

    bool OnInitCalled()
    {
        return mOnInitCalled;
    }

    bool OnOpenCalled()
    {
        return mOnOpenCalled;
    }

    bool OnCloseCalled()
    {
        return mOnCloseCalled;
    }

    bool OnUpdateCalled()
    {
        return mOnUpdateCalled;
    }
};

TEST(Window, Constructor)
{
    lkCommon::Window w;
    EXPECT_FALSE(w.IsOpen());
}

TEST(Window, Init)
{
    lkCommon::Window w;
    EXPECT_TRUE(w.Init());
}

TEST(Window, OpenWithoutInit)
{
    lkCommon::Window w;
    w.SetInvisible(true);
    EXPECT_FALSE(w.Open(0, 0, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));
}

TEST(Window, Open)
{
    lkCommon::Window w;
    w.SetInvisible(true);
    EXPECT_TRUE(w.Init());
    EXPECT_FALSE(w.IsOpen());
    EXPECT_TRUE(w.Open(0, 0, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));
    EXPECT_TRUE(w.IsOpen());
}

TEST(Window, OnInitCallback)
{
    CallbackTestWindow w;
    EXPECT_FALSE(w.OnInitCalled());
    w.Init();
    EXPECT_TRUE(w.OnInitCalled());
}

TEST(Window, OnOpenCallback)
{
    // just-created window shouldn't call any callbacks
    CallbackTestWindow w;
    EXPECT_FALSE(w.OnInitCalled());
    EXPECT_FALSE(w.OnOpenCalled());

    // Init should call OnInit only
    w.Init();
    EXPECT_TRUE(w.OnInitCalled());
    EXPECT_FALSE(w.OnOpenCalled());

    // Open should call OnOpen
    w.SetInvisible(true);
    EXPECT_TRUE(w.Open(0, 0, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));
    EXPECT_TRUE(w.OnOpenCalled());
}

TEST(Window, OnUpdateCallback)
{
    CallbackTestWindow w;
    EXPECT_FALSE(w.OnUpdateCalled());

    EXPECT_TRUE(w.Init());
    EXPECT_FALSE(w.OnUpdateCalled());

    w.SetInvisible(true);
    EXPECT_TRUE(w.Open(0, 0, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));
    EXPECT_FALSE(w.OnUpdateCalled());

    w.Update(TEST_WINDOW_UPDATE_DELTA_TIME);
    EXPECT_TRUE(w.OnUpdateCalled());
}

TEST(Window, OnCloseCallback)
{
    CallbackTestWindow w;
    EXPECT_FALSE(w.OnCloseCalled());

    EXPECT_TRUE(w.Init());
    EXPECT_FALSE(w.OnCloseCalled());

    // Calling Close() without Open() should not trigger OnClose()
    w.Close();
    EXPECT_FALSE(w.OnCloseCalled());

    w.SetInvisible(true);
    EXPECT_TRUE(w.Open(0, 0, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));
    EXPECT_FALSE(w.OnCloseCalled());

    w.Close();
    EXPECT_TRUE(w.OnCloseCalled());
}

TEST(Window, DisplayImageTest)
{
    lkCommon::Window w;

    EXPECT_TRUE(w.Init());

    w.SetInvisible(true);
    EXPECT_TRUE(w.Open(100, 100, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));

    lkCommon::Image i;
    EXPECT_TRUE(i.Resize(TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT));

    for (uint32_t x = 0; x < TEST_WINDOW_WIDTH; ++x)
    {
        for (uint32_t y = 0; y < TEST_WINDOW_HEIGHT; ++y)
        {
            float r = static_cast<float>(x) / static_cast<float>(TEST_WINDOW_WIDTH) * 255.0f;
            float g = static_cast<float>(y) / static_cast<float>(TEST_WINDOW_HEIGHT) * 255.0f;
            i.SetPixel(x, y, static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(0));
        }
    }

    EXPECT_TRUE(w.DisplayImage(0, 0, i));
}

TEST(Window, DisplayImageTooBigTest)
{
    lkCommon::Window w;

    EXPECT_TRUE(w.Init());

    w.SetInvisible(true);
    EXPECT_TRUE(w.Open(100, 100, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));

    lkCommon::Image i;
    EXPECT_TRUE(i.Resize(TEST_WINDOW_WIDTH + 1, TEST_WINDOW_HEIGHT + 1));

    // contents don't matter, the function should fail even before displaying anything
    EXPECT_FALSE(w.DisplayImage(0, 0, i));
}

TEST(Window, DisplayImageOutsideBoundsTest)
{
    lkCommon::Window w;

    EXPECT_TRUE(w.Init());

    w.SetInvisible(true);
    EXPECT_TRUE(w.Open(100, 100, TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, TEST_WINDOW_NAME));

    lkCommon::Image i;
    EXPECT_TRUE(i.Resize(TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT));

    // contents don't matter, the function should fail even before displaying anything
    EXPECT_FALSE(w.DisplayImage(1, 1, i));
}
