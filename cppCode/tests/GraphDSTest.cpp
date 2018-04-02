//
// Created by Naga Satya Karthik on 11/10/17.
//
#include <gtest/gtest.h>

#include "../src/RabinKarpHash.h"
#include "../src/ConstructKMers.h"
#include "../src/BBHash.h"
#include "../src/GraphDS.h"

#define GENOMEFA "DeBruijnGraph/datasets/genome.fa"

namespace {
    class GraphDSTest : public testing::Test {
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
//        string data;
        int k;
        ConstructKMers* sut;
        RabinKarpHash* rkhash;
        BBHashExt* bbHashExt;
        GraphDS* gds;
        vector<long long> kmersHashValues;
        vector<string> kmers;
        GraphDSTest() {
//            data = "AGCGCTGAAAGTTTCATGAACAT";
            k = 5;
            sut = new ConstructKMers(k-1);
            rkhash = new RabinKarpHash(k-1);
//            kmers = sut->getKmersFromData(data);
            kmers = sut->getKMers(GENOMEFA);
            kmersHashValues = sut->getRKHashMaps(kmers);
            bbHashExt = new BBHashExt(kmersHashValues);
            gds = new GraphDS(bbHashExt->getSize(), 4, k, bbHashExt, rkhash);
        }

        virtual ~GraphDSTest() {
            delete gds;
            delete sut;
            delete rkhash;
            delete bbHashExt;
        }
    };
}

TEST_F(GraphDSTest, DISABLED_getNeighbours) {
    for(int i = 0 ; i < kmers.size(); i++){
        printf("%s, %lld, %lu\n", kmers[i].c_str(), kmersHashValues[i], bbHashExt->getMPHF(kmersHashValues[i]));
    }
//    gds->printInOut(); //Prints all 0's and 1'st
    for(int id = 1; id < kmers.size(); id++){
        gds->addStaticEdge(bbHashExt->getMPHF(kmersHashValues[id-1]), bbHashExt->getMPHF(kmersHashValues[id]), rkhash->getLastCharI(kmersHashValues[id]), rkhash->getFirstCharI(kmersHashValues[id-1]));
    }
    gds->printInOut();

    //Test neighbours of 19th vertex id
    vector<u_int64_t> neighboursExpected = {10, 15, 7, 3};
    vector<u_int64_t> neighboursAct = gds->getNeighbours(5);
    //prints both the lists
    printf("Expected: ");
    for(u_int64_t nact: neighboursExpected)printf("%lu, ", nact);
    printf("\nActual: ");
    for(u_int64_t nact: neighboursAct)printf("%lu, ", nact);
    printf("\n");

    // checks all neighbours found whether present in expected list
    for(u_int64_t nact: neighboursAct){
        if(find(neighboursExpected.begin(), neighboursExpected.end(), nact) == neighboursExpected.end()){
            printf("%lu is not found in expected list\n", nact);
            EXPECT_TRUE(false);
        }
    }
    // checks all neighbours expected whether present in neighbours found list
    for(u_int64_t nexpt: neighboursExpected){
        if(find(neighboursAct.begin(), neighboursAct.end(), nexpt) == neighboursAct.end()){
            printf("%lu is not found in Actual list\n", nexpt);
            EXPECT_TRUE(false);
        }
    }
}


TEST_F(GraphDSTest, DISABLED_buildForestTest){
//    for(int i = 0 ; i < kmers.size(); i++){
//        printf("%s, %lld, %lu\n", kmers[i].c_str(), kmersHashValues[i], bbHashExt->getMPHF(kmersHashValues[i]));
//    }
//    gds->printInOut(); //Prints all 0's and 1'st
    for(int id = 1; id < kmers.size(); id++){
        gds->addStaticEdge(bbHashExt->getMPHF(kmersHashValues[id-1]), bbHashExt->getMPHF(kmersHashValues[id]), rkhash->getLastCharI(kmersHashValues[id]), rkhash->getFirstCharI(kmersHashValues[id-1]));
    }
//    gds->printInOut();
//    gds->printParentPointers();
//    gds->printComponents();
    time_t b, e;
    time(&b);
    gds->buildForest();
    time(&e);
    printf("Time taken by the build forest: %.2lf seconds.\n", difftime(e,b));
//    gds->printParentPointers();
//    gds->printComponents();

}


TEST_F(GraphDSTest, BreakEvenAddDynamicEdgeTest){
//    for(int i = 0 ; i < kmers.size(); i++){
//        printf("%s, %lld, %lu\n", kmers[i].c_str(), kmersHashValues[i], bbHashExt->getMPHF(kmersHashValues[i]));
//    }
//    gds->printInOut(); //Prints all 0's and 1'st
//    for(int id = 1; id < kmers.size()/2; id++){
//    for(int id = 1; id < kmers.size(); id++){
//        gds->addStaticEdge(bbHashExt->getMPHF(kmersHashValues[id-1]), bbHashExt->getMPHF(kmersHashValues[id]), rkhash->getLastCharI(kmersHashValues[id]), rkhash->getFirstCharI(kmersHashValues[id-1]));
//    }
//    gds->printInOut();
    gds->buildForest();

//    gds->printParentPointers();
//    gds->printComponents();
    time_t b, e;
    printf("Build forest done for full dataset.");
    time(&b);
    for(int id = 0; id < kmers.size(); id++){
//        printf("Processing id: %d\n", id);
        gds->addDynamicEdge(bbHashExt->getMPHF(kmersHashValues[id-1]), bbHashExt->getMPHF(kmersHashValues[id]), rkhash->getLastCharI(kmersHashValues[id]), rkhash->getFirstCharI(kmersHashValues[id-1]));
    }
    time(&e);
    printf("Time taken by the BreakEven Edge forest: %.2lf seconds.\n", difftime(e,b));
//    gds->printParentPointers();
//    gds->printComponents();
}




TEST_F(GraphDSTest, DISABLED_updateParentPointersTest){
    for(int i = 0 ; i < kmers.size(); i++){
        printf("%s, %lld, %lu\n", kmers[i].c_str(), kmersHashValues[i], bbHashExt->getMPHF(kmersHashValues[i]));
    }
//    gds->printInOut(); //Prints all 0's and 1'st
    for(int id = 1; id < kmers.size(); id++){
        gds->addStaticEdge(bbHashExt->getMPHF(kmersHashValues[id-1]), bbHashExt->getMPHF(kmersHashValues[id]), rkhash->getLastCharI(kmersHashValues[id]), rkhash->getFirstCharI(kmersHashValues[id-1]));
    }
//    gds->printInOut();
    gds->buildForest();

    gds->printParentPointers();
    gds->printComponents();
    gds->updateParentPointers(15);
    gds->printParentPointers();
    gds->printComponents();
}

