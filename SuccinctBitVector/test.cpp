#include <bits/stdc++.h>
#include "SuccinctBitVector.hpp"

using namespace std;

unsigned long randxor(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

double speed_access(uint64_t num) {
    vector<uint64_t> data(num);
    SuccinctBitVector bv(num);
    for (uint64_t i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
        bv.setBit(data[i], i);
    }

    bv.build();

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < data.size(); ++i) {
        if (bv.access(i) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_rank(uint64_t num) {
    vector<bool> data(num);
    SuccinctBitVector bv(num);
    for (uint64_t i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
        bv.setBit(data[i], i);
    }

    bv.build();

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (uint64_t rank = 1; rank < data.size(); ++rank) {
        uint64_t val = data[randxor() % num];
        if (bv.rank(val, rank) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_select(uint64_t num) {
    vector<uint64_t > data(num), count(2, 0);
    SuccinctBitVector bv(num);
    for (uint64_t i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
        count[data[i]]++;
        bv.setBit(data[i], i);
    }

    bv.build();

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t val = data[randxor() % num];
        uint64_t rank  = randxor() % count.at(val) + 1;
        if (bv.select(val, rank) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

bool test1() {
    vector<uint64_t> data = {1, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    vector<uint64_t> rank1 = {0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3};
    vector<uint64_t> rank0 = {0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7};
    vector<uint64_t> select1 = {0, 1, 4, 8};                // 0はダミー
    vector<uint64_t> select0 = {0, 2, 3, 5, 6, 7, 9, 10};   // 0はダミー

    SuccinctBitVector sb(data.size());

    for (unsigned int i = 0; i < data.size(); ++i) {
        sb.setBit(data[i], i);
    }

    sb.build();

    // rank
    for (uint64_t i = 0; i < rank1.size(); ++i) {
        assert(sb.rank(1, i) == rank1[i]);
    }
    for (uint64_t i = 0; i < rank0.size(); ++i) {
        assert(sb.rank(0, i) == rank0[i]);
    }

    // select
    for (uint64_t i = 1; i < select1.size(); ++i) {
        assert(sb.select(1, i) == select1[i]);
    }
    for (uint64_t i = 1; i < select0.size(); ++i) {
        assert(sb.select(0, i) == select0[i]);
    }

    return true;
}

// すべて1
bool test2() {

    uint64_t N = 10000000;
    SuccinctBitVector sb(N);

    vector<uint64_t> data(N, 1);
    vector<uint64_t> rank1(N + 1);
    iota(rank1.begin(), rank1.end(), 0);
    vector<uint64_t> rank0(N + 1, 0);
    vector<uint64_t> select1(N);
    iota(select1.begin(), select1.end(), 0);
    for (uint64_t i = 0; i < data.size(); ++i) {
        sb.setBit(data[i], i);
        assert(sb.access(i) == data[i]);
    }

    sb.build();
    assert(sb.getNumOne() == N);

    for (uint64_t i = 0; i < data.size(); ++i) {
        assert(sb.rank(1, i) == rank1[i]);
        assert(sb.rank(0, i) == rank0[i]);
    }
    for (uint64_t rank = 1; rank < select1.size(); ++rank) {
        assert(sb.select(1, rank) == select1[rank]);
        assert(sb.select(0, rank) == NOTFOUND);
    }

    return true;
}

// すべて0
bool test3() {

    uint64_t N = 10000000;
    SuccinctBitVector sb(N);

    vector<uint64_t> data(N, 0);
    vector<uint64_t> rank1(N + 1, 0);
    vector<uint64_t> rank0(N + 1);
    iota(rank0.begin(), rank0.end(), 0);
    vector<uint64_t> select0(N);
    iota(select0.begin(), select0.end(), 0);
    for (uint64_t i = 0; i < data.size(); ++i) {
        sb.setBit(data[i], i);
        assert(sb.access(i) == data[i]);
    }

    sb.build();
    assert(sb.getNumOne() == 0);

    for (uint64_t i = 0; i < data.size(); ++i) {
        assert(sb.rank(1, i) == rank1[i]);
        assert(sb.rank(0, i) == rank0[i]);
    }
    for (unsigned int rank = 1; rank < select0.size(); ++rank) {
        assert(sb.select(1, rank) == NOTFOUND);
        assert(sb.select(0, rank) == select0[rank]);
    }

    return true;
}

// ランダムテスト
bool test4() {

    for (int _ = 0; _ < 10; ++_) {
        uint64_t N = randxor() % 10000000;
        SuccinctBitVector sucBV(N);

        vector<bool> naive(N, false);
        int numOfOne = 0;
        for (uint64_t i = 0; i < N; ++i) {
            if (randxor() % 2) {
                sucBV.setBit(1, i);
                naive[i] = true;
                if (sucBV.access(i) != 1) {
                    cout << "NG access(" << i << ")" << endl;
                    cout << "expected: " << i << " actual: " << sucBV.access(i) << endl;
                    return false;
                }
                numOfOne++;
            }
        }

        sucBV.build();
        if (sucBV.getNumOne() != numOfOne) {
            cout << "NG getNumOne()" << endl;
            cout << "expected: " << numOfOne << " actual: " << sucBV.getNumOne() << endl;
            return false;
        }

        uint64_t rank1 = 0, rank0 = 0;
        for (uint64_t i = 0; i < N; ++i) {
            if (sucBV.rank(1, i) != rank1) {
                cout << "NG rank(1, " << i << ")" << endl;
                cout << "expected: " << rank1 << " actual: " << sucBV.rank(1, i) << endl;
                return false;
            }
            if (sucBV.rank(0, i) != rank0) {
                cout << "NG rank(0, " << i << ")" << endl;
                cout << "expected: " << rank1 << " actual: " << sucBV.rank(0, i) << endl;
                return false;
            }

            if (naive[i]) {
                rank1++;
                if (sucBV.select(1, rank1) != i + 1) {
                    cout << "NG select(1, " << rank1 << ")" << endl;
                    cout << "expected: " << i + 1 << " actual: " << sucBV.select(1, rank1) << endl;
                    return false;
                }
            } else {
                rank0++;
                if (sucBV.select(0, rank0) != i + 1) {
                    cout << "NG select(0, " << rank0 << ")" << endl;
                    cout << "expected: " << i + 1 << " actual: " << sucBV.select(0, rank0) << endl;
                    return false;
                }
            }
        }
    }

    return true;
}

void speed_test(uint64_t num) {
    cout << "access:" << speed_access(num) << "ms" << endl;
    cout << "rank:" << speed_rank(num) << "ms" << endl;
    cout << "select:" << speed_select(num) << "ms" << endl;
}


int main(){
    speed_test(1000000);

    cout << "Test1 " << (test1() ? "OK" : "NG") << endl;
    cout << "Test2 " << (test2() ? "OK" : "NG") << endl;
    cout << "Test3 " << (test3() ? "OK" : "NG") << endl;
    cout << "Test4 " << (test4() ? "OK" : "NG") << endl;

    return 0;
}