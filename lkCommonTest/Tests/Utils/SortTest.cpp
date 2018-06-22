#include <gtest/gtest.h>
#include <lkCommon/Utils/Sort.hpp>
#include <lkCommon/Utils/Timer.hpp>
#include <random>


const std::vector<int> SORTED_UP_INT{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const std::vector<float> SORTED_UP_FLOAT{1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 5.5f};

const std::vector<int> SORTED_DOWN_INT{13, 9, 7, 5, 4, 2, 1, 0};
const std::vector<float> SORTED_DOWN_FLOAT{5.5f, 5.0f, 4.5f, 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.5f, 1.0f};


TEST(QuickSort, SimpleIntArray)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> unsorted = SORTED_UP_INT;
    std::shuffle(unsorted.begin(), unsorted.end(), gen);

    lkCommon::Utils::QuickSort(unsorted);
    ASSERT_TRUE(unsorted == SORTED_UP_INT);
}

TEST(QuickSort, SimpleFloatArray)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<float> unsorted = SORTED_UP_FLOAT;
    std::shuffle(unsorted.begin(), unsorted.end(), gen);

    lkCommon::Utils::QuickSort(unsorted);
    ASSERT_TRUE(unsorted == SORTED_UP_FLOAT);
}

TEST(QuickSort, FloatPerfTest)
{
    lkCommon::Utils::Timer t;
    std::random_device rd;
    std::mt19937 gen(rd());
    const uint32_t REPEAT_COUNT = 10;
    double averageTime = 0.0;

    for (size_t i = 16; i <= 10000; i *= 2)
    {
        std::vector<float> sorted;
        sorted.resize(i);
        for (uint32_t j = 0; j < i; ++j)
            sorted[j] = static_cast<float>(j);

        std::vector<float> unsorted = sorted;

        std::cout << "#" << i << ": ";
        averageTime = 0.0;
        for (uint32_t j = 0; j < REPEAT_COUNT; ++j)
        {
            std::shuffle(unsorted.begin(), unsorted.end(), gen);

            t.Start();
            lkCommon::Utils::QuickSort(unsorted);
            averageTime += t.Stop();

            ASSERT_TRUE(unsorted == sorted);
        }

        std::cout << averageTime / static_cast<double>(REPEAT_COUNT) << std::endl;
    }
}


TEST(BitonicSort, SimpleInt)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> unsorted{7, 1, 4, 2, 9, 13, 0, 5};
    //std::shuffle(unsorted.begin(), unsorted.end(), gen);

    lkCommon::Utils::BitonicSort(unsorted, false);
    ASSERT_TRUE(unsorted == SORTED_DOWN_INT);
}

TEST(BitonicSort, SimpleFloat)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<float> unsorted = SORTED_DOWN_FLOAT;
    std::shuffle(unsorted.begin(), unsorted.end(), gen);

    lkCommon::Utils::BitonicSort(unsorted, false);
    ASSERT_TRUE(unsorted == SORTED_DOWN_FLOAT);
}

TEST(BitonicSort, FloatPerfTest)
{
    lkCommon::Utils::Timer t;
    std::random_device rd;
    std::mt19937 gen(rd());
    const uint32_t REPEAT_COUNT = 10;
    double averageTime = 0.0;

    for (size_t i = 16; i <= 10000; i <<= 1)
    {
        std::vector<float> sorted;
        sorted.resize(i);
        for (uint32_t j = 0; j < i; ++j)
            sorted[j] = static_cast<float>(j);

        std::vector<float> unsorted = sorted;

        std::cout << "#" << i << ": ";
        averageTime = 0.0;
        for (uint32_t j = 0; j < REPEAT_COUNT; ++j)
        {
            std::shuffle(unsorted.begin(), unsorted.end(), gen);

            t.Start();
            lkCommon::Utils::BitonicSort(unsorted, true);
            averageTime += t.Stop();

            ASSERT_TRUE(unsorted == sorted);
        }

        std::cout << averageTime / static_cast<double>(REPEAT_COUNT) << std::endl;
    }
}
