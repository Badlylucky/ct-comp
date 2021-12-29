#include "inputUtility.hpp"
using namespace std;

void testSuite::outputInfo(){
    cerr<<"testcaseName = "<<testName<<endl;
    cerr<<"testcaseSize = "<<caseName.size()<<endl;
    for(int i=0;i<caseName.size();i++){
        cerr<<i<<endl;
        for(int j=0;j<caseName[i].size();j++){
            cerr<<caseName[i][j]<<endl;
        }
    }
    return;
}

// https://qiita.com/iseki-masaya/items/70b4ee6e0877d12dafa8
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    string item;
    for (char ch: s) {
        if (ch == delim) {
            if (!item.empty())
                elems.push_back(item);
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty())
        elems.push_back(item);
    return elems;
}

// 先頭と末尾のスペースを削除する
string strip(string s){
    int ind;
    // 先頭
    ind = 0;
    while(s[ind] == ' ' || s[ind] == '\n' || s[ind] == '\r'){
        ind++;
    }
    s = s.substr(ind);
    // 最後
    ind = s.size() - 1;
    while(s[ind] == ' ' || s[ind] == '\n' || s[ind] == '\r'){
        ind--;
    }
    s = s.erase(ind+1);
    return s;
}
// ACTS formatのテストケースのケース名とテスト名を読み込む
testSuite readACTS(string path){
    ifstream ifs(path);
    string linestr;
    testSuite ret = testSuite();
    getline(ifs,linestr); // [system]
    getline(ifs,linestr); // -- specify system name
    getline(ifs,linestr); // Name: hoge
    ret.testName = strip(linestr.substr(6));
    
    getline(ifs,linestr);
    getline(ifs,linestr); // [Parameter]
    getline(ifs,linestr); // -- general syntax is parameter_name : value1, value2, ...

    vector<vector<string>> testcaseNames;
    // Par0 (enum) : PAR0_0, PAR0_1, PAR0_2,...
    while(getline(ifs,linestr)){
        if(linestr.size() < 2){
            break;
        }
        vector<string> splitparam = split(linestr, ':');
        // parameter name
        string paramName = strip(split(splitparam[0], '(')[0]);
        // parameter values
        string tmplinestr = splitparam[1];
        vector<string> testcase = split(tmplinestr, ','); // スペース付きのテストケース名に分割
        for(int i=0;i<testcase.size();i++){
            testcase[i] = strip(testcase[i]); // スペースを削除
        }
        ret.paramNames.push_back(paramName);
        testcaseNames.push_back(testcase);
    }
    ret.caseName = testcaseNames;
    return ret;
}
// CTWedge formatのテストケースのテストケース名とテスト名を読み込む
testSuite readCTWedge(string path){
    ifstream ifs(path);
    string linestr;
    testSuite ret = testSuite();
    getline(ifs,linestr); // Model MCA_0
    ret.testName = strip(split(linestr, ' ')[1]);
    getline(ifs,linestr); // Parameters:

    vector<vector<string>> testcaseNames;
    while(getline(ifs,linestr)){
        if(linestr.size() < 2){
            break;
        }
        vector<string> splitparam = split(linestr, ':');
        // parameter name
        string paramName = strip(splitparam[0]);
        // parameter values
        string tmplinestr = strip(splitparam[1]);
        vector<string> testcase;
        // Booleanかどうかを判定する
        if (tmplinestr[0] == '{'){
            // not Boolean
            // {と}を取り除く
            tmplinestr = tmplinestr.substr(1);
            tmplinestr = tmplinestr.erase(tmplinestr.size() - 1);
            testcase = split(tmplinestr, ',');
        }else{
            // Boolean
            testcase.emplace_back("true");
            testcase.emplace_back("false");
        }
        ret.paramNames.push_back(paramName);
        testcaseNames.push_back(testcase);
    }
    ret.caseName = testcaseNames;
    return ret;
}

testSuite readFile(string path){
    ifstream ifs(path);
    string linestr;
    testSuite ret;
    // ACTSかCTWedgeかをチェックする
    if (ifs.fail()){
        cerr<<"Failed to open file."<<endl;
        exit(1);
    }
    getline(ifs,linestr);
    ifs.close();
    if(linestr.find("[System]") != string::npos) {
        ret = readACTS(path);   
    }else if(linestr.find("Model") != string::npos) {
        ret = readCTWedge(path);
    }
    return ret;
}

void csvOutput(const string path, const testSuite &suite, const vector<vector<int>> &ans){
    ofstream ofs(path);
    // output param name
    for(int i=0;i<suite.paramNames.size();i++){
        ofs<<suite.paramNames[i];
        if(i == suite.paramNames.size() - 1){
            ofs<<endl;
        }else{
            ofs<<",";
        }
    }
    // output Testcases
    for(int i=0;i<ans.size();i++){
        for(int j=0;j<ans[i].size();j++){
            ofs<<suite.caseName[j][ans[i][j]];
            if(j < ans[i].size() - 1){
                ofs<<",";
            }
        }
        if(i < ans.size() - 1){
            ofs<<endl;
        }
    }
    ofs.close();
}
void csvOutput(const string path, const vector<vector<int>> &ans){
    ofstream ofs(path);
    for(int i=0;i<ans.size();i++){
        for(int j=0;j<ans[i].size();j++){
            ofs<<ans[i][j];
            if(j < ans[i].size() - 1){
                ofs<<",";
            }
        }
        if(i < ans.size() - 1){
            ofs<<endl;
        }
    }
    ofs.close();
}
// for test
// int main(){
//     string path = "../benchmarks/CTWedge/MCA_2.ctw";
//     testSuite tmp = readFile(path);
//     tmp.outputInfo();
//     return 0;
// }