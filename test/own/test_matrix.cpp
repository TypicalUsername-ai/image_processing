//
// Created by matid on 6/5/2022.
//

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "improc.hpp"

//------------------------------- 2D ARRAY / VECTOR TESTS [recompile needed] ------------------------------------------

TEST(Matrix_2D, getter_rows){
    Matrix<int> m1 = Matrix(3,2,3);
    EXPECT_EQ(m1.get_nrows(), 3);
}

TEST(Matrix_2D, getter_cols){
    Matrix<int> m1 = Matrix(3,2,3);
    EXPECT_EQ(m1.get_ncols(), 2);
}

TEST(Matrix_2D, operator_test){
    Matrix<int> m1 = Matrix(3,2,4);
    EXPECT_EQ(m1[1][1], 4);
}

TEST(Matrix_2D, constructor){
    Matrix<int> m1 = Matrix(2,2,3);
    for(std::size_t row = 0; row < 2; row ++){
        for(std::size_t col = 0; col < 2; col++){
            EXPECT_EQ(m1[row][col], 3);
        }
    }
}

TEST(Matrix_2D, strict_val_constructor){
#if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
    MatrixData<int> data_1{{1, 2},{3, 4}};
#else
    MatrixData<int> data_1 = new int*[2];
    data_1[0] = new int[2];
    data_1[0][0] = 1;
    data_1[0][1] = 2;
    data_1[1] = new int[2];
    data_1[1][0] = 3;
    data_1[1][1] = 4;
#endif

    Matrix<int> m1 = Matrix(2,2, data_1);
    EXPECT_EQ(m1[0][0], 1);
    EXPECT_EQ(m1[0][1], 2);
    EXPECT_EQ(m1[1][0], 3);
    EXPECT_EQ(m1[1][1], 4);
}

TEST(Matrix_2D, copy_constructor){
    Matrix<int> m1 = Matrix(3,2,4);
    Matrix<int> m2 = Matrix(m1);
    EXPECT_EQ(m1[1][1], m2[1][1]);
    EXPECT_EQ(m1.get_ncols(), m2.get_ncols());
    EXPECT_EQ(m1.get_nrows(), m2.get_nrows());
}

TEST(Matrix_2D, byte_single_operator){
    Matrix<byte> m1 = Matrix(2, 2, (byte) 0);
    m1[0] = {1,1};

    EXPECT_EQ(m1[0][0], 1);
    EXPECT_EQ(m1[0][1], 1);
}

//fixme nested operators dont work on vectors

// they work on array 2d however
TEST(MATRIX_2D, byte_nested_operators){
    Matrix<byte> m1 = Matrix(2, 2, (byte) 0);
    m1[1][1] = 2;
    EXPECT_EQ(m1[1][1], 2);
}