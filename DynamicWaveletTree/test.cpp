#include<bits/stdc++.h>
#include "DynamicWaveletTree.hpp"

using namespace std;

unsigned long randxor(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

double speed_access(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < data.size(); ++i) {
        if (dwt.access(i) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_rank(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (uint64_t rank = 1; rank < data.size(); ++rank) {
        uint64_t val = data[randxor() % num];
        if (dwt.rank(val, rank) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_select(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num), count(num_of_alphabet, 0);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
        count[data[i]]++;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t val = data[randxor() % num];
        uint64_t rank  = randxor() % count.at(val) + 1;
        if (dwt.select(val, rank) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_erase(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        data.emplace_back((uint64_t)randxor() % num_of_alphabet);
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num - 1; ++i) {
        uint64_t pos = i == 0 ? 0 : randxor() % dwt.size;
        dwt.erase(pos);
    }
    auto end = std::chrono::system_clock::now();
    if (dwt.access(0) == 1) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_insert(uint64_t num, uint64_t num_of_alphabet) {

    DynamicWaveletTree dwt(num_of_alphabet);
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t pos = i == 0 ? 0 : randxor() % i;
        uint64_t c = randxor() % num_of_alphabet;
        dwt.insert(pos, c);
    }
    auto end = std::chrono::system_clock::now();
    if (dwt.access(0) == -1) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_update(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        data.emplace_back((uint64_t)randxor() % num_of_alphabet);
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num - 1; ++i) {
        uint64_t pos = randxor() % data.size();
        uint64_t c = randxor() % num_of_alphabet;
        dwt.update(pos, c);
    }
    auto end = std::chrono::system_clock::now();
    if (dwt.access(0) == -1) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_quantile(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        if (dwt.quantileRange(0, num, i) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

bool test_access(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] != dwt.access(i)) {
            return false;
        }
    }
    return true;
}

bool test_rank(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    set<uint64_t> s(data.begin(), data.end());
    for (auto val : s) {
        for (uint64_t i = 0; i < data.size(); ++i) {

            int expected = 0;
            for (int j = 0; j < i; ++j) {
                expected += (data[j] == val);
            }

            if (dwt.rank(val, i) != expected) {
                return false;
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

    DynamicWaveletTree dwt(num_of_alphabet, data);

    set<uint64_t> s(data.begin(), data.end());
    for (auto val : s) {

        for (uint64_t rank = 1; rank < count.at(val); ++rank) {

            uint64_t expected = 0;
            int n = 0;
            for (uint64_t i = 0; i < data.size(); ++i) {
                n += (data.at(i) == val);
                if (n == rank) {
                    expected = i + 1;   // selectは見つけたindex+1
                    break;
                }
            }

            if (dwt.select(val, rank) != expected) {
                return false;
            }
        }
    }

    return true;
}

// 2つの動的ビットベクトルが同じかチェック
bool same(DynamicWaveletTree expected, DynamicWaveletTree actual) {

    if (expected.size != actual.size) {
        cout << "Error at n:" << " expected:" << expected.size << " actual: " << actual.size << endl;
        return false;
    }
    if (expected.maximum_element != actual.maximum_element) {
        cout << "Error at maximum_element:" << " expected:" << expected.maximum_element << " actual: " << actual.maximum_element << endl;
        return false;
    }
    if (expected.bit_size != actual.bit_size) {
        cout << "Error at num_of_bit:" << " expected:" << expected.bit_size << " actual: " << actual.bit_size << endl;
        return false;
    }

    vector<uint64_t> expected_bits, actual_bits;
    for (int i = 0; i < expected.size; ++i) {
        expected_bits.emplace_back(expected.access(i));
    }
    for (int i = 0; i < actual.size; ++i) {
        actual_bits.emplace_back(actual.access(i));
    }

    if (expected_bits != actual_bits) {
        return false;
    }

    return true;
}

bool test_erase(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        data.emplace_back((uint64_t)randxor() % num_of_alphabet);
    }

    DynamicWaveletTree actual(num_of_alphabet, data);

    for (int i = 0; i < num; ++i) {
        uint64_t pos = i == 0 ? 0 : randxor() % data.size();
        data.erase(data.begin() + pos);

        DynamicWaveletTree expected(num_of_alphabet, data);
        actual.erase(pos);

        if (not same(expected, actual)) {
            return false;
        }
    }

    return true;
}

bool test_insert(uint64_t num, uint64_t num_of_alphabet) {

    DynamicWaveletTree actual(num_of_alphabet);
    vector<uint64_t> data;
    for (int i = 0; i < num; ++i) {
        uint64_t pos = data.size() == 0 ? 0 : randxor() % data.size();
        uint64_t c = randxor() % num_of_alphabet;

        data.insert(data.begin() + pos, c);

        DynamicWaveletTree expected(num_of_alphabet, data);

        actual.insert(pos, c);
        if (not same(expected, actual)) {
            return false;
        }
    }

    return true;
}

bool test_update(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree actual(num_of_alphabet, data);

    for (int i = 0; i < num - 1; ++i) {
        uint64_t pos = randxor() % data.size();
        uint64_t c = randxor() % num_of_alphabet;
        data[pos] = c;

        DynamicWaveletTree expected(num_of_alphabet, data);
        actual.update(pos, c);

        if (not same(expected, actual)) {
            return false;
        }
    }

    return true;
}

bool test_quantile(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    DynamicWaveletTree dwt(num_of_alphabet, data);

    for (uint64_t begin = 0; begin < data.size(); ++begin) {
        for (uint64_t end = 0; end < data.size(); ++end) {
            for (uint64_t k = 0; k < data.size(); ++k) {

                std::vector<pair<uint64_t, uint64_t>> temp;
                for (uint64_t i = begin; i < end; ++i) {
                    temp.emplace_back(make_pair(data[i], i));
                }
                sort(temp.begin(), temp.end());

                uint64_t expected_val = NOTFOUND;
                uint64_t expected_pos = NOTFOUND;
                if (temp.size() > k) {
                    expected_val = temp.at(k).first;
                    expected_pos = temp.at(k).second;
                }

                auto pos = dwt.quantileRange(begin, end, k);
                if (pos == NOTFOUND and pos == expected_pos) {
                    continue;
                }
                auto val = dwt.access(pos);
                if (pos != expected_pos or val != expected_val) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_sum(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    set<uint64_t> se(data.begin(), data.end());
    DynamicWaveletTree dwt(num_of_alphabet, data);
    for (uint64_t s = 0; s < data.size(); ++s) {
        for (uint64_t e = s + 1; e < data.size(); ++e) {
            for (auto low : se) {
                for (auto high : se) {
                    if (low > high) {
                        continue;
                    }
                    uint64_t expect = 0;
                    for (uint64_t i = s; i < e; ++i) {
                        if (low <= data[i] and data[i] <= high) {
                            expect += data[i];
                        }
                    }

                    auto ans = dwt.sum(s, e, low, high);

                    if (ans != expect) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void speed_test(uint64_t num, uint64_t num_of_alphabet) {
    cout << "access:" << speed_access(num, num_of_alphabet) << "ms" << endl;
    cout << "rank:" << speed_rank(num, num_of_alphabet) << "ms" << endl;
    cout << "select:" << speed_select(num, num_of_alphabet) << "ms" << endl;
    cout << "insert:" << speed_insert(num, num_of_alphabet) << "ms" << endl;
    cout << "erase:" << speed_erase(num, num_of_alphabet) << "ms" << endl;
    cout << "update:" << speed_update(num, num_of_alphabet) << "ms" << endl;
    cout << "quantile:" << speed_quantile(num, num_of_alphabet) << "ms" << endl;
}


bool test(uint64_t num, uint64_t num_of_alphabet) {
    bool ok = true;

    cout << "test access:" << ((ok &= test_access(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test rank:" << ((ok &= test_rank(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test select:" << ((ok &= test_select(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test erase:" << (test_erase(num, num_of_alphabet) ? "OK" : "NG") << endl;
    cout << "test insert:" << (test_insert(num, num_of_alphabet) ? "OK" : "NG") << endl;
    cout << "test update:" << (test_update(num, num_of_alphabet) ? "OK" : "NG") << endl;
    cout << "test quantile:" << (test_quantile(num, num_of_alphabet) ? "OK" : "NG") << endl;
    cout << "test sum:" << (test_sum(num, num_of_alphabet) ? "OK" : "NG") << endl;

    return ok;
}

int main() {

    uint64_t num = 100000;
    uint64_t num_of_alphabet = 10000000;

//    cout << "SPEED" << endl;
//    speed_test(num, num_of_alphabet);

    cout << "TEST" << endl;
    for (int i = 0; i < 10000; ++i) {
        if (i == 438) {
            int aa = 0;
        }
        cout << "Test:" << i << endl;
        num = randxor() % 40 + 1;
        num_of_alphabet = randxor() % 4 + 1;
        if (not test(num, num_of_alphabet)) {
            cout << "ERROR" << endl;
            break;
        }
    }

    return 0;
}