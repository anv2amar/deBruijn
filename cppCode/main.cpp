#include "iostream"
//#include "src/PreProcess.h"
#include "boost/program_options.hpp"
#include <gtest/gtest.h>
#include "string"
#include "src/PreProcess.h"
#include "src/ConstructKMers.h"
#include "src/GraphDS.h"

#include <iterator>
#define GENOMEFA "datasets/genome.fa"

void getRowCol(int id, int *row, int *col, int m, int n);

using namespace boost::program_options;
using namespace std;
namespace po = boost::program_options;


int main(int argc, char *argv[]) {
    try {

        int limit, k;
        bool appendMode;
        double staticFraction;
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("choice", po::value<int>(), "select the methods to run: \n"
                        "0: Run the test cases\n"
                        "1: preprocess the input file to the output file. Please give the container paths here. (if using docker)."
                        "2: Add dynamic edges from the file"
                )
                ("input-file", po::value<string>(), "input file, should be a fastq (used for choice 1 & 2)")
                ("output-file", po::value<string>(), "output file, can be any file ")
                ("limit", po::value<int>(&limit)->default_value(1000) , "limit to number of sequence reads while preprocessing (Choice 1). Defaults to 1000 sequence reqds from fastq files")
                ("k", po::value<int>(&k)->default_value(15), "value of K in the k-mer")
                ("append-mode", po::value<bool>(&appendMode)->default_value(false), "append-mode add dynamic edges to the existing graph\n "
                                "if appendMode is false then all operations are individual")
                        ("static-fraction", po::value<double>(&staticFraction)->default_value(0.5), "Fraction of static edges that needs to be added before adding dynamic edges")
                ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, (const char *const *) argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("choice")) {
            cout << "choice is selected to "
                 << vm["choice"].as<int>() << ".\n";
            int c = vm["choice"].as<int>();
            switch (c) {
                case 0:
                    testing::InitGoogleTest(&argc, argv);
                    return RUN_ALL_TESTS();
                case 1:
                    if (!vm.count("input-file") || !vm.count("output-file")) {
                        printf("input-file and output-file are mandatory params for preprocess\n");
                        break;
                    } else {
//                        string result = "/Users/nagasaty/0classes/bdb/DeBruijnGraph/datasets/outputs/reduced.fq";
                        PreProcess *obj = new PreProcess(vm["input-file"].as<string>(), vm["output-file"].as<string>());
                        obj->truncate(limit);
                        printf("Preprocessed the file from %s to  %s...\n", vm["input-file"].as<string>().c_str(),
                               vm["output-file"].as<string>().c_str());
                    }
                case 2:
                    if (!vm.count("input-file") || !vm.count("k") || !vm.count("static-fraction")) {
                        printf("input-file and output-file are mandatory params for preprocess\n");
                        break;
                    } else {
                        ConstructKMers *sut;
                        RabinKarpHash *rkhash;
                        BBHashExt *bbHashExt;
                        GraphDS *gds;
                        vector<vector<long long>> kmersHashValues;
                        vector<vector<string>> kmers;
                        time_t b, e;

                        sut = new ConstructKMers(k - 1);
                        printf("Currently processing File: %s\n", vm["input-file"].as<string>().c_str());
                        rkhash = new RabinKarpHash(k - 1);
                        time(&b);
                        kmers = sut->getKMersFastQ(vm["input-file"].as<string>());
                        time(&e);
                        printf("Value of K: %d\n # of K-1mers inserting %lu\n", k, kmers.size());
                        printf("Time taken by the Constructing Kmer is: %.2lf seconds.\n", difftime(e, b));
                        time(&b);
                        kmersHashValues = sut->getRKHashMaps(kmers);
                        time(&e);
                        printf("Time taken by the Constructing RabinKarp Hash values is: %.2lf seconds.\n",
                               difftime(e, b));

                        time(&b);
                        bbHashExt = new BBHashExt(kmersHashValues);
                        time(&e);
                        printf("Time taken by the Constructing Minimal Perfect Hash values is: %.2lf seconds.\n",
                               difftime(e, b));

                        gds = new GraphDS(bbHashExt->getSize(), 4, k, bbHashExt, rkhash);
//                        printf("Preprocessed the file from %s to  %s...\n", vm["input-file"].as<string>().c_str(), vm["output-file"].as<string>().c_str());
                        int totalSize = 0 , dedges = 0, row, col, staticCount = 0, dynamicCount = 0 , buildForestC;
                        for(vector<string> kmer: kmers) totalSize+= kmer.size()-1;
                        staticCount = (int)floor(staticFraction*totalSize);
                        printf("TotalSize: %d is divided int static: %d & dynamic %d", totalSize, staticCount, totalSize-staticCount);
                        for(vector<long long> kmersHashValue: kmersHashValues){
                            for(int i = 1; i < kmersHashValue.size(); i++){
                                if(staticCount-- > 0) {
                                    gds->addStaticEdge(bbHashExt->getMPHF(kmersHashValue[i-1]),
                                    bbHashExt->getMPHF(kmersHashValue[i]),
                                    rkhash->getLastChar(kmersHashValue[i]),
                                    rkhash->getFirstChar(kmersHashValue[i-1]));
                                    if(staticCount == 0){
                                        time(&b);
                                        gds->buildForest();
                                        time(&e);
                                        printf("Time taken by the Build Forest is: %.2lf seconds.\n", difftime(e, b));
                                        staticCount--;
                                        time(&b);
                                    }
                                }else {
//                                    printf("static Edge , dc : %d\n", staticCount);

                                    gds->addDynamicEdge(bbHashExt->getMPHF(kmersHashValue[i-1]),
                                                       bbHashExt->getMPHF(kmersHashValue[i]),
                                                       rkhash->getLastChar(kmersHashValue[i]),
                                                       rkhash->getFirstChar(kmersHashValue[i-1]));
                                    dedges++;
                                }
                            }
                        }
                        time(&e);
                        printf("Time taken by the adding Dynamic Edges (No of Edges: %2d)forest: %.2lf seconds.\n", dedges, difftime(e, b));
//                        if (staticFraction > 0) {
//                            for (int id = 0; id < (int) floor(staticFraction * totalSize); id++) {
//                                if(id % n == 0) continue;
//                                getRowCol(id, &row, &col, m, n);
//
//                                gds->addStaticEdge(bbHashExt->getMPHF(kmersHashValues[row][col - 1]),
//                                                   bbHashExt->getMPHF(kmersHashValues[row][col]),
//                                                   rkhash->getLastCharI(kmersHashValues[row][col]),
//                                                   rkhash->getFirstCharI(kmersHashValues[row][col - 1]));
//                            }
//                            time(&b);
//                            gds->buildForest();
//                            time(&e);
//                            printf("Time taken by the Build Forest is: %.2lf seconds.\n", difftime(e, b));
//                        }
//                        time(&b); int dedges = 0 ;
//                        printf("Dynamic Edges: %lu (adding into graph)\n",kmers.size()-((int)floor(staticFraction * kmers.size())));
//                        for (int id = (int) floor(staticFraction * totalSize); id < kmers.size(); id++) {
//                            if(id % n == 0) continue;
//                            getRowCol(id, &row, &col, m, n);
//                            gds->addDynamicEdge(bbHashExt->getMPHF(kmersHashValues[row][col - 1]),
//                                               bbHashExt->getMPHF(kmersHashValues[row][col]),
//                                               rkhash->getLastCharI(kmersHashValues[row][col]),
//                                               rkhash->getFirstCharI(kmersHashValues[row][col - 1]));
//                            dedges++;
//                        }
//                        time(&e);
//                        printf("Time taken by the adding Dynamic Edges (No of Edges: %2d)forest: %.2lf seconds.\n", dedges, difftime(e, b));
                        gds->invariantCheck();
                        gds->printComponentsMetrics();
                    }
            }
        } else{
            cout << "Invalid Choice selected. Please select the choice from 0 - 2, \n" << desc << endl;
//            testing::InitGoogleTest(&argc, argv);
//            return RUN_ALL_TESTS();
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}

void getRowCol(int id, int *row, int *col, int m, int n) {
    *row = id/m;
    *col = id%n;
    return;
}

