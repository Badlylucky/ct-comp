#include<iostream>
#include<vector>
#include<utility>
#include<set>
#include<algorithm>
#include<iterator>
#include<tuple>
using namespace std;
using ip=pair<int,int>;
using tp=tuple<ip,ip,ip,ip,ip,ip,ip,ip>;

vector<vector<int> > ans;
int t=2;
int k;
pair<int,int> v[20];
int popcount(unsigned int n){
    int ret=0;
    for(unsigned int i=0;i<32;i++){
        if(n&(1<<i)){
            ret++;
        }
    }
    return ret;
}
// https://qiita.com/drken/items/7c6ff2aa4d8fce1c9361#8-next_combination
int next_combination(int sub) {
    int x = sub & -sub, y = sub + x;
    return (((sub & ~y) / x) >> 1) | y;
}
tp generatetuple(){
    return tp(ip(-1,-1), ip(-1,-1), ip(-1,-1), ip(-1,-1), ip(-1,-1), ip(-1,-1), ip(-1,-1), ip(-1,-1));
}
ip getelement(const tp &t, const int index){
    switch(index){
        case 0:
        return get<0>(t);
        case 1:
        return get<1>(t);
        case 2:
        return get<2>(t);
        case 3:
        return get<3>(t);
        case 4:
        return get<4>(t);
        case 5:
        return get<5>(t);
        case 6:
        return get<6>(t);
        case 7:
        return get<7>(t);
        default:
        return ip(-1,-1);
    }
}
void outputline(vector<int> &line){
    for(int i=0;i<line.size();i++){
        cout<<line[i];
        if(i<line.size()-1){
            cout<<',';
        }else{
            cout<<endl;
        }
    }
    return;
}
//最初のt個分の組み合わせを決定し、テストケースを生成する
void initGenerate(int cnt, vector<int> val){
    if(cnt==0){
        vector<int> tmp(k,-1);
        for(int i=0;i<val.size();i++){
            tmp[v[i].second]=val[i];
        }
        ans.push_back(tmp);
        return;
    }else{
        for(int i=0;i<v[t-cnt].first;i++){
            vector<int> cpy;
            copy(val.begin(),val.end(),back_inserter(cpy));
            cpy.push_back(i);
            initGenerate(cnt-1,cpy);
        }
    }
}
// t-1個分の組み合わせを全列挙する。initGenerateとほぼ同じ
void enumerateCombination(int cnt, vector<int> kumi, vector<int> val){
    if(cnt == 0){
        //
    }else{
        //
    }
}
vector<set<tp> > enumerateInteractions(int ind) {
    vector<set<tp>> interactions(v[ind].first);
    vector<int> target;
    // 現在のところt=2固定なのでbit全探索の意味はない
    // indより前のものについて(t-1)個組の組合せを列挙する
    int i = (1<<(t-1)) - 1;
    for(;i<(1<<ind);i = next_combination(i)){
        // cerr<<i<<endl;
        target.push_back(i);
        for(int param=0;param<v[ind].first;param++){
            for(int bit=0;bit<ind;bit++){
            // ここが複数個必要
                if(target[i]&(1<<bit)){
                    for(int j=0;j<v[bit].first;j++){
                        // 要素の追加はvの並び順で行う
                        tp newt = generatetuple();
                        get<0>(newt) = ip(v[bit].second, j);
                        interactions[param].insert(newt);
                    }
                }
            }
        }
    }
    return interactions;
}
// Algorithm: IPO_Hを実行する
vector<set<tp> > IPOH(int ind){
    // interactionのうちindが絡むものを全て列挙する
    vector<set<tp> > interactions = enumerateInteractions(ind);
    cerr<<"kazoeage done"<<endl;
    // 最初のv[ind]個は順番に割り付けを行う
    for(int i=0;i<v[ind].first;i++){
        ans[i][v[ind].second]=i;
        // 満たされたinteractionの削除を行う
        for(int j=0;j<k;j++){
            tp newt = generatetuple();
            // ここは探索の必要がある
            get<0>(newt) = ip(j, ans[i][j]);
            if(ans[i][j]!=-1 && interactions[i].count(newt)){
                interactions[i].erase(newt);
            }
        }
    }
    cerr<<"first edit done"<<endl;
    // それ以降は貪欲に割り付ける
    for(int i=v[ind].first;i<ans.size();i++){
        cerr<<i<<endl;
        int maxparam=-1;
        int maxcnt=-1;
        // パラメータ全てについて関連するinteractionを列挙
        for(int j=0;j<v[ind].first;j++){
            int cnt=0;
            for(int l=0;l<k;l++){
                tp newt = generatetuple();
                // ここは探索の必要がある
                get<0>(newt) = ip(l, ans[i][l]);
                if(ans[i][l]!=-1 && interactions[j].count(newt)){
                    cnt++;
                }
            }
            // cerr<<cnt<<endl;
            if(cnt>maxcnt){
                maxcnt=cnt;
                maxparam=j;
            }
        }
        // 決まった値で更新する
        ans[i][v[ind].second]=maxparam;
        cerr<<"maxparam done"<<endl;
        // 満たされたinteractionの削除を行う
        for(int j=0;j<k;j++){
            tp newt = generatetuple();
            // ここは探索の必要がある
            get<0>(newt) = ip(j, ans[i][j]);
            if(ans[i][j]!=-1 && interactions[maxparam].count(newt)){
                interactions[maxparam].erase(newt);
            }
        }
    }
    cerr<<"second edit done"<<endl;
    return interactions;
}
// 指定したテストケースにインタラクションを入れることができるかチェックする
bool checkInsertInteraction(const tp &interaction, const vector<int> &test){
    bool ret = true;
    for(int i=0;i<8;i++){
        ip tmp = getelement(interaction, i);
        if(tmp.first == -1){
            break;
        }
        if(!(test[tmp.first] == tmp.second || test[tmp.first] == -1)){
            ret = false;
            break;
        }
    }
    return ret;
}
// テストケースにインタラクションを挿入する
void insertInteraction(const tp &interaction, vector<int> &test){
    for(int i=0;i<8;i++){
        ip tmp = getelement(interaction, i);
        if(tmp.first == -1){
            break;
        }
        test[tmp.first] = tmp.second;
    }
}
// Algorithm: IPO_Vを実行する
void IPOV(int ind, vector<set<tp> > &interactions){
    // すべてのinteractionについて調べる
    vector<vector<int> > addpair;
    for(int param=0;param<v[ind].first;param++){
        // 全列挙を行う
        addpair.clear();
        for(auto itr = interactions[param].begin();itr != interactions[param].end(); itr++){
            // 過去の追加列を全て調べて、あればそこに入れる
            bool newflag = true;
            for(int i=0;i<addpair.size();i++){
                if(checkInsertInteraction(*itr, addpair[i])) {
                    insertInteraction(*itr, addpair[i]);
                    newflag = false;
                    break;
                }
            }
            // 入れられる場所がなければ新しく作る
            if(newflag) {
                vector<int> t(k,-1);
                t[v[ind].second] = param;
                insertInteraction(*itr, t);
                addpair.push_back(t);
            }
        }
        // 全列挙が終わったら突っ込む
        for(int i=0;i<addpair.size();i++){
            ans.push_back(addpair[i]);
        }
    }
    return;
}
void solve(){
    // count all interactions
    sort(v,v+k);
    reverse(v,v+k);
    // 大きいものt個の組み合わせを全て割り当てる
    initGenerate(t, vector<int>());
    // for(int i=0;i<ans.size();i++){
    //     outputline(ans[i]);
    // }
    cerr<<"init generate done"<<endl;
    // 残りのk-t個について調べる
    for(int i=t;i<k;i++){
        vector<set<tp> > interactions = IPOH(i);
        IPOV(i, interactions);
    }
}
int main(){
    // input
    cin>>t>>k;
    for(int i=0;i<k;i++){
        int tmp;
        cin>>tmp;
        v[i]=make_pair(tmp,i);
    }
    solve();
    // output
    cout<<ans.size()<<endl;
    for(int i=0;i<ans.size();i++){
        outputline(ans[i]);
    }
    return 0;
}