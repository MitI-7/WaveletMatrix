#include<bits/stdc++.h>
#include "../SemiDynamicWaveletMatrix/SemiDynamicWaveletMatrix.hpp"

using namespace std;


unsigned long randxor(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

template<typename T> void print_vector(const vector<T> &v) {
    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
}

int naive_rank(int c, vector<uint64_t> &v, int pos, vector<bool> &active) {
    int ret = 0;
    for (int i = 0; i < min(pos, (int)v.size()); ++i) {
        if (not active[i]) {
            continue;
        }
        ret += (v[i] == c);
    }
    return ret;
}

int naive_select(int c, vector<uint64_t> &v, int rank, vector<bool> &active) {
    for (int i = 0; i < v.size(); ++i) {
        if (not active[i]) {
            continue;
        }
        if (v[i] == c) {
            rank--;
        }
        if (rank == 0) {
            return i + 1;
        }
    }

    return NOTFOUND;
}

uint64_t naive_quantile(vector<uint64_t> &v, int begin, int end, int r, vector<bool> &active) {
    vector<pair<int, int>> a;
    for (int i = begin; i < min(end, (int)v.size()); ++i) {
        if (active[i]) {
            a.emplace_back(make_pair(v[i], i));
        }
    }
    sort(a.begin(), a.end());

    if (r >= a.size()) {
        return NOTFOUND;
    }
    return a.at(r).second;
}

bool test_rank(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }
    vector<bool> state(num, false); // はじめは全部inactive

    SemiDynamicWaveletMatrix wm(data);

    for (int i = 0; i < data.size(); ++i) {
        assert(not wm.isActive(i));
    }

    set<uint64_t> alphabet(data.begin(), data.end());
    for (int i = 0; i < 1000; ++i) {
        for (auto c : alphabet) {
            for (uint64_t pos = 0; pos < data.size(); ++pos) {

                if (wm.rank(c, pos) != naive_rank(c, data, pos, state)) {
                    print_vector(data);
                    print_vector(state);
                    cout << "Error at rank(" << c << ", " << pos << ")" << endl;
                    int a = wm.rank(c, pos);
                    int b = naive_rank(c, data, pos, state);
                    cout << a << " " << b << endl;
                    return false;
                }
            }
        }

        if (i == 0) {
            wm.toActiveAll();
            fill(state.begin(), state.end(), true);
        }
        else {
            int idx = randxor() % num;
            if (randxor() % 2 == 0) {
                state[idx] = false;
                wm.toInactive(idx);
            } else {
                state[idx] = true;
                wm.toActive(idx);
            }
        }
    }

    return true;
}

bool test_select(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num), count(num_of_alphabet, 0);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
        count[data[i]]++;
    }
    vector<bool> state(num, false); // はじめは全部inactive

    SemiDynamicWaveletMatrix wm(data);
    set<uint64_t> alphabet(data.begin(), data.end());
    for (int i = 0; i < 1000; ++i) {
        for (auto c : alphabet) {

            for (uint64_t rank = 1; rank < count.at(c) + 10; ++rank) {

                if (wm.select(c, rank) != naive_select(c, data, rank, state)) {
                    print_vector(data);
                    print_vector(state);
                    cout << "select(" << c << ", " << rank << ")" << endl;
                    int a = wm.select(c, rank);
                    int b = naive_select(c, data, rank, state);
                    cout << a << " " << b << endl;
                    return false;
                }
            }
        }

        if (i == 0) {
            wm.toActiveAll();
            fill(state.begin(), state.end(), true);
        }
        else {
            int idx = randxor() % num;
            if (randxor() % 2 == 0) {
                state[idx] = false;
                wm.toInactive(idx);
            } else {
                state[idx] = true;
                wm.toActive(idx);
            }
        }
    }

    return true;
}

bool test_quantile(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num), count(num_of_alphabet, 0);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
        count[data[i]]++;
    }
    vector<bool> state(num, false); // はじめは全部inactive

    SemiDynamicWaveletMatrix wm(data);
    for (int i = 0; i < 1000; ++i) {
        for (int begin = 0; begin < num; ++begin) {
            for (int end = begin + 1; end <= num; ++end) {

                for (int r = 0; r < (end - begin) + 1; ++r) {

                    if (wm.quantileRange(begin, end, r) != naive_quantile(data, begin, end, r, state)) {
                        print_vector(data);
                        print_vector(state);
                        cout << "quantile(" << begin << ", " << end << ", " << r << ")" << endl;
                        int a = wm.quantileRange(begin, end, r);
                        int b = naive_quantile(data, begin, end, r, state);
                        cout << a << " " << b << endl;
                        return false;
                    }
                }
            }
        }

        if (i == 0) {
            wm.toActiveAll();
            fill(state.begin(), state.end(), true);
        }
        else {
            int idx = randxor() % num;
            if (randxor() % 2 == 0) {
                state[idx] = false;
                wm.toInactive(idx);
            } else {
                state[idx] = true;
                wm.toActive(idx);
            }
        }
    }

    return true;
}


bool test(uint64_t num, uint64_t num_of_alphabet) {
    bool ok = true;
    cout << "test rank:" << ((ok &= test_rank(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test select:" << ((ok &= test_select(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test quantile:" << ((ok &= test_quantile(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    return ok;
}

int main() {

    cout << "TEST" << endl;
    for (int i = 0; i < 100; ++i) {
        cout << "Test:" << i << endl;
        uint64_t num = randxor() % 130 + 1;
        uint64_t num_of_alphabet = randxor() % 50 + 1;
        if (not test(num, num_of_alphabet)) {
            cout << "ERROR" << endl;
            break;
        }
    }

    return 0;
}