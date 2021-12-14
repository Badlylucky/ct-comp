#include<iostream>
#include<vector>
#include<utility>
#include<set>
#include<algorithm>
#include<iterator>
using namespace std;

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
        // for(int i=0;i<tmp.size();i++){
        //     cerr<<tmp[i]<<",";
        // }
        // cerr<<endl;
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
// Algorithm: IPO_Hを実行する
vector<set<pair<int, int> > > IPOH(int ind){
    // interactionのうちindが絡むものを全て列挙する
    vector<set<pair<int, int> > > interactions(v[ind].first);
    vector<int> target;
    // 現在のところt=2固定なのでbit全探索の意味はない
    // indより前のものについて(t-1)個組の組合せを列挙する
    for(int i=0;i<(1<<ind);i++){
        if(popcount(i)==t-1){
            cerr<<i<<endl;
            target.push_back(i);
        }
    }
    // それらについて、全値の集合を調べる
    for(int param=0;param<v[ind].first;param++){
        // vector<pair<int,int>> oneinteraction;
        for(int i=0;i<target.size();i++){
            for(int bit=0;bit<ind;bit++){
                if(target[i]&(1<<bit)){
                    for(int j=0;j<v[bit].first;j++){
                        interactions[param].insert(make_pair(v[bit].second, j));
                    }
                }
            }
        }
        cerr<<interactions[param].size()<<endl;
    }
    cerr<<"kazoeage done"<<endl;
    // 最初のv[ind]個は順番に割り付けを行う
    for(int i=0;i<v[ind].first;i++){
        ans[i][v[ind].second]=i;
        // 満たされたinteractionの削除を行う
        for(int j=0;j<k;j++){
            if(ans[i][j]!=-1 && interactions[i].count(make_pair(j, ans[i][j]))){
                interactions[i].erase(make_pair(j, ans[i][j]));
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
                if(ans[i][l]!=-1 && interactions[j].count(make_pair(l, ans[i][l]))){
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
            if(ans[i][j]!=-1 && interactions[maxparam].count(make_pair(j, ans[i][j]))){
                interactions[maxparam].erase(make_pair(j, ans[i][j]));
            }
        }
    }
    cerr<<"second edit done"<<endl;
    return interactions;
}
// Algorithm: IPO_Vを実行する
void IPOV(int ind, vector<set<pair<int, int> > > &interactions){
    // すべてのinteractionについて調べる
    vector<vector<int> > addpair;
    for(int param=0;param<v[ind].first;param++){
        // 全列挙を行う
        addpair.clear();
        for(auto itr = interactions[param].begin();itr != interactions[param].end(); itr++){
            // 過去の追加列を全て調べて、あればそこに入れる
            bool newflag = true;
            for(int i=0;i<addpair.size();i++){
                if(addpair[i][itr->first] == -1) {
                    addpair[i][itr->first] = itr->second;
                    newflag = false;
                    break;
                }
            }
            // 入れられる場所がなければ新しく作る
            if(newflag) {
                vector<int> t(k,-1);
                t[v[ind].second] = param;
                t[itr->first] = itr->second;
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
        vector<set<pair<int, int> > > interactions = IPOH(i);
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