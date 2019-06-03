#include <bits/stdc++.h>
#include "../dynamicBitVector/DynamicBitVector.hpp"

class DynamicWaveletMatrix {
public:
    std::vector<DynamicBitVector> bit_arrays;
    std::vector<uint64_t> begin_one;               // 各bitの1の開始位置

    uint64_t size;                                 // 与えられた配列のサイズ
    uint64_t maximum_element;                      // 最大の文字
    uint64_t bit_size;                             // 文字を表すのに必要なbit数

public:
    // max_element: 入ってくる中で一番大きい数値
    DynamicWaveletMatrix (uint64_t maximum_element) : size(0), maximum_element(maximum_element + 1) {
        this->bit_size = this->get_num_of_bit(maximum_element);
        if (bit_size == 0) {
            bit_size = 1;
        }
        this->begin_one.resize(bit_size);

        for (uint64_t i = 0; i < bit_size; ++i) {
            DynamicBitVector sv;
            bit_arrays.push_back(sv);
        }
    }

    DynamicWaveletMatrix (uint64_t num_of_alphabet, const std::vector<uint64_t> &array) : size(0), maximum_element(num_of_alphabet + 1) {
        this->bit_size = this->get_num_of_bit(num_of_alphabet);
        if (bit_size == 0) {
            bit_size = 1;
        }
        this->begin_one.resize(bit_size);

        if (array.empty()) {
            for (uint64_t i = 0; i < bit_size; ++i) {
                DynamicBitVector sv;
                bit_arrays.push_back(sv);
            }
            return;
        }

        size = array.size();

        std::vector<uint64_t> v(array), b(array.size(), 0);

        for (uint64_t i = 0; i < bit_size; ++i) {

            std::vector<uint64_t> temp;
            // 0をtempにいれてく
            for (uint64_t j = 0; j < v.size(); ++j) {
                uint64_t c = v.at(j);
                uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
                if (bit == 0) {
                    temp.push_back(c);
                    b[j] = 0;
                }
            }

            this->begin_one.at(i) = temp.size();

            // 1をtempにいれてく
            for (uint64_t j = 0; j < v.size(); ++j) {
                uint64_t c = v.at(j);
                uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
                if (bit == 1) {
                    temp.push_back(c);
                    b[j] = 1;
                }
            }

            DynamicBitVector dbv(b);
            bit_arrays.emplace_back(dbv);
            v = temp;
        }
    }

    // v[pos]
    uint64_t access(uint64_t pos) {
        if (pos >= this->size) { return NOTFOUND; }

        uint64_t c = 0;
        for (uint64_t i = 0; i < bit_arrays.size(); ++i) {
            uint64_t bit = bit_arrays.at(i).access(pos);   // もとの数値がのi番目のbit
            c = (c <<= 1) | bit;
            pos = bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
        }
        return c;
    }

    // v[0, pos)のcの数
    uint64_t rank(uint64_t c, uint64_t pos) {
        assert(pos <= size);
        if (c >= maximum_element) {
            return 0;
        }

        uint64_t left = 0, right = pos;
        for (uint64_t i = 0; i < bit_size; ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;  // 上からi番目のbit
            left = bit_arrays.at(i).rank(bit, left);             // cのi番目のbitと同じ数値の数
            right = bit_arrays.at(i).rank(bit, right);           // cのi番目のbitと同じ数値の数
            if (bit) {
                left += this->begin_one.at(i);
                right += this->begin_one.at(i);
            }
        }

        return right - left;
    }

    // i番目のcの位置 + 1を返す。rankは1-origin
    uint64_t select(uint64_t c, uint64_t rank) {
        assert(rank > 0);
        if (c >= maximum_element) {
            return NOTFOUND;
        }

        uint64_t left = 0;
        for (uint64_t i = 0; i < bit_size; ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;  // 上からi番目のbit
            left = bit_arrays.at(i).rank(bit, left);               // cのi番目のbitと同じ数値の数
            if (bit) {
                left += this->begin_one.at(i);
            }
        }

        uint64_t index = left + rank;
        for (uint64_t i = 0; i < bit_arrays.size(); ++i){
            uint64_t bit = ((c >> i) & 1);      // 下からi番目のbit
            if (bit == 1) {
                index -= this->begin_one.at(bit_size - i - 1);
            }
            index = this->bit_arrays.at(bit_size - i - 1).select(bit, index);
        }
        return index;
    }

    // posにcを挿入する
    void insert(uint64_t pos, uint64_t c) {
        assert(pos <= this->size);

        for (uint64_t i = 0; i < bit_arrays.size(); ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
            bit_arrays.at(i).insert(pos, bit);
            pos = bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
            else {
                this->begin_one.at(i)++;
            }
        }

        this->size++;
    }

    // 末尾にcを追加する
    void push_back(uint64_t c) {
        this->insert(this->size, c);
    }

    // posを削除する
    void erase(uint64_t pos) {
        assert(pos < this->size);
        if (pos >= this->size) {
            throw "Segmentation fault";
        }

        for (uint64_t i = 0; i < bit_arrays.size(); ++i) {
            uint64_t bit = bit_arrays.at(i).access(pos);   // もとの数値のi番目のbit

            auto next_pos = bit_arrays.at(i).rank(bit, pos);
            bit_arrays.at(i).erase(pos);

            if (bit) {
                next_pos += this->begin_one.at(i);
            }
            else {
                this->begin_one.at(i)--;
            }
            pos = next_pos;
        }
        this->size--;
    }

    // posにcをセットする
    void update(uint64_t pos, uint64_t c) {
        assert(pos < this->size);
        this->erase(pos);
        this->insert(pos, c);
    }

    // 他の操作は通常のWavelet Matrixと同じ

private:
    uint64_t get_num_of_bit(uint64_t x) {
        if (x == 0) return 0;
        x--;
        uint64_t bit_num = 0;
        while (x >> bit_num) {
            ++bit_num;
        }
        return bit_num;
    }
};