#pragma once
#include<bits/stdc++.h>
#include "../SemiDynamicBitVector/SemiDynamicBitVector.hpp"


class SemiDynamicWaveletMatrix {
private:
    std::vector<SemiDynamicBitVector> bit_arrays;
    std::vector<uint64_t> begin_one;                         // 各bitでの1の開始位置
    std::unordered_map<uint64_t, uint64_t> begin_alphabet;   // 最後のソートされた配列で各文字の開始位置
    FenwickTree leafState;

    uint64_t size;       // 与えられた配列のサイズ
    uint64_t sigma;      // 文字数
    uint64_t bit_size;   // 文字を表すのに必要なbit数

public:
    uint64_t num_active = 0; // num of active elements

    SemiDynamicWaveletMatrix (const std::vector<uint64_t> &array) : size(array.size()), leafState(array.size()) {
        assert(array.size() > 0);
        sigma =  *max_element(array.begin(), array.end()) + 1;
        bit_size = std::max((uint64_t)1, get_num_of_bit(sigma));

        for (uint64_t i = 0; i < bit_size; ++i) {
            SemiDynamicBitVector sv(size);
            bit_arrays.push_back(sv);
        }
        this->begin_one.resize(bit_size);

        std::vector<uint64_t> v(array);
        for (uint64_t i = 0; i < bit_size; ++i) {

            std::vector<uint64_t> temp;
            // 0をtempにいれてく
            for (uint64_t j = 0; j < v.size(); ++j) {
                uint64_t c = v.at(j);
                uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
                if (bit == 0) {
                    temp.push_back(c);
                    bit_arrays.at(i).setBit(0, j);
                }
            }

            this->begin_one.at(i) = temp.size();

            // 1をtempにいれてく
            for (uint64_t j = 0; j < v.size(); ++j) {
                uint64_t c = v.at(j);
                uint64_t bit = (c >> (this->bit_size - i - 1)) & 1;  //　上からi番目のbit
                if (bit == 1) {
                    temp.push_back(c);
                    this->bit_arrays.at(i).setBit(1, j);
                }
            }

            this->bit_arrays.at(i).build();
            v = temp;
        }

        // ソートされた配列内での各文字の位置を取得
        for (int i = v.size() - 1; i >= 0; --i) {
            this->begin_alphabet[v.at(i)] = i;
        }

        this->leafState.toAllInActive();
    }

    bool isActive(uint64_t pos) {
        return this->bit_arrays.at(0).isActive(pos);
    }

    void toActive(uint64_t pos) {
        assert(pos < this->size);

        if (this->isActive(pos)) {
            return;
        }

        for (uint64_t i = 0; i < this->bit_arrays.size(); ++i) {
            const uint64_t bit = this->bit_arrays.at(i).access(pos);   // もとの数値のi番目のbit
            this->bit_arrays.at(i).toActive(pos);

            pos = this->bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
        }
        this->leafState.toActive(pos);
        this->num_active++;
    }

    void toActiveAll() {
        for (int i = 0; i < this->bit_arrays.size(); ++i) {
            this->bit_arrays.at(i).toAllActive();
        }
        this->leafState.toAllActive();
        this->num_active = this->size;
    }

    void toInactive(uint64_t pos) {
        assert(pos < this->size);

        if (not this->isActive(pos)) {
            return;
        }

        for (uint64_t i = 0; i < this->bit_arrays.size(); ++i) {
            const uint64_t bit = this->bit_arrays.at(i).access(pos);   // もとの数値のi番目のbit
            this->bit_arrays.at(i).toInactive(pos);

            pos = this->bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
        }

        this->leafState.toInactive(pos);
        this->num_active--;
    }

    // v[pos]
    uint64_t access(uint64_t pos) {
        if (pos >= this->size) { return NOTFOUND; }

        uint64_t c = 0;
        for (uint64_t i = 0; i < this->bit_arrays.size(); ++i) {
            uint64_t bit = this->bit_arrays.at(i).access(pos);   // もとの数値のi番目のbit
            c = (c <<= 1) | bit;
            pos = this->bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
        }
        return c;
    }

    // v[0, pos)のcの数
    uint64_t rank(const uint64_t c, uint64_t pos) {
        assert(pos <= size);
        if (c >= this->sigma) {
            return 0;
        }
        if (this->begin_alphabet.find(c) == this->begin_alphabet.end()) {
            return 0;
        }

        for (uint64_t i = 0; i < this->bit_size; ++i) {
            uint64_t bit = (c >> (this->bit_size - i - 1)) & 1;  // 上からi番目のbit
            pos = this->bit_arrays.at(i).rank(bit, pos);   // cのi番目のbitと同じ数値の数
            if (bit) {
                pos += this->begin_one.at(i);
            }
        }

        const uint64_t begin_pos = this->begin_alphabet.at(c);
        return pos - begin_pos - this->leafState.countInactive(begin_pos, pos);
    }

