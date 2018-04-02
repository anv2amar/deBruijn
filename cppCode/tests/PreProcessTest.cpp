//
// Created by Naga Satya Karthik on 12/6/17.
//

#include <gtest/gtest.h>
#include "ctime"
#include "iostream"
#include "../src/PreProcess.h"

#define GENOMEFQ "DeBruijnGraph/datasets/escherichia_coli_k12_mg1655.m130404_014004_sidney_c100506902550000001823076808221337_s1_p0.1.fastq"

namespace {
    class PreProcessTest : public testing::Test {
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
        PreProcessTest() {

        }

        virtual ~PreProcessTest() {

        }
    };
}

TEST_F(PreProcessTest, DISABLED_PrintTest){
    PreProcess *obj = new PreProcess(GENOMEFQ, "DeBruijnGraph/datasets/outputs/reduced.fq");
    obj->printFile();
}


TEST_F(PreProcessTest, TruncateTest){
    string result = "DeBruijnGraph/datasets/outputs/reduced.fq";
    PreProcess *obj = new PreProcess(GENOMEFQ, result);
    obj->truncate(10);
    ifstream resultFile(result);
    ASSERT_TRUE(resultFile.is_open());
    resultFile.close();
}


