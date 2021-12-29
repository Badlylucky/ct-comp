#include <string>
#include <vector>
#include <fstream>
#include<iostream>
using namespace std;
// テストケースの文字列と数字を対応させるためのクラス
class testSuite{
    public:
    string testName = "failed to open";
    vector<vector<string>> caseName;
    void outputInfo();
};

// 指定されたファイルをテストケースとして読み込む
testSuite readFile(string path);