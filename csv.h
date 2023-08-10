#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

// 2023-05-25
// Written by Sora Sugiyama

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

vector<vector<string> >csvReader(string path,char deli,bool Header){
    ifstream file(path);
    vector<vector<string> >ret;
    if(file.fail()){
        cout<<"file does not exist.\n";
        return ret;
    }
    while(file.good()){
        ret.push_back(vector<string>());
        vector<string>&vs=ret.back();
        string tmp="";
        while(file.good()){
            char c=file.get();
            if(c=='\n')break;
            if(c==','){
                if(tmp=="")tmp="null";
                vs.push_back(tmp);
                tmp="";
            }else tmp.push_back(c);
        }
        if(!Header){
            Header=true;
            ret.pop_back();
        }
    }
    file.close();
    return ret;
}

#endif // CSV_H_INCLUDED
