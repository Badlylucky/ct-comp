#include <string>
#include <vector>
#include <fstream>
#include<iostream>
using namespace std;
// テストケースの文字列と数字を対応させるためのクラス
class testSuite{
    public:
    string testName = "failed to open";
    vector<string> paramNames;
    vector<vector<string>> caseName;
    void outputInfo();
};

// 指定されたファイルをテストケースとして読み込む
testSuite readFile(string path);

// CSVの形で結果を出力する
void csvOutput(const string path, const testSuite &suite, const vector<vector<int>> &ans);
void csvOutput(const string path, const vector<vector<int>> &ans);