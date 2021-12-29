#include<iostream>
#include<vector>
#include<utility>
#include<set>
#include<algorithm>
#include<iterator>
#include"inputUtility.hpp"
using namespace std;
using vi=vector<int>;

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
void initGenerate(){
    // 同値な言い換え
    int now = 1;
    for(int i = 0;i < t;++i){
        now *= v[i].first;
    }
    for(int choice = 0;choice < now;++choice){
        vector<int> cur = vi(k,-1);
        int tmpChoice = choice;
        for(int q = 0;q < t;++q){
            cur[v[q].second] = tmpChoice % v[q].first;
            tmpChoice /= v[q].first;
        }
        ans.push_back(cur);
    }
}
// t-1個分の組み合わせを全列挙する。
// cnt: 中間変数
// kumi: どの値を使うか
// val: 現在収容している組合せ
void enumerateCombination(const vector<int> &kumi, vector<vector<int>> &ret){
    int now = 1;
    for(int i=0;i<kumi.size();i++){
        now *= v[kumi[i]].first;
    }
    for(int choice = 0;choice < now;++choice){
        vector<int> cur = vi(k, -1);
        int tmpChoice = choice;
        for(int q=0;q<kumi.size();q++){
            cur[v[kumi[q]].second] = tmpChoice % v[kumi[q]].first;
            tmpChoice /= v[kumi[q]].first;
        }
        ret.push_back(cur);
    }
    return;
}
vector<set<vi> > enumerateInteractions(int ind) {
    vector<set<vi>> interactions(v[ind].first);
    vector<int> target;
    // indより前のものについて(t-1)個組の組合せを列挙する
    int i = (1<<(t-1)) - 1;
    for(;i<(1<<ind);i = next_combination(i)){
        target.clear();
        // 得られたbit列をtargetに分割する
        for(int bit=0;bit<ind;bit++){
            if(i&(1<<bit)){
                target.push_back(bit);
            }
        }
        // targetをもとにenumetrateCombinationを呼び出す
        vector<vector<int>> ret;
        enumerateCombination(target, ret);
        // retとtargetから挿入するinteractionを列挙する
        for(int i=0;i<ret.size();i++){
            // interactionの復元が終わったら挿入する
            for(int j=0;j<v[ind].first;j++){
                interactions[j].insert(ret[i]);
            }
        }
    }
    return interactions;
}
// Algorithm: IPO_Hを実行する
vector<set<vi> > IPOH(int ind){
    // interactionのうちindが絡むものを全て列挙する
    vector<set<vi> > interactions = enumerateInteractions(ind);
    cerr<<"kazoeage done"<<endl;
    // 最初のv[ind]個は順番に割り付けを行う
    for(int i=0;i<v[ind].first;i++){
        ans[i][v[ind].second]=i;
        // 満たされたinteractionの削除を行う
        int kumiawase = (1<<(t-1)) - 1;
        for(;kumiawase<(1<<ind);kumiawase = next_combination(kumiawase)){
            vi interaction = vi(k,-1);
            for(int bit=0;bit<ind;bit++){
                if((kumiawase&(1<<bit)) == 0){
                    continue;
                }
                interaction[v[bit].second] = ans[i][v[bit].second];
            }
            if(interactions[i].count(interaction)){
                interactions[i].erase(interaction);
            }
        }
    }
    cerr<<"first edit done"<<endl;
    // それ以降は貪欲に割り付ける
    for(int i=v[ind].first;i<ans.size();i++){
        int maxparam=-1;
        int maxcnt=-1;
        // パラメータ全てについて関連するinteractionを列挙
        // i番目のテストケースについて、パラメータjをv[ind].secondに割り当てる
        // それまでにあるのはjテストケース
        for(int j=0;j<v[ind].first;j++){
            int cnt=0;
            int kumiawase = (1<<(t-1)) - 1;
            for(;kumiawase<(1<<ind);kumiawase = next_combination(kumiawase)){
                vi interaction = vi(k,-1);
                for(int bit=0;bit<ind;bit++){
                    if((kumiawase&(1<<bit)) == 0){
                        continue;
                    }
                    interaction[v[bit].second] = ans[i][v[bit].second];
                }
                // outputline(interaction);
                // cerr<<interactions[j].count(interaction)<<endl;
                if(interactions[j].count(interaction)){
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
        int kumiawase = (1<<(t-1)) - 1;
        for(;kumiawase<(1<<ind);kumiawase = next_combination(kumiawase)){
            vi interaction = vi(k,-1);
            for(int bit=0;bit<ind;bit++){
                if((kumiawase&(1<<bit)) == 0){
                    continue;
                }
                interaction[v[bit].second] = ans[i][v[bit].second];
            }
            if(interactions[maxparam].count(interaction)){
                interactions[maxparam].erase(interaction);
            }
        }
    }
    cerr<<"second edit done"<<endl;
    return interactions;
}
// 指定したテストケースにインタラクションを入れることができるかチェックする
bool checkInsertInteraction(const vi &interaction, const vector<int> &test){
    bool ret = true;
    for(int i=0;i<k;i++){
        int tmp = interaction[i];
        if(tmp == -1){
            continue;
        }
        if(!(test[i] == tmp || test[i] == -1)){
            ret = false;
            break;
        }
    }
    return ret;
}
// テストケースにインタラクションを挿入する
void insertInteraction(const vi &interaction, vector<int> &test){
    for(int i=0;i<k;i++){
        int tmp = interaction[i];
        if(tmp == -1){
            continue;
        }
        test[i] = tmp;
    }
}
// Algorithm: IPO_Vを実行する
void IPOV(int ind, vector<set<vi> > &interactions){
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
    initGenerate();
    for(int i=0;i<ans.size();i++){
        outputline(ans[i]);
    }
    cerr<<"init generate done"<<endl;
    // 残りのk-t個について調べる
    for(int i=t;i<k;i++){
        vector<set<vi> > interactions = IPOH(i);
        IPOV(i, interactions);
    }
}
int main(){
    // input
    // for test
    cin>>t>>k;
    for(int i=0;i<k;i++){
        int tmp;
        cin>>tmp;
        v[i]=make_pair(tmp,i);
    }
    solve();
    // ouviut
    cout<<ans.size()<<endl;
    for(int i=0;i<ans.size();i++){
        outputline(ans[i]);
    }
    return 0;
}