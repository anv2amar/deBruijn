//
// Created by Naga Satya Karthik on 11/11/17.
//


#include <gtest/gtest.h>
#include "ctime"
#include "../src/ConstructKMers.h"
#include "../src/RabinKarpHash.h"
#include "../src/BooPHF.h"

//#include "../lib/BBHash-master/BooPHF.h"
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  long long, hasher_t  > boophf_t;

#define GENOMEFA "DeBruijnGraph/datasets/genome.fa"
#define GENOMESMALLL "DeBruijnGraph/datasets/small.fa"

namespace {
    class ConstructKMersTest : public testing::Test {
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
        ConstructKMersTest() {
            k = 8;
            sut = new ConstructKMers(k);
            rkhash = new RabinKarpHash(k);
        }

        virtual ~ConstructKMersTest() {
            delete sut;
            delete rkhash;
        }
    };
}

TEST_F(ConstructKMersTest, Kmers) {
    time_t begin, end;
    time(&begin);
    vector<string> kmers = sut->getKMers(GENOMEFA);
    time(&end);
    printf ("time taken for getKmers() %.2lf seconds.\n", difftime(end,begin));
    time(&begin);
    vector<long long> kmersHashValues = sut->getRKHashMaps(kmers);
    time(&end);
//    for(int i= 0 ; i < kmers.size(); i++){
//        printf("%s, %lld\n", kmers[i].c_str(), kmersHashValues[i]);
//    }
    printf ("time taken for getRKHashMaps() %.2lf seconds.\n", difftime(end,begin));

    time(&begin);
    map<string, long long> map;
    set<long long> hashSet;
    printf("KMERS Size: %d", (int)kmers.size() );
    for(int i = 0 ; i < kmersHashValues.size(); i++){
        if(map.find(kmers[i]) != map.end()){
            if(map[kmers[i]]!= kmersHashValues[i]){
                printf("Diff hash value found for the same key");
                ASSERT_FALSE(true);
            }
        }else{
            map.insert(make_pair(kmers[i], kmersHashValues[i]));
            if(hashSet.find(kmersHashValues[i]) != hashSet.end()){
                printf("Duplicate hash found. for : %lli, \n"
                               "There is one more key in the map with the same hash value.\n", kmersHashValues[i]);
                ASSERT_FALSE(true);
            }
            hashSet.insert(kmersHashValues[i]);
        }
    }
    printf("No duplicate hash values found");
    time(&end);
    printf ("time taken for TestCase() %.2lf seconds.\n", difftime(end,begin));
    ASSERT_EQ(kmersHashValues.size(), kmers.size());
}

TEST_F(ConstructKMersTest, GetUniqueKmersTest){
    vector<string> kmers = {"abc", "eac", "aec", "abc"};
    vector<string> uniqKmers = sut->getUniqueKMers(kmers);
    ASSERT_EQ(uniqKmers.size(), 3);
    ASSERT_EQ(uniqKmers[0], "abc");
    ASSERT_EQ(uniqKmers[1], "aec");
    ASSERT_EQ(uniqKmers[2], "eac");
}


