#include <bits/stdc++.h>
#include "SemiDynamicBitVector.hpp"

using namespace std;

unsigned long randxor(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

uint64_t naive_rank(int b, vector<uint64_t> &v, int pos, vector<bool> &state) {
    int ret = 0;
    for (int i = 0; i < pos; ++i) {
        if (not state[i]) {
            continue;
        }
        ret += (v[i] == b);
    }
    return ret;
}

uint64_t naive_select(int b, vector<uint64_t> &v, int rank, vector<bool> &state) {
    for (int i = 0; i < v.size(); ++i) {
        if (not state[i]) {
            continue;
        }
        if (v[i] == b) {
            rank--;
        }
        if (rank == 0) {
            return i + 1;
        }
    }

    return NOTFOUND;
}

// ランダムテスト
bool test1() {

    for (int _ = 0; _ < 100000; ++_) {

        uint64_t N = randxor() % 1000;
        SemiDynamicBitVector sucBV(N);

        vector<uint64_t> data(N, 0);
        vector<bool> state(N, false);
        for (uint64_t i = 0; i < N; ++i) {
            if (randxor() % 2) {
                sucBV.setBit(1, i);
                data[i] = 1;
            }
            if (randxor() % 2) {
                state[i] = true;
            }
        }

        // set status
        for (int i = 0; i < state.size(); ++i) {
            if (state[i]) {
                sucBV.to_active(i);
            }
            else {
                sucBV.to_inactive(i);
            }
        }

        sucBV.build();
        for (uint64_t pos = 0; pos < N; ++pos) {
            if (sucBV.rank(1, pos) != naive_rank(1, data, pos, state)) {
                cout << "NG rank(1, " << pos << ")" << endl;
                cout << "expected: " << naive_rank(1, data, pos, state) << " actual: " << sucBV.rank(1, pos) << endl;
                return false;
            }
            if (sucBV.rank(0, pos) != naive_rank(0, data, pos, state)) {
                cout << "NG rank(0, " << pos << ")" << endl;
                cout << "expected: " << naive_rank(0, data, pos, state) << " actual: " << sucBV.rank(0, pos) << endl;
                return false;
            }
            if (sucBV.select(1, pos + 1) != naive_select(1, data, pos + 1, state)) {
                cout << "NG select(1, " << pos + 1 << ")" << endl;
                cout << "expected: " << naive_select(1, data, pos + 1, state) << " actual: " << sucBV.select(1, pos + 1) << endl;
                return false;
            }
            if (sucBV.select(0, pos + 1) != naive_select(0, data, pos + 1, state)) {
                cout << "NG select(0, " << pos + 1 << ")" << endl;
                cout << "expected: " << naive_select(0, data, pos + 1, state) << " actual: " << sucBV.select(0, pos + 1) << endl;
                return false;
            }
        }
    }

    return true;
}

int main(){
    cout << "Test1 " << (test1() ? "OK" : "NG") << endl;

    return 0;
}