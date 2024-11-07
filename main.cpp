#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;


float get_distance(vector<int> perm, vector<int> l, int i, int j){
    float res = (l.at(perm.at(i)) + l.at(perm.at(j))) / 2;
    for(int k = i; k < j; ++k)
        res += l.at(perm.at(k));
    return res;
}


float get_f(vector<int> perm, vector<vector<int>> c, vector<int> l){
    float res = 0;
    int n = c.at(0).size();
    for(int t = 0; t < n; ++t){
        for(int k = t; k < n; ++k){
            if(perm.at(t) < perm.at(k))
                res += c.at(perm.at(t)).at(perm.at(k)) * get_distance(perm, l, t, k);
            else
                res += c.at(perm.at(k)).at(perm.at(t)) * get_distance(perm, l, t, k);
        }
    }
    return res;
}


int main(){
    string tmp;
    int n;
    ifstream f("Y-10_t.txt");
    getline(f, tmp);
    n = stoi(tmp);
    vector<int> def(n, 0);
    vector<vector<int>> c(n, def);
    vector<int> l(n);
    getline(f, tmp);
    istringstream iss(tmp);
    for(int i = 0; i < n; ++i)
        iss >> l.at(i);
    for(int i = 0; i < n; ++i){
        getline(f, tmp);
        istringstream iss(tmp);
        for(int j = 0; j < n; ++j)
            iss >> c.at(i).at(j);
    }
    f.close();
    vector<int> perm(n);
    vector<int> arg(n, 0);
    for(int i = 0; i < n; ++i)
        perm.at(i) = i;
    float m = FLT_MAX, cur;
    do{
        cur = get_f(perm, c, l);
        if(cur < m){
            m = cur;
            cout << m << endl;
            arg = perm;
        }
    }while(next_permutation(perm.begin(), perm.end()));
    cout << m << endl;
    for(int i : arg)
        cout << i << " ";
    cout << endl;
    // cout << endl;
    // vector<int> vec({0, 4, 1, 9, 6, 3, 7, 2, 5, 8});
    // float r = get_f(vec, c, l);
    // cout << r << endl;
    return 0;
}
