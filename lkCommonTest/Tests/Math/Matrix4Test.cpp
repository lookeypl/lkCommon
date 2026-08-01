#include <gtest/gtest.h>
#include <lkCommon/Math/Matrix4.hpp>

using namespace lkCommon::Math;


const float TEST_MATRIX_ZERO_RAW[16] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
};

const float TEST_MATRIX_ONE_RAW[16] = {
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
};

#define TEST_TO_DO EXPECT_TRUE(false) << "Test not yet implemented!"


// helper macro to unclutter test code
#define CREATE_TEST_MATRIX(n, a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4) \
const float n##_RAW[16] = { \
    a1, b1, c1, d1, \
    a2, b2, c2, d2, \
    a3, b3, c3, d3, \
    a4, b4, c4, d4, \
}; \
const Vector4 n##_VEC[4] = { \
    Vector4(&n##_RAW[0]), \
    Vector4(&n##_RAW[4]), \
    Vector4(&n##_RAW[8]), \
    Vector4(&n##_RAW[12]), \
}; \
const Matrix4 n(n##_RAW);


const float TEST_FLOAT = 2.0f;

CREATE_TEST_MATRIX(TEST_MATRIX_1,
                   1.0f, 2.0f, 3.0f, 4.0f,
                   5.0f, 6.0f, 7.0f, 8.0f,
                   8.0f, 7.0f, 6.0f, 5.0f,
                   4.0f, 3.0f, 2.0f, 1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_2,
                   8.0f, 7.0f, 6.0f, 5.0f,
                   4.0f, 3.0f, 2.0f, 1.0f,
                   1.0f, 2.0f, 3.0f, 4.0f,
                   5.0f, 6.0f, 7.0f, 8.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_PLUS_FLOAT,
                    3.0f, 4.0f, 5.0f,  6.0f,
                    7.0f, 8.0f, 9.0f, 10.0f,
                   10.0f, 9.0f, 8.0f,  7.0f,
                    6.0f, 5.0f, 4.0f,  3.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_PLUS_MATRIX_2,
                   9.0f, 9.0f, 9.0f, 9.0f,
                   9.0f, 9.0f, 9.0f, 9.0f,
                   9.0f, 9.0f, 9.0f, 9.0f,
                   9.0f, 9.0f, 9.0f, 9.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_MINUS_FLOAT,
                   -1.0f, 0.0f, 1.0f,  2.0f,
                    3.0f, 4.0f, 5.0f,  6.0f,
                    6.0f, 5.0f, 4.0f,  3.0f,
                    2.0f, 1.0f, 0.0f, -1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_MINUS_MATRIX_2,
                   -7.0f, -5.0f, -3.0f, -1.0f,
                    1.0f,  3.0f,  5.0f,  7.0f,
                    7.0f,  5.0f,  3.0f,  1.0f,
                   -1.0f, -3.0f, -5.0f, -7.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_MUL_FLOAT,
                    2.0f,  4.0f,  6.0f,  8.0f,
                   10.0f, 12.0f, 14.0f, 16.0f,
                   16.0f, 14.0f, 12.0f, 10.0f,
                    8.0f,  6.0f,  4.0f,  2.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_MUL_MATRIX_2,
                    39.0f,  43.0f,  47.0f,  51.0f,
                   111.0f, 115.0f, 119.0f, 123.0f,
                   123.0f, 119.0f, 115.0f, 111.0f,
                    51.0f,  47.0f,  43.0f,  39.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_1_TRANSPOSED,
                   1.0f, 5.0f, 8.0f, 4.0f,
                   2.0f, 6.0f, 7.0f, 3.0f,
                   3.0f, 7.0f, 6.0f, 2.0f,
                   4.0f, 8.0f, 5.0f, 1.0f)

const Vector4 TEST_VECTOR_POS(-1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 TEST_VECTOR_AT(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 TEST_VECTOR_UP(0.0f, 1.0f, 0.0f, 0.0f);
CREATE_TEST_MATRIX(TEST_MATRIX_RH_LOOK_AT,
                   1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_RH_PROJECTION,
                   1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f)

const float TEST_FLOAT_ROT_ANGLE = LKCOMMON_PIF / 2.0f;
const float TEST_FLOAT_ROT_ANGLE_SIN = sinf(TEST_FLOAT_ROT_ANGLE);
const float TEST_FLOAT_ROT_ANGLE_COS = cosf(TEST_FLOAT_ROT_ANGLE);
CREATE_TEST_MATRIX(TEST_MATRIX_ROTATION_X,
                   1.0f,                     0.0f,                      0.0f, 0.0f,
                   0.0f, TEST_FLOAT_ROT_ANGLE_COS, -TEST_FLOAT_ROT_ANGLE_SIN, 0.0f,
                   0.0f, TEST_FLOAT_ROT_ANGLE_SIN,  TEST_FLOAT_ROT_ANGLE_COS, 0.0f,
                   0.0f,                     0.0f,                      0.0f, 1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_ROTATION_Y,
                    TEST_FLOAT_ROT_ANGLE_COS, 0.0f, TEST_FLOAT_ROT_ANGLE_SIN, 0.0f,
                                        0.0f, 1.0f,                     0.0f, 0.0f,
                   -TEST_FLOAT_ROT_ANGLE_SIN, 0.0f, TEST_FLOAT_ROT_ANGLE_COS, 0.0f,
                                        0.0f, 0.0f,                     0.0f, 1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_ROTATION_Z,
                   TEST_FLOAT_ROT_ANGLE_COS, -TEST_FLOAT_ROT_ANGLE_SIN, 0.0f, 0.0f,
                   TEST_FLOAT_ROT_ANGLE_SIN,  TEST_FLOAT_ROT_ANGLE_COS, 0.0f, 0.0f,
                                       0.0f,                      0.0f, 1.0f, 0.0f,
                                       0.0f,                      0.0f, 0.0f, 1.0f)

const float TEST_FLOAT_TRANSLATION_X =  1.0f;
const float TEST_FLOAT_TRANSLATION_Y =  3.0f;
const float TEST_FLOAT_TRANSLATION_Z = -2.0f;
const Vector4 TEST_VECTOR_TRANSLATION(TEST_FLOAT_TRANSLATION_X,
                                      TEST_FLOAT_TRANSLATION_Y,
                                      TEST_FLOAT_TRANSLATION_Z,
                                      1.0f);
CREATE_TEST_MATRIX(TEST_MATRIX_TRANSLATION,
                   1.0f, 0.0f, 0.0f, TEST_FLOAT_TRANSLATION_X,
                   0.0f, 1.0f, 0.0f, TEST_FLOAT_TRANSLATION_Y,
                   0.0f, 0.0f, 1.0f, TEST_FLOAT_TRANSLATION_Z,
                   0.0f, 0.0f, 0.0f,                     1.0f)

const float TEST_FLOAT_SCALE_X = 2.0f;
const float TEST_FLOAT_SCALE_Y = 4.0f;
const float TEST_FLOAT_SCALE_Z = 0.5f;
const Vector4 TEST_VECTOR_SCALE(TEST_FLOAT_SCALE_X, TEST_FLOAT_SCALE_Y, TEST_FLOAT_SCALE_Z, 1.0f);
CREATE_TEST_MATRIX(TEST_MATRIX_SCALE_SINGLE_FLOAT,
                   TEST_FLOAT_SCALE_X,               0.0f,               0.0f, 0.0f,
                                 0.0f, TEST_FLOAT_SCALE_X,               0.0f, 0.0f,
                                 0.0f,               0.0f, TEST_FLOAT_SCALE_X, 0.0f,
                                 0.0f,               0.0f,               0.0f, 1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_SCALE,
                   TEST_FLOAT_SCALE_X,               0.0f,               0.0f, 0.0f,
                                 0.0f, TEST_FLOAT_SCALE_Y,               0.0f, 0.0f,
                                 0.0f,               0.0f, TEST_FLOAT_SCALE_Z, 0.0f,
                                 0.0f,               0.0f,               0.0f, 1.0f)

CREATE_TEST_MATRIX(TEST_MATRIX_IDENTITY,
                   1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f)

const Vector4 TEST_VECTOR(1.0f, 2.0f, 3.0f, 4.0f);
const Vector4 TEST_MATRIX_1_MUL_VECTOR(30.0f, 70.0f, 60.0f, 20.0f);


void ExpectMatrixEqual(const float ref[], const Matrix4& m)
{
    for (uint32_t i = 0; i < 4; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            EXPECT_EQ(ref[j + (i * 4)], m[i][j]) << "Matrix coordinates are " << i << "x" << j;
        }
    }
}


TEST(Matrix4, ConstructorDefault)
{
    Matrix4 m;
    ExpectMatrixEqual(TEST_MATRIX_ZERO_RAW, m);
}

TEST(Matrix4, ConstructorFloat)
{
    Matrix4 m(1.0f);
    ExpectMatrixEqual(TEST_MATRIX_ONE_RAW, m);
}

TEST(Matrix4, ConstructorFloatArray)
{
    Matrix4 m(TEST_MATRIX_1_RAW);
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, ConstructorFloat16)
{
    Matrix4 m(TEST_MATRIX_1_RAW[0], TEST_MATRIX_1_RAW[4],  TEST_MATRIX_1_RAW[8], TEST_MATRIX_1_RAW[12],
              TEST_MATRIX_1_RAW[1], TEST_MATRIX_1_RAW[5],  TEST_MATRIX_1_RAW[9], TEST_MATRIX_1_RAW[13],
              TEST_MATRIX_1_RAW[2], TEST_MATRIX_1_RAW[6], TEST_MATRIX_1_RAW[10], TEST_MATRIX_1_RAW[14],
              TEST_MATRIX_1_RAW[3], TEST_MATRIX_1_RAW[7], TEST_MATRIX_1_RAW[11], TEST_MATRIX_1_RAW[15]);
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, ConstructorVector)
{
    Matrix4 m(Vector4(TEST_MATRIX_1_RAW[0],  TEST_MATRIX_1_RAW[1],  TEST_MATRIX_1_RAW[2],  TEST_MATRIX_1_RAW[3]),
              Vector4(TEST_MATRIX_1_RAW[4],  TEST_MATRIX_1_RAW[5],  TEST_MATRIX_1_RAW[6],  TEST_MATRIX_1_RAW[7]),
              Vector4(TEST_MATRIX_1_RAW[8],  TEST_MATRIX_1_RAW[9],  TEST_MATRIX_1_RAW[10], TEST_MATRIX_1_RAW[11]),
              Vector4(TEST_MATRIX_1_RAW[12], TEST_MATRIX_1_RAW[13], TEST_MATRIX_1_RAW[14], TEST_MATRIX_1_RAW[15]));
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, Constructor__m128)
{
    // Idx's are inverted because of how _mm_set_ps works
    Matrix4 m(_mm_set_ps(TEST_MATRIX_1_RAW[3],  TEST_MATRIX_1_RAW[2],  TEST_MATRIX_1_RAW[1],  TEST_MATRIX_1_RAW[0]),
              _mm_set_ps(TEST_MATRIX_1_RAW[7],  TEST_MATRIX_1_RAW[6],  TEST_MATRIX_1_RAW[5],  TEST_MATRIX_1_RAW[4]),
              _mm_set_ps(TEST_MATRIX_1_RAW[11], TEST_MATRIX_1_RAW[10], TEST_MATRIX_1_RAW[9],  TEST_MATRIX_1_RAW[8]),
              _mm_set_ps(TEST_MATRIX_1_RAW[15], TEST_MATRIX_1_RAW[14], TEST_MATRIX_1_RAW[13], TEST_MATRIX_1_RAW[12]));
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, ConstructorCopy)
{
    Matrix4 m(TEST_MATRIX_1);
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, ConstructorMove)
{
    Matrix4 temp(TEST_MATRIX_1);
    Matrix4 m(std::move(temp));
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, AssignmentCopy)
{
    Matrix4 m;
    m = TEST_MATRIX_1;
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, AssignmentMove)
{
    Matrix4 temp(TEST_MATRIX_1);
    Matrix4 m;
    m = std::move(temp);
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, Data)
{
    Matrix4 m(TEST_MATRIX_1_RAW);
    // swap the arguments here for convenience sake
    ExpectMatrixEqual(m.Data(), TEST_MATRIX_1);
}

TEST(Matrix4, AccessVariable)
{
    Matrix4 m(TEST_MATRIX_1_RAW);
    EXPECT_EQ(TEST_MATRIX_1_VEC[0], m[0]);
    EXPECT_EQ(TEST_MATRIX_1_VEC[1], m[1]);
    EXPECT_EQ(TEST_MATRIX_1_VEC[2], m[2]);
    EXPECT_EQ(TEST_MATRIX_1_VEC[3], m[3]);
}

TEST(Matrix4, AccessConst)
{
    const Matrix4 m(TEST_MATRIX_1_RAW);
    EXPECT_EQ(TEST_MATRIX_1_VEC[0], m[0]);
    EXPECT_EQ(TEST_MATRIX_1_VEC[1], m[1]);
    EXPECT_EQ(TEST_MATRIX_1_VEC[2], m[2]);
    EXPECT_EQ(TEST_MATRIX_1_VEC[3], m[3]);
}

TEST(Matrix4, AddAssignmentMatrix)
{
    Matrix4 m(TEST_MATRIX_1);
    m += TEST_MATRIX_2;
    ExpectMatrixEqual(TEST_MATRIX_1_PLUS_MATRIX_2_RAW, m);
}

TEST(Matrix4, AddAssignmentFloat)
{
    Matrix4 m(TEST_MATRIX_1);
    m += TEST_FLOAT;
    ExpectMatrixEqual(TEST_MATRIX_1_PLUS_FLOAT_RAW, m);
}

TEST(Matrix4, AddMatrix)
{
    Matrix4 m1(TEST_MATRIX_1);
    Matrix4 m2(TEST_MATRIX_2);
    Matrix4 res = m1 + m2;
    ExpectMatrixEqual(TEST_MATRIX_1_PLUS_MATRIX_2_RAW, res);
}

TEST(Matrix4, AddFloat)
{
    Matrix4 m(TEST_MATRIX_1);
    float f = TEST_FLOAT;
    Matrix4 res = m + f;
    ExpectMatrixEqual(TEST_MATRIX_1_PLUS_FLOAT_RAW, res);
}

TEST(Matrix4, SubtractAssignmentMatrix)
{
    Matrix4 m(TEST_MATRIX_1);
    m -= TEST_MATRIX_2;
    ExpectMatrixEqual(TEST_MATRIX_1_MINUS_MATRIX_2_RAW, m);
}

TEST(Matrix4, SubtractAssignmentFloat)
{
    Matrix4 m(TEST_MATRIX_1);
    m -= TEST_FLOAT;
    ExpectMatrixEqual(TEST_MATRIX_1_MINUS_FLOAT_RAW, m);
}

TEST(Matrix4, SubtractMatrix)
{
    Matrix4 m1(TEST_MATRIX_1);
    Matrix4 m2(TEST_MATRIX_2);
    Matrix4 res = m1 - m2;
    ExpectMatrixEqual(TEST_MATRIX_1_MINUS_MATRIX_2_RAW, res);
}

TEST(Matrix4, SubtractFloat)
{
    Matrix4 m(TEST_MATRIX_1);
    float f = TEST_FLOAT;
    Matrix4 res = m - f;
    ExpectMatrixEqual(TEST_MATRIX_1_MINUS_FLOAT_RAW, res);
}

TEST(Matrix4, MultiplyAssignmentMatrix)
{
    Matrix4 m(TEST_MATRIX_1);
    m *= TEST_MATRIX_2;
    ExpectMatrixEqual(TEST_MATRIX_1_MUL_MATRIX_2_RAW, m);
}

TEST(Matrix4, MultiplyAssignmentFloat)
{
    Matrix4 m(TEST_MATRIX_1);
    m *= TEST_FLOAT;
    ExpectMatrixEqual(TEST_MATRIX_1_MUL_FLOAT_RAW, m);
}

TEST(Matrix4, MultiplyMatrix)
{
    Matrix4 m1(TEST_MATRIX_1);
    Matrix4 m2(TEST_MATRIX_2);
    Matrix4 res = m1 * m2;
    ExpectMatrixEqual(TEST_MATRIX_1_MUL_MATRIX_2_RAW, res);
}

TEST(Matrix4, MultiplyFloat)
{
    Matrix4 m(TEST_MATRIX_1);
    float f = TEST_FLOAT;
    Matrix4 res = m * f;
    ExpectMatrixEqual(TEST_MATRIX_1_MUL_FLOAT_RAW, res);
}

TEST(Matrix4, DivideAssignmentFloat)
{
    Matrix4 m(TEST_MATRIX_1_MUL_FLOAT);
    m /= TEST_FLOAT;
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, m);
}

TEST(Matrix4, DivideFloat)
{
    Matrix4 m(TEST_MATRIX_1_MUL_FLOAT);
    float f = TEST_FLOAT;
    Matrix4 res = m / f;
    ExpectMatrixEqual(TEST_MATRIX_1_RAW, res);
}

TEST(Matrix4, CompareEqual)
{
    Matrix4 m(TEST_MATRIX_1);
    EXPECT_EQ(TEST_MATRIX_1, m);
}

TEST(Matrix4, CompareNotEqual)
{
    Matrix4 m(TEST_MATRIX_1);
    EXPECT_NE(TEST_MATRIX_2, m);
}

TEST(Matrix4, Transpose)
{
    Matrix4 m(TEST_MATRIX_1_RAW);
    ExpectMatrixEqual(TEST_MATRIX_1_TRANSPOSED_RAW, m.Transpose());
}

TEST(Matrix4, Determinant)
{
    TEST_TO_DO;
}

TEST(Matrix4, Invert)
{
    TEST_TO_DO;
}

TEST(Matrix4, MultiplyMatrixVector)
{
    Matrix4 m(TEST_MATRIX_1);
    EXPECT_EQ(TEST_MATRIX_1_MUL_VECTOR, m * TEST_VECTOR);
}

TEST(Matrix4, CreateRHLookAt)
{
    TEST_TO_DO;
}

TEST(Matrix4, CreateRHProjection)
{
    TEST_TO_DO;
}

TEST(Matrix4, CreateRotationX)
{
    Matrix4 m = Matrix4::CreateRotationX(TEST_FLOAT_ROT_ANGLE);
    ExpectMatrixEqual(TEST_MATRIX_ROTATION_X_RAW, m);
}

TEST(Matrix4, CreateRotationY)
{
    Matrix4 m = Matrix4::CreateRotationY(TEST_FLOAT_ROT_ANGLE);
    ExpectMatrixEqual(TEST_MATRIX_ROTATION_Y_RAW, m);
}

TEST(Matrix4, CreateRotationZ)
{
    Matrix4 m = Matrix4::CreateRotationZ(TEST_FLOAT_ROT_ANGLE);
    ExpectMatrixEqual(TEST_MATRIX_ROTATION_Z_RAW, m);
}

TEST(Matrix4, CreateTranslation)
{
    Matrix4 m = Matrix4::CreateTranslation(TEST_VECTOR_TRANSLATION);
    ExpectMatrixEqual(TEST_MATRIX_TRANSLATION_RAW, m);
}

TEST(Matrix4, CreateScaleFloat)
{
    Matrix4 m = Matrix4::CreateScale(TEST_FLOAT_SCALE_X);
    ExpectMatrixEqual(TEST_MATRIX_SCALE_SINGLE_FLOAT_RAW, m);
}

TEST(Matrix4, CreateScaleFloatXYZ)
{
    Matrix4 m = Matrix4::CreateScale(TEST_FLOAT_SCALE_X, TEST_FLOAT_SCALE_Y, TEST_FLOAT_SCALE_Z);
    ExpectMatrixEqual(TEST_MATRIX_SCALE_RAW, m);
}

TEST(Matrix4, CreateScaleVector)
{
    Matrix4 m = Matrix4::CreateScale(TEST_VECTOR_SCALE);
    ExpectMatrixEqual(TEST_MATRIX_SCALE_RAW, m);
}

TEST(Matrix4, Constants_MATRIX_IDENTITY)
{
    ExpectMatrixEqual(TEST_MATRIX_IDENTITY_RAW, Matrix4::IDENTITY);
}

