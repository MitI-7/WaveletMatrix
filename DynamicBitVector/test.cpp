#include <bits/stdc++.h>
#include "DynamicBitVector.hpp"

using namespace std;

unsigned long randxor(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}
double speed_constructor(uint64_t num) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor()  % 2;
        data.emplace_back(bit);
    }

    auto start = std::chrono::system_clock::now();
    DynamicBitVector dbv(data);
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_access(uint64_t num) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = randxor() % 2;
    }

    DynamicBitVector dbv(data);

    auto start = std::chrono::system_clock::now();
    int dummy = 0;
    for (int i = 0; i < num; ++i) {
        uint64_t pos = randxor() % (num);
        dbv.access(pos);
    }
    auto end = std::chrono::system_clock::now();
    if (dummy != 0) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_push_back(uint64_t num) {
    DynamicBitVector dbv;

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor() % 2;
        dbv.push_back(bit);
    }
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_insert(uint64_t num) {
    DynamicBitVector dbv;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t pos = randxor() % (i + 1);
        uint64_t bit = randxor() % 2;
        dbv.insert(pos, bit);
    }
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_erase(uint64_t num) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor() % 2;
        data.emplace_back(bit);
    }
    DynamicBitVector dbv(data);

    auto start = std::chrono::system_clock::now();
    for (int i = num - 1; i > 0; --i) {
        uint64_t pos = randxor() % i;
        dbv.erase(pos);
    }
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_update(uint64_t num) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor() % 2;
        data.emplace_back(bit);
    }
    DynamicBitVector dbv(data);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t pos = randxor() % num;
        uint64_t bit = randxor() % 2;
        dbv.update(pos, bit);
    }
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_rank(uint64_t num) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
    }

    DynamicBitVector dbv(data);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t pos = randxor() % num;
        uint64_t bit = randxor() % 2;
        dbv.rank(bit, pos);
    }
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_select(uint64_t num) {
    vector<uint64_t> data(num), count(2, 0);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
        count[data[i]]++;
    }

    DynamicBitVector dbv(data);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor() % 2;
        uint64_t pos = max((uint64_t)1, randxor() % count[bit]);
        dbv.select(bit, pos);
    }
    auto end = std::chrono::system_clock::now();
    if (dbv.access(0) == 3) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// dbvのbitの並びとansの値が一致しているか確認
bool check(DynamicBitVector dbv, const vector<uint64_t> &ans) {
    bool ok = true;
    for (int pos = 0; pos < ans.size(); ++pos) {
        if (ans[pos] != dbv.access((uint64_t)pos)) {
            cout << "ERROR at pos:" << pos << " expect:" << ans[pos] << " actual:" << dbv.access((uint64_t)pos) << endl;
            ok = false;
        }
    }

    if (ans.size() != dbv.size) {
        cout << "ERROR at size expect:" << ans.size() << " actual:" << dbv.size << endl;
        ok = false;
    }

    if (accumulate(ans.begin(), ans.end(), (uint64_t)0) != dbv.num_one) {
        cout << "ERROR at num one expect:" << accumulate(ans.begin(), ans.end(), (uint64_t)0) << " actual:" << dbv.num_one << endl;
        ok = false;
    }

    return ok;
}

bool test_constructor(uint64_t num, bool check_tree) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor() % 2;
        data.emplace_back(bit);
    }

    DynamicBitVector dbv(data);
    if (check_tree and not dbv.is_valid_tree(true)) {
        return false;
    }

    return check(dbv, data);
}

bool test_access(uint64_t num, bool check_tree) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = randxor() % 2;
    }
    DynamicBitVector dbv(data);
    return check(dbv, data);
}

bool test_append(uint64_t num, bool check_tree) {
    vector<uint64_t> data;
    DynamicBitVector dbv;
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor()  % 2;
        data.emplace_back(bit);
        dbv.push_back(bit);
        if (check_tree and not dbv.is_valid_tree(true)) {
            return false;
        }
    }

    return check(dbv, data);
}

bool test_insert(uint64_t num, bool check_tree) {
    vector<uint64_t> data;
    DynamicBitVector dbv;
    for (int i = 0; i < num; ++i) {
        uint64_t pos = randxor() % (i + 1);
        uint64_t bit = randxor() % 2;
        data.insert(data.begin() + pos, bit);
        dbv.insert(pos, bit);
        if (check_tree and not dbv.is_valid_tree(true)) {
            return false;
        }
    }

    return check(dbv, data);
}

