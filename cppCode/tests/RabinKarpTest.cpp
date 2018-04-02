//
// Created by Naga Satya Karthik on 11/10/17.
//
#include <gtest/gtest.h>

#include "../src/RabinKarpHash.h"


namespace {
    class RabinKarpHashTest : public testing::Test {
    protected:
        time_t b, e;
        virtual void SetUp() {
            time(&b);
        }

        virtual void TearDown() {
            time(&e);
            printf ("time taken for this Test Case is: %.2lf seconds.\n", difftime(e,b));
        }
    public:
        RabinKarpHash* sut;

        RabinKarpHashTest() {
            sut = new RabinKarpHash(4);
        }

        virtual ~RabinKarpHashTest() {
            delete sut;
        }
    };
}

TEST_F(RabinKarpHashTest, hashValueOfATGC) {
    ASSERT_EQ(sut->rabinkarpHash("ATGC"), 27);
//    ASSERT_EQ(sut->rabinkarpHash("TGC"), 27);
    ASSERT_EQ(sut->rabinkarpHash("LFAJFLAJFF"), -1);
}

TEST_F(RabinKarpHashTest, FirstCharID){
    ASSERT_EQ(sut->getFirstChar(sut->rabinkarpHash("TGCA")), 'T');
    ASSERT_EQ(sut->getFirstChar(sut->rabinkarpHash("ATGC")), 'A');
}

TEST_F(RabinKarpHashTest, LastCharID){
    ASSERT_EQ(sut->getLastChar(sut->rabinkarpHash("TGCA")), 'A');
    ASSERT_EQ(sut->getLastChar(sut->rabinkarpHash("ATGC")), 'C');
}

TEST_F(RabinKarpHashTest, charToIMapping){
    ASSERT_EQ(sut->genToI('A'), 0);
    ASSERT_EQ(sut->genToI('T'), 1);
    ASSERT_EQ(sut->genToI('G'), 2);
    ASSERT_EQ(sut->genToI('C'), 3);
    ASSERT_EQ(sut->genToI('a'), -1);
}