    // activeな要素のみでrank番目のcの位置 + 1を返す。rankは1-origin
    uint64_t select(uint64_t c, uint64_t rank) {
        assert(rank > 0);
        if (c >= this->sigma) {
            return NOTFOUND;
        }
        if (this->begin_alphabet.find(c) == this->begin_alphabet.end()) {
            return NOTFOUND;
        }

        // 葉でactiveな要素のみを考慮したときのrank番目のcの位置を見つける
        uint64_t index = find(this->begin_alphabet.at(c), this->size, c, rank);

        for (uint64_t i = 0; i < this->bit_arrays.size(); ++i){
            uint64_t bit = ((c >> i) & 1);      // 下からi番目のbit
            if (bit == 1) {
                index -= this->begin_one.at(this->bit_size - i - 1);
            }
            index = this->bit_arrays.at(this->bit_size - i - 1).select(bit, index);
        }

        return index;
    }

    uint64_t findMedian() {
        return this->quantileRange(0, this->size, (this->num_active - 1) / 2);
    }

    // v[begin_pos, end_pos)でk番目に小さい数値のindexを返す(kは0-origin)
    // つまり小さい順に並べてk番目の値のindex
    uint64_t quantileRange(const uint64_t begin_pos, const uint64_t end_pos, int k) {
        if ((end_pos > this->size || begin_pos >= end_pos) || (k >= end_pos - begin_pos)) {
            return NOTFOUND;
        }

        uint64_t begin = begin_pos;
        uint64_t end = end_pos;

        uint64_t val = 0;
        for (uint64_t i = 0; i < this->bit_size; ++i) {
            uint64_t num_of_zero_begin = this->bit_arrays.at(i).activeRank(0, begin);
            uint64_t num_of_zero_end = this->bit_arrays.at(i).activeRank(0, end);
            const uint64_t num_of_zero = num_of_zero_end - num_of_zero_begin;     // beginからendまでにあるactiveな0の数
            const uint64_t bit = (k < num_of_zero) ? 0 : 1;                 // k番目の値のi番目のbitが0か1か

            num_of_zero_begin = this->bit_arrays.at(i).rank(0, begin);
            num_of_zero_end = this->bit_arrays.at(i).rank(0, end);

            if (bit) {
                k -= num_of_zero;
                begin = this->begin_one.at(i) + begin - num_of_zero_begin;
                end = this->begin_one.at(i) + end - num_of_zero_end;
            }
            else {
                begin = num_of_zero_begin;
                end = num_of_zero_end;
            }

            if ((begin > end) or ((end - begin) < k)) {
                return NOTFOUND;
            }

            val = ((val << 1) | bit);
        }

        if (this->begin_alphabet.find(val) == this->begin_alphabet.end()) {
            return NOTFOUND;
        }

        // [begin, end)でactiveな要素のみでk + 1番目のindex
        uint64_t index = find(begin, end, val, k + 1);
        if (index == NOTFOUND) {
            return NOTFOUND;
        }

        uint64_t rank = index - begin_alphabet.at(val);
        // rank番目のvalの位置を取得
        uint64_t idx = this->normalSelect(val, rank);
        if (idx == NOTFOUND) {
            return NOTFOUND;
        }
        idx--;

        if (idx < begin_pos or idx > end_pos or this->access(idx) != val or not this->isActive(idx)) {
            return NOTFOUND;
        }

        return idx;
    }

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

    // i番目のcの位置 + 1を返す。rankは1-origin
    // activeとinactiveの両方を考慮する
    uint64_t normalSelect(uint64_t c, uint64_t rank) {
        assert(rank > 0);
        if (c >= this->sigma) {
            return NOTFOUND;
        }
        if (this->begin_alphabet.find(c) == this->begin_alphabet.end()) {
            return NOTFOUND;
        }

        uint64_t index = this->begin_alphabet.at(c) + rank;
        for (uint64_t i = 0; i < bit_arrays.size(); ++i){
            uint64_t bit = ((c >> i) & 1);      // 下からi番目のbit
            if (bit == 1) {
                index -= this->begin_one.at(bit_size - i - 1);
            }
            index = this->bit_arrays.at(bit_size - i - 1).select(bit, index);
        }
        return index;
    }

    // activeな要素のみを考慮したとき，葉の[begin, end)でrank番目のcのindex
    uint64_t find(const uint64_t begin, const uint64_t end, const uint64_t c, const uint64_t rank) {
        uint64_t index = NOTFOUND;
        long long left = begin - 1;
        long long right = end + 1;
        while (right - left > 1) {
            long long mid = (left + right) / 2;
            long long num = (mid - begin) - this->leafState.countInactive(begin, mid);    // [begin, mid)にあるactiveな要素数

            if (num < rank) {
                left = mid;
            } else {
                if (num == rank) {
                    index = mid;
                }
                right = mid;
            }
        }

        if (index == NOTFOUND or index > end) {
            return NOTFOUND;
        }

        return index;
    }
};