bool test_erase(uint64_t num, bool check_tree) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        uint64_t bit = randxor() % 2;
        data[i] = bit;
    }

    DynamicBitVector dbv(data);

    bool ok = true;
    for (int i = 0; i < data.size(); ++i) {
        uint64_t pos = randxor() % (data.size());

        data.erase(data.begin() + pos);
        dbv.erase(pos);
        if (check_tree and not dbv.is_valid_tree(true)) {
            ok = false;
        }

        ok &= check(dbv, data);
        if (not ok) {
            break;
        }
    }

    return ok;
}

bool test_update(uint64_t num, bool check_tree) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = randxor() % 2;
    }
    DynamicBitVector dbv(data);

    bool ok = true;
    for (int i = 0; i < data.size(); ++i) {
        uint64_t pos = randxor() % (data.size());
        uint64_t bit = randxor() % 2;

        data[pos] = bit;
        dbv.update(pos, bit);
        if (check_tree and not dbv.is_valid_tree(true)) {
            ok = false;
        }

        ok &= check(dbv, data);
        if (not ok) {
            break;
        }
    }

    return ok;
}

bool test_rank(uint64_t num) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
    }

    DynamicBitVector dbv(data);

    for (uint64_t bit = 0; bit <= 1; ++bit) {
        for (uint64_t i = 0; i < data.size(); ++i) {
            int expected = 0;
            for (int j = 0; j < i; ++j) {
                expected += (data[j] == bit);
            }

            if (dbv.rank(bit, i) != expected) {
                return false;
            }
        }
    }

    return true;
}

bool test_select(uint64_t num) {
    vector<uint64_t> data(num), count(2, 0);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % 2;
        count[data[i]]++;
    }

    DynamicBitVector dbv(data);

    for (uint64_t bit = 0; bit <= 1; ++bit) {

        for (uint64_t rank = 1; rank < count.at(bit); ++rank) {

            uint64_t expected = 0;
            int n = 0;
            for (uint64_t i = 0; i < data.size(); ++i) {
                n += (data.at(i) == bit);
                if (n == rank) {
                    expected = i + 1;   // selectは見つけたindex+1
                    break;
                }
            }

            if (dbv.select(bit, rank) != expected) {
                return false;
            }
        }
    }

    return true;
}

void speed_test(uint64_t num) {
    cout << "constructor:" << speed_constructor(num) << "ms" << endl;
    cout << "access:" << speed_access(num) << "ms" << endl;
    cout << "push_back:" << speed_push_back(num) << "ms" << endl;
    cout << "insert:" << speed_insert(num) << "ms" << endl;
    cout << "erase:" << speed_erase(num) << "ms" << endl;
    cout << "update:" << speed_update(num) << "ms" << endl;
    cout << "rank:" << speed_rank(num) << "ms" << endl;
    cout << "select:" << speed_select(num) << "ms" << endl;
}

bool test(uint64_t num) {
    bool ok = true;

    cout << "Test constructor:" << ((ok &= test_constructor(num, true)) ? "OK" : "NG") << endl;
    cout << "Test access:" << ((ok &= test_access(num, true)) ? "OK" : "NG") << endl;
    cout << "Test append:" << ((ok &= test_append(num, true)) ? "OK" : "NG") << endl;
    cout << "Test insert:" << ((ok &= test_insert(num, true)) ? "OK" : "NG") << endl;
    cout << "Test erase:" << ((ok &= test_erase(num, true)) ? "OK" : "NG") << endl;
    cout << "Test update:" << ((ok &= test_update(num, true)) ? "OK" : "NG") << endl;
    cout << "Test rank:" << ((ok &= test_rank(num)) ? "OK" : "NG") << endl;
    cout << "Test select:" << ((ok &= test_select(num)) ? "OK" : "NG") << endl;

    return ok;
};

int main() {
    cout << "SPEED" << endl;
    speed_test(100000);
    cout << endl;

    cout << "TEST" << endl;
    for (int i = 0; i < 1000; ++i) {
        cout << "test:" << i << endl;
        uint64_t num = randxor() % 1000;
        if (not test(num)) {
            break;
        }
    }
    cout << endl;

    return 0;
}