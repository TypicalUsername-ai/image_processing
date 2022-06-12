//
// Created by matid on 6/7/2022.
//
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "improc.hpp"
//FIXME fix the nullptr change stuff

TEST(Image_test, empty_constructor){
    EXPECT_NO_THROW(Image im(10, 10, nullptr, BITMAPFILEHEADER()););
}

TEST(Image_test, data_constructor){
#if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
        MatrixData<byte> m1{{(byte) 1, (byte) 2},{ (byte) 1, (byte) 2}};
#else
        MatrixData<byte> m1 = new byte*[2];
        for(std::size_t i = 0; i < 2; i++){
            m1[i] = new byte[2];
            m1[i][0] = (byte) 1;
            m1[i][1] = (byte) 2;
        }
#endif

    EXPECT_NO_THROW(Image im(2, 2, nullptr, BITMAPFILEHEADER(), m1););
}

TEST(Image_test, get_header){
    Image im(2, 2, nullptr, BITMAPFILEHEADER());
    EXPECT_NO_THROW(im.get_bitmapheader());
}

TEST(Image_test, get_info){
    Image im(2, 2, nullptr, BITMAPFILEHEADER());
    EXPECT_EQ(im.get_bitmapinfo(), nullptr);
}

TEST(Image_test, get_pixels){

#if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
    MatrixData<byte> m1{{(byte) 1, (byte) 2},{(byte) 1, (byte) 2}};
#else
    MatrixData<byte> m1 = new byte*[2];
    for(std::size_t i = 0; i < 2; i++){
        m1[i] = new byte[2];
        m1[i][0] = (byte) 1;
        m1[i][1] = (byte) 2;
    }
#endif
    Image im(2, 2, nullptr, BITMAPFILEHEADER(), m1);
    EXPECT_EQ(im[0][0], (byte) 1);
    EXPECT_EQ(im[0][1], (byte) 2);
    EXPECT_EQ(im[1][0], (byte) 1);
    EXPECT_EQ(im[1][1], (byte) 2);

}

TEST(Image_test, copy_constructor){
#if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
    MatrixData<byte> m1{{(byte) 1, (byte) 2},{(byte) 1, (byte) 2}};
#else
    MatrixData<byte> m1 = new byte*[2];
    for(std::size_t i = 0; i < 2; i++){
        m1[i] = new byte[2];
        m1[i][0] = (byte) 1;
        m1[i][1] = (byte) 2;
    }
#endif
    Image im(2, 2, nullptr , BITMAPFILEHEADER(), m1);
    Image im2(im);
    //ASSERT_NE( (long unsigned int) im2.get_matrix(), (long unsigned int) im.get_matrix());
    EXPECT_EQ(im2[0][0], (byte) 1);
    EXPECT_EQ(im2[0][1], (byte) 2);
    EXPECT_EQ(im2[1][0], (byte) 1);
    EXPECT_EQ(im2[1][1], (byte) 2);
}

TEST(Image_test, nested_operators_assignment){
    #if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
    MatrixData<byte> m1{{(byte) 1, (byte) 2},{ (byte) 1, (byte) 2}};
    #else
    MatrixData<byte> m1 = new byte*[2];
        for(std::size_t i = 0; i < 2; i++){
            m1[i] = new byte[2];
            m1[i][0] = (byte) 1;
            m1[i][1] = (byte) 2;
        }
    #endif
    Image im(2, 2, nullptr , BITMAPFILEHEADER(), m1);
    Image im2(im);
    (im2[1])[1] = (byte) 3;
    //ASSERT_NE( (long unsigned int) im2.get_matrix(), (long unsigned int) im.get_matrix());
    EXPECT_EQ(im2[0][0], (byte) 1);
    EXPECT_EQ(im2[0][1], (byte) 2);
    EXPECT_EQ(im2[1][0], (byte) 1);
    EXPECT_EQ(im2[1][1], (byte) 3);
}

