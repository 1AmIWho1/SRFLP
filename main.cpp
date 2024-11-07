#include <iostream>
#include <vector>
#include <fstream>
using namespace std;


float get_distance(vector<int> l, int i, int j){
    float res = (l.at(i) + l.at(j)) / 2;
    for(int k = i; k < j + 1; ++k)
        res += l.at(k);
    return res;
}


vector<vector<float>> get_distances(vector<int> l){
    int n = l.size();
    vector<vector<float>> d;
    for(int i = 1; i < n - 1; ++i){
        vector<float> tmp;
        for(int j = i + 1; j < n; ++j)
            tmp.push_back(get_distance(l, i, j));
        d.push_back(tmp);
    }
    return d;
}


int main(){
    vector<vector<int>> c;
    vector<int> l;
    string tmp;
    int n;
    ifstream f("Y-10_t.txt");
    getline(f, tmp, ' ');
    n = stoi(tmp);
    for(int i = 0; i < n; ++i){
        getline(f, tmp, ' ');
        l.push_back(stoi(tmp));
    }
    for(int i = 0; i < n; ++i){
        vector<int> line;
        for(int j = 0; j < n; ++j){
            getline(f, tmp, ' ');
            line.push_back(stoi(tmp));
        }
        c.push_back(line);
    }
    f.close();
    vector<vector<float>> d = get_distances(l);
    for(auto i : d){
        for(float j : i)
            cout << j << " ";
        cout << endl;
    }
    return 0;
}
