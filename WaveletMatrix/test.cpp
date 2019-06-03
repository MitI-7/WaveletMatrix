#include<bits/stdc++.h>
#include "../waveletMatrix/WaveletMatrix.hpp"

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

    WaveletMatrix wm(data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < data.size(); ++i) {
        if (wm.access(i) == -1) {
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

    WaveletMatrix wm(data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (uint64_t rank = 1; rank < data.size(); ++rank) {
        uint64_t val = data[randxor() % num];
        if (wm.rank(val, rank) == -1) {
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

    WaveletMatrix wm(data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t val = data[randxor() % num];
        uint64_t rank  = randxor() % count.at(val) + 1;
        if (wm.select(val, rank) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_sum(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i]= (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        uint64_t begin = randxor() % num;
        uint64_t end  = randxor() % num;
        uint64_t x  = randxor() % num_of_alphabet;
        uint64_t y  = randxor() % num_of_alphabet;
        if (begin > end) {
            swap(begin, end);
        }
        if (x > y) {
            swap(x, y);
        }
        if (wm.rangeSum(begin, end, 0, num_of_alphabet) == -1) {
            dummy++;
        }
    }
    auto end = std::chrono::system_clock::now();
    if (dummy) {
        cout << "dummy" << endl;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double speed_quantile(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);

    int dummy = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < num; ++i) {
        if (wm.quantileRange(0, num, i) == -1) {
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

    WaveletMatrix wm(data);
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] != wm.access(i)) {
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

    WaveletMatrix wm(data);

    set<uint64_t> s(data.begin(), data.end());
    for (auto val : s) {
        for (uint64_t i = 0; i < data.size(); ++i) {

            int expected = 0;
            for (int j = 0; j < i; ++j) {
                expected += (data[j] == val);
            }

            if (wm.rank(val, i) != expected) {
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

    WaveletMatrix wm(data);

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

            if (wm.select(val, rank) != expected) {
                return false;
            }
        }
    }

    return true;
}

bool test_quantile(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);

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

                auto pos = wm.quantileRange(begin, end, k);
                auto val = wm.access(pos);
                if (pos != expected_pos or val != expected_val) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool test_rangeFreq(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);

    for (uint64_t begin = 0; begin < data.size(); ++begin) {
        for (uint64_t end = 0; end < data.size() + 1; ++end) {
            for (uint64_t min = 0; min < num_of_alphabet; ++min) {
                for (uint64_t max = 0; max < num_of_alphabet + 1; ++max) {

                    uint64_t expected = 0;
                    for (uint64_t i = begin; i < end; ++i) {
                        expected += (min <= data[i] and data[i] < max);
                    }

                    uint64_t actual = wm.rangeFreq(begin, end, min, max);
                    if (actual != expected) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


bool test_rankall(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);
    for (uint64_t begin = 0; begin < data.size(); ++begin) {
        for (uint64_t end = 0; end < data.size() + 1; ++end) {
            for (uint64_t c = 0; c < num_of_alphabet + 1; ++c) {

                // 期待値
                uint64_t expected_rank = 0, expected_rank_less_than = 0, expected_rank_more_than = 0;
                for (uint64_t i = begin; i < end; ++i) {
                    expected_rank += (data[i] == c);
                    expected_rank_less_than += (data[i] < c);
                    expected_rank_more_than += (data[i] > c);
                }

                // 実行値
                auto ans = wm.rankAll(c, begin, end);
                uint64_t rank, rank_less_than, rank_more_than;
                tie(rank, rank_less_than, rank_more_than) = ans;

                if (rank != expected_rank) {
                    return false;
                }
                if (rank_less_than != expected_rank_less_than) {
                    return false;
                }
                if (rank_more_than != expected_rank_more_than) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool test_topk(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);

    for (uint64_t s = 0; s < data.size(); ++s) {
        for (uint64_t e = s + 1; e < data.size(); ++e) {
            for (uint64_t k = 0; k < data.size() + 1; ++k) {

                vector<pair<uint64_t, int64_t>> expect;    // (頻度，値)
                map<uint64_t, uint64_t> count, pos;
                for (uint64_t i = s; i < e; ++i) {
                    if (count[data.at(i)] == 0) {
                        pos[data.at(i)] = i;
                    }
                    count[data.at(i)]++;
                }

                for (auto p : count) {
                    expect.emplace_back(make_pair(p.second, -p.first));
                }
                sort(expect.rbegin(), expect.rend());   // 頻度が大きく，値が小さい順にソート

                auto actual = wm.topk(s, e, k);

                for (uint64_t i = 0; i < min(k, expect.size()); ++i) {
                    if (actual[i].first != -expect[i].second or actual[i].second != expect[i].first) {
                        return false;
                    }
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
    WaveletMatrix wm(data);
    for (uint64_t s = 0; s < data.size(); ++s) {
        for (uint64_t e = s + 1; e < data.size(); ++e) {
            for (auto low : se) {
                for (auto high : se) {
                    if (low > high) {
                        continue;
                    }
                    uint64_t expect = 0;
                    for (uint64_t i = s; i < e; ++i) {
                        if (low <= data[i] and data[i] < high) {
                            expect += data[i];
                        }
                    }

                    auto ans = wm.rangeSum(s, e, low, high);

                    if (ans != expect) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool test_prevValue(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);
    for (uint64_t s = 0; s < data.size(); ++s) {
        for (uint64_t e = 0; e < data.size(); ++e) {
            for (uint64_t x = 0; x < num_of_alphabet + 10; ++x) {
                for (uint64_t y = 0; y < num_of_alphabet + 10; ++y) {

                    uint64_t expect = 0;
                    bool found = false;
                    for (uint64_t i = s; i < e; ++i) {
                        if (x <= data[i] and data[i] < y) {
                            expect = max(expect, data[i]);
                            found = true;
                        }
                    }

                    auto ans = wm.prevValue(s, e, x, y);

                    if (not found) {
                        expect = NOTFOUND;
                    }
                    if (ans != expect) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool test_nextValue(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);
    for (uint64_t s = 0; s < data.size(); ++s) {
        for (uint64_t e = 0; e < data.size(); ++e) {
            for (uint64_t x = 0; x < num_of_alphabet + 10; ++x) {
                for (uint64_t y = 0; y < num_of_alphabet + 10; ++y) {

                    uint64_t expect = NOTFOUND;
                    bool found = false;
                    for (uint64_t i = s; i < e; ++i) {
                        if (x <= data[i] and data[i] < y) {
                            expect = min(expect, data[i]);
                            found = true;
                        }
                    }

                    auto ans = wm.nextValue(s, e, x, y);

                    if (not found) {
                        expect = NOTFOUND;
                    }
                    if (ans != expect) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool test_intersect(uint64_t num, uint64_t num_of_alphabet) {
    vector<uint64_t> data(num);
    for (int i = 0; i < num; ++i) {
        data[i] = (uint64_t)randxor() % num_of_alphabet;
    }

    WaveletMatrix wm(data);

    for (uint64_t s1 = 0; s1 < data.size(); ++s1) {
        for (uint64_t e1 = s1 + 1; e1 < data.size(); ++e1) {
            for (uint64_t s2 = 0; s2 < data.size(); ++s2) {
                for (uint64_t e2 = s2 + 1; e2 < data.size(); ++e2) {

                    vector<tuple<uint64_t, uint64_t, uint64_t>> expect;
                    map<uint64_t, uint64_t> m1, m2;
                    for (uint64_t i = s1; i < e1; ++i) {
                        m1[data.at(i)]++;
                    }
                    for (uint64_t i = s2; i < e2; ++i) {
                        m2[data.at(i)]++;
                    }

                    for (auto p : m1) {
                        if (m2[p.first] != 0) {
                            expect.emplace_back(make_tuple(p.first, p.second, m2[p.first]));
                        }
                    }

                    auto actual = wm.intersect(s1, e1, s2, e2);
                    if (actual.size() != expect.size()) {
                        return false;
                    }
                    for (int i = 0; i < actual.size(); ++i) {
                        uint64_t v1, a1, b1;
                        tie(v1, a1, b1) = actual[i];
                        uint64_t v2, a2, b2;
                        tie(v2, a2, b2) = expect[i];

                        if (v1 != v2 or a1 != a2 or b1 != b2) {
                            return false;
                        }
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
    cout << "sum:" << speed_sum(num, num_of_alphabet) << "ms" << endl;
    cout << "quantile:" << speed_quantile(num, num_of_alphabet) << "ms" << endl;
}

bool test(uint64_t num, uint64_t num_of_alphabet) {
    bool ok = true;

    cout << "test access:" << ((ok &= test_access(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test rank:" << ((ok &= test_rank(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test select:" << ((ok &= test_select(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test quantile:" << ((ok &= test_quantile(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test rankall:" << ((ok &= test_rankall(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test topk:" << ((ok &= test_topk(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test sum:" << ((ok &= test_sum(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test rangeFreq:" << ((ok &= test_rangeFreq(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test prevValue:" << ((ok &= test_prevValue(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test nextValue:" << ((ok &= test_nextValue(num, num_of_alphabet)) ? "OK" : "NG") << endl;
    cout << "test intersect:" << ((ok &= test_intersect(num, num_of_alphabet)) ? "OK" : "NG") << endl;

    return ok;
}

int main() {

    uint64_t num = 100000;
    uint64_t num_of_alphabet = 10000000;

    cout << "SPEED" << endl;
    speed_test(num, num_of_alphabet);

    cout << "TEST" << endl;
    for (int i = 0; i < 1000; ++i) {
        cout << "Test:" << i << endl;
        num = randxor() % 100 + 1;
        num_of_alphabet = randxor() % 50 + 1;
        if (not test(num, num_of_alphabet)) {
            cout << "ERROR" << endl;
            break;
        }
    }

    return 0;
}