//
// Created by Naga Satya Karthik on 12/6/17.
//

#ifndef CPPCODE_PREPROCESS_H
#define CPPCODE_PREPROCESS_H

#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include "vector"
using namespace std;
typedef long long ll;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<int> vi;

class PreProcess {
    string inputFilename, outputFilename;
public:
    PreProcess(string inputFilename, string outputFilename) : inputFilename(inputFilename), outputFilename(outputFilename) {}
    void truncate(int limit){
        string line;
        vector<string> fileLines;
        ifstream myfile (inputFilename);
        ofstream newfile (outputFilename);
        if (myfile.is_open() && newfile.is_open())
        {
            int i = 0;
            while ( getline (myfile,line) )
            {
                if(i%400 == 0){
                    writeToFile(fileLines, newfile);
                    fileLines.clear();
                }
                if((i++)%4 == 1) {
                    if(limit-- <= 0){
                        writeToFile(fileLines, newfile);
                        break;
                    }
                    fileLines.push_back(line);
                }
            }
            myfile.close();
            newfile.close();
        }
        else cout << "Unable to open file";
    }

    void writeToFile(vector<string> &fileLines, ofstream &newfile) {
        string s="";
        if(fileLines.size() == 0) return ;
        for(int ii = 0 ; ii < fileLines.size(); ii++){
            s += fileLines[ii]+((ii==(fileLines.size()-1))?"":"\n");
        }
        cout << "." ;
        newfile<< s << endl;
    }

    void printFile(){
        string line;
        ifstream myfile (inputFilename);
        if (myfile.is_open())
        {
            int i = 0;
            while ( getline (myfile,line) )
            {
                cout << line << '\n';
            }
            myfile.close();
        }
        else cout << "Unable to open file";
    }
};

#endif //CPPCODE_PREPROCESS_H
