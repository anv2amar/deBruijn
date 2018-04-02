//
// Created by Naga Satya Karthik on 11/13/17.
//



#include <gtest/gtest.h>
#include "ctime"
#include "../src/ConstructKMers.h"
#include "../src/BBHash.h"


#define GENOMEFA "DeBruijnGraph/datasets/genome.fa"

namespace {
    class BBHashTest : public testing::Test {
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
        int k;
        ConstructKMers* sut;
        RabinKarpHash* rkhash;
        BBHashExt* bbHashExt;
        BBHashTest() {
            k = 15;
            sut = new ConstructKMers(k);
            rkhash = new RabinKarpHash(k);
        }

        virtual ~BBHashTest() {
            delete sut;
            delete rkhash;
        }
    };
}

TEST_F(BBHashTest, getMPHF) {
    // GIven an long long returns the index of the mphf
    //CHeck for unique ness.
    vector<long long> input_keys = {1,2,3,4,5,6,7,10,20,30,40,50};
    bbHashExt = new BBHashExt(input_keys);
    set<uint64_t> hashSet;
    u_int64_t idx;
//    printf("Size of the keys: %d",(int)bbHashExt->n);
    for(long long rkhash: input_keys){
        idx = bbHashExt->getMPHF(rkhash);
        printf("RKHash: %lld, Value(IDX): %lu\n", rkhash, idx);
    }

    printf("yoyoyoyoyoyyoyoyo %lu\n", bbHashExt->getMPHF(60));
}


TEST_F(BBHashTest, getMPHFFullDataSet) {
    // GIven an long long returns the index of the mphf
    //CHeck for unique ness.
    vector<string> kmers = sut->getKMers(GENOMEFA);
    vector<long long> kmersHashValues = sut->getRKHashMaps(kmers);
    printf("KMERS size: %lu\n", kmersHashValues.size());
    time_t b, e;
    time(&b);
    bbHashExt = new BBHashExt(kmersHashValues);
    time(&e);
    printf("Time take by BBHash for large file: %.2lf seconds.\n", difftime(e,b));
    sort(kmers.begin(), kmers.end());
    kmers.erase(unique(kmers.begin(), kmers.end()), kmers.end());
    printf("Size of the Unique Kmers: %lu\n",kmers.size());
}



