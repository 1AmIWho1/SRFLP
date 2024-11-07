#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <thread>
using namespace std;


vector<double> best_solutuions;
vector<vector<int>> best_perm;


double get_distance(vector<int> perm, vector<int> l, int i, int j){
    double res = (l.at(perm.at(i)) + l.at(perm.at(j))) / 2;
    for(int k = i; k < j; ++k)
        res += l.at(perm.at(k));
    return res;
}


double get_f(vector<int> perm, vector<vector<int>> c, vector<int> l){
    double res = 0;
    int n = c.at(0).size();
    for(int t = 0; t < n; ++t)
        for(int k = t; k < n; ++k)
            if(perm.at(t) < perm.at(k))
                res += c.at(perm.at(t)).at(perm.at(k)) * get_distance(perm, l, t, k);
            else
                res += c.at(perm.at(k)).at(perm.at(t)) * get_distance(perm, l, t, k);
    return res;
}


void process(int thread_id, vector<int> perm, vector<vector<int>> c, vector<int> l, int chunk_size){
    double m = numeric_limits<double>::max();
    vector<int> local_best_perm;
    for (int i = 0; i < chunk_size; i++) {
        double f = get_f(perm, c, l);
        if (f < m) {
            m = f;
            local_best_perm = perm;
        }
        next_permutation(perm.begin(), perm.end());
    }
    best_perm.at(thread_id) = local_best_perm;
    best_solutuions.at(thread_id) = m;
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

    long total_permutations = 1;
    for(int i = 2; i < n; ++i)
        total_permutations *= i;
    vector<int> perm(n);
    for(int i = 0; i < n; ++i)
        perm.at(i) = i;
    vector<int> arg(n, 0);
    int worker_count = thread::hardware_concurrency();
    int chunk_size = total_permutations / worker_count;
    cout << "Worker count: " << worker_count << endl;
    cout << "Chunk size: " << chunk_size << endl;
    auto start = chrono::high_resolution_clock::now();
    vector<thread> workers(worker_count);
    vector<int> _def(n);
    vector<vector<int>> __def(worker_count, _def);
    best_perm = __def;
    vector<double> ___def(worker_count);
    best_solutuions = ___def;
    workers.at(0) = thread(process, 0, perm, c, l, chunk_size);
    for(int i = 1; i < worker_count; ++i){
        for(int j = 0; j < chunk_size; ++j)
            next_permutation(perm.begin(), perm.end());
        workers.at(i) = thread(process, i, perm, c, l, chunk_size);
    }
    for(thread &worker : workers)
        worker.join();
    
    int best_id = 0;
    for(int i = 1; i < worker_count; ++i){
        if(best_solutuions.at(i) < best_solutuions.at(best_id))
            best_id = i;
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Total time: " << duration.count() << " us" << endl;
    cout << "Best solution: " << best_solutuions.at(best_id) << endl;
    cout << "Best permutation: ";
    for(int i : best_perm.at(best_id))
        cout << i << " ";
    cout << endl;
    return 0;
}
