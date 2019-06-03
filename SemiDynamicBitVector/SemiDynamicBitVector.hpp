#pragma once
#include<bits/stdc++.h>

enum {
    NOTFOUND = 0xFFFFFFFFFFFFFFFFLLU
};


class FenwickTree {
public:
    const unsigned int n;
    std::vector<int> v;   // active: 0, inactive: 1

    FenwickTree(unsigned long n) : n(n) {
        this->v.assign(n + 1, 0);
    }

    bool isActive(const int pos) {
        int num = this->countInactive(pos + 1) - countInactive(pos);
        return num == 0;
    }

    int countInactive(int pos) {
        assert(0 <= pos && pos <= this->n);
        if (pos == 0) {
            return 0;
        }

        long long s = 0;
        pos -= 1;
        while (pos >= 0) {
            s += this->v[pos];
            pos = (pos & (pos + 1)) - 1;
        }
        return s;
    }

    int countInactive(int left, int right) {
        assert(left <= right);
        if (left == right) {
            return 0;
        }
        return this->countInactive(right) - this->countInactive(left);
    }

    void toActive(int pos) {
        if (not this->isActive(pos)) {
            this->add(pos, -1);
        }
    }

    void toAllActive() {
        std::fill(this->v.begin(), this->v.end(), 0);
    }

    void toInactive(int pos) {
        if (this->isActive(pos)) {
            this->add(pos, 1);
        }
    }

    // 対応していないところが1になってしまうので基本使わない
    void toAllInActive() {
        this->v.at(0) = 1;
        for (int i = 1; i < this->n; i++) {
            this->v.at(i) = this->v.at(i - 1) + 1;
        }

        for (int i = this->n - 1; i > 0; --i) {
            int lower_i = (i & (i + 1)) - 1;
            if (lower_i >= 0) {
                this->v.at(i) -= this->v.at(lower_i);
            }
        }
    }

private:
    // v[i] += x(0-origin)
    void add(int i, int x) {
        assert(i < this->n);

        while (i < this->n) {
            this->v.at(i) += x;
            i |= i + 1;
        }
    }
};


class SemiDynamicBitVector {
public:
    const uint64_t size;    // ビットベクトルのサイズ
    static const uint64_t blockBitNum = 16;
    static const uint64_t LEVEL_L = 512;
    static const uint64_t LEVEL_S = 16;

    std::vector<uint64_t> L;   // 大ブロック
    std::vector<uint16_t> S;   // 小ブロック
    std::vector<uint16_t> B;   // ビットベクトル

    uint64_t numOne = 0;       // 1bitの数

    FenwickTree state0;        // bitが0のinactiveを管理(bitが1の位置のものはactiveにしておく)
    FenwickTree state1;        // bitが1のinactiveを管理(bitが0の位置のものはactiveにしておく)

public:
    explicit SemiDynamicBitVector(const uint64_t n) : size(n), state0(n), state1(n) {
        const uint64_t s = (n + blockBitNum - 1) / blockBitNum + 1;   // ceil(n, blockSize)
        this->B.assign(s, 0);
        this->L.assign(n / LEVEL_L + 1, 0);
        this->S.assign(n / LEVEL_S + 1, 0);
    }

    // B[pos] = bit
    void setBit(const uint64_t bit, const uint64_t pos){
        assert(bit == 0 or bit == 1);
        assert(pos < this->size);

        const uint64_t blockPos = pos / blockBitNum;
        const uint64_t offset = pos % blockBitNum;
        if (bit == 1) {
            B.at(blockPos) |= (1LLU << offset);
            state1.toInactive(pos);
            state0.toActive(pos);
        }
        else {
            B.at(blockPos) &= (~(1LLU << offset));
            state0.toInactive(pos);
            state1.toActive(pos);
        }
    }

    // initial all inactive
    void build() {
        uint64_t num = 0;
        for (uint64_t i = 0; i <= size; i++){
            if (i % LEVEL_L == 0) {
                L.at(i / LEVEL_L) = num;
            }
            if (i % LEVEL_S == 0) {
                S.at(i / LEVEL_S) = num - L.at(i / LEVEL_L);
            }
            if (i != size and i % blockBitNum == 0) {
                num += this->popCount(this->B.at(i / blockBitNum));
            }
        }
        this-> numOne = num;
    }

    // B[pos]
    uint64_t access(const uint64_t pos) {
        assert(pos < this->size);
        const uint64_t blockPos = pos / blockBitNum;
        const uint64_t offset   = pos % blockBitNum;
        return ((B.at(blockPos) >> offset) & 1);
    }

    // B[0, pos)のbitの数
    uint64_t rank(const uint64_t bit, const uint64_t pos) {
        assert(bit == 0 or bit == 1);
        assert(pos <= this->size);

        if (bit) {
            uint64_t r = L[pos / LEVEL_L] + S[pos / LEVEL_S] + popCount(B[pos / blockBitNum] & ((1 << (pos % blockBitNum)) - 1));
            return r;
        }
        else {
            uint64_t r = pos - rank(1, pos);
            return r;
        }
    }

    // rank only active element
    uint64_t activeRank(const uint64_t bit, const uint64_t pos) {
        assert(bit == 0 or bit == 1);
        assert(pos <= this->size);

        if (bit) {
            uint64_t r = L[pos / LEVEL_L] + S[pos / LEVEL_S] + popCount(B[pos / blockBitNum] & ((1 << (pos % blockBitNum)) - 1));
            uint64_t x = this->state1.countInactive(pos);    // あとでまとめる
            return r - x;
        }
        else {
            uint64_t r = pos - rank(1, pos);
            uint64_t x = this->state0.countInactive(pos); // あとでまとめる
            return r - x;
        }
    }

    // rank番目のbitの位置 + 1(rankは1-origin)
    uint64_t select(const uint64_t bit, const uint64_t rank) {
        assert(bit == 0 or bit == 1);
        assert(rank > 0);
        if (bit == 0 and rank > this->size - this-> numOne) { return NOTFOUND; }
        if (bit == 1 and rank > this-> numOne)              { return NOTFOUND; }

        // 大ブロックL内を検索
        uint64_t large_idx = 0;
        {
            uint64_t left = 0;
            uint64_t right = L.size();
            while (right - left > 1) {
                uint64_t mid = (left + right) / 2;
                uint64_t r = L.at(mid);
                r = (bit) ? r : mid * LEVEL_L - L.at(mid);

                if (r < rank) {
                    left = mid;
                    large_idx = mid;
                } else {
                    right = mid;
                }
            }
        }

        // 小ブロックS内を検索
        uint64_t small_idx = (large_idx * LEVEL_L) / LEVEL_S;
        {
            uint64_t left = (large_idx * LEVEL_L) / LEVEL_S;
            uint64_t right = std::min(((large_idx  + 1) * LEVEL_L) / LEVEL_S, (uint64_t)S.size());
            while (right - left > 1) {
                uint64_t mid = (left + right) / 2;
                uint64_t r = L.at(large_idx) + S.at(mid);
                r = (bit) ? r :mid * LEVEL_S - r;

                if (r < rank) {
                    left = mid;
                    small_idx = mid;
                } else {
                    right = mid;
                }
            }
        }

        // Bをブロック単位で順番に探索
        uint64_t rank_pos = 0;
        {
            const uint64_t begin_block_idx = (small_idx * LEVEL_S) / blockBitNum;
            uint64_t total_bit = L.at(large_idx) + S.at(small_idx);
            if (bit == 0) {
                total_bit = small_idx * LEVEL_S - total_bit;
            }
            for (uint64_t i = 0;; ++i) {
                uint64_t b = popCount(B.at(begin_block_idx + i));
                if (bit == 0) {
                    b = blockBitNum - b;
                }
                if (total_bit + b >= rank) {
                    uint64_t block = (bit) ? B.at(begin_block_idx + i) : ~B.at(begin_block_idx + i);
                    rank_pos = (begin_block_idx + i) * blockBitNum + selectInBlock(block, rank - total_bit);
                    break;
                }
                total_bit += b;
            }
        }

        return rank_pos + 1;
    }

    // select only active element
    uint64_t activeSelect(const uint64_t bit, const uint64_t rank) {
        assert(bit == 0 or bit == 1);
        assert(rank > 0);
        if (bit == 0 and rank > this->size - this-> numOne) { return NOTFOUND; }
        if (bit == 1 and rank > this-> numOne)              { return NOTFOUND; }

        // 大ブロックL内を検索
        uint64_t large_idx = 0;
        {
            uint64_t left = 0;
            uint64_t right = L.size();
            while (right - left > 1) {
                uint64_t mid = (left + right) / 2;
                uint64_t r = L.at(mid); // [0, mid)にある1の数
                if (bit) {
                    r = r - this->state1.countInactive(mid * LEVEL_L);                         // [0, mid)にあるactiveな1の数
                }
                else {
                    r = mid * LEVEL_L - L.at(mid) - this->state0.countInactive(mid * LEVEL_L); // [0, mid)にあるactiveな0の数
                }

                if (r < rank) {
                    left = mid;
                    large_idx = mid;
                } else {
                    right = mid;
                }
            }
        }

        // 小ブロックS内を検索
        uint64_t small_idx = (large_idx * LEVEL_L) / LEVEL_S;
        {
            uint64_t left = (large_idx * LEVEL_L) / LEVEL_S;
            uint64_t right = std::min(((large_idx  + 1) * LEVEL_L) / LEVEL_S, (uint64_t)S.size());
            while (right - left > 1) {
                uint64_t mid = (left + right) / 2;
                uint64_t r = L.at(large_idx) + S.at(mid);   // [0, mid)にある1の数
                if (bit) {
                    r = r - this->state1.countInactive(mid * LEVEL_S);                 // [0, mid]にあるactiveな1の数
                }
                else {
                    r = mid * LEVEL_S - r - this->state0.countInactive(mid * LEVEL_S); // [0, mid)にあるactiveな0の数
                }

                if (r < rank) {
                    left = mid;
                    small_idx = mid;
                } else {
                    right = mid;
                }
            }
        }

        // Bをブロック単位で順番に探索
        uint64_t rank_pos = 0;
        {
            const uint64_t begin_block_idx = (small_idx * LEVEL_S) / blockBitNum;
            uint64_t total_bit = L.at(large_idx) + S.at(small_idx);
            if (bit) {
                total_bit = total_bit - this->state1.countInactive(small_idx * LEVEL_S);
            }
            else {
                total_bit = small_idx * LEVEL_S - total_bit - this->state0.countInactive(small_idx * LEVEL_S);
            }

            const uint64_t base = small_idx * LEVEL_S;
            for (uint64_t i = 0; begin_block_idx + i < B.size(); ++i) {
                if ((begin_block_idx + i) * blockBitNum > this->size) {
                    break;
                }

                uint64_t b = popCount(B.at(begin_block_idx + i));   // ブロック内の1の数
                if (bit) {
                    b = b - this->state1.countInactive((begin_block_idx + i) * blockBitNum,
                                                       std::min(this->size, (begin_block_idx + i + 1) * blockBitNum));
                }
                else {
                    b = (blockBitNum - b) - this->state0.countInactive((begin_block_idx + i) * blockBitNum,
                                                                       std::min(this->size, (begin_block_idx + i + 1) *
                                                                                            blockBitNum));
                }

                // blockをすべていれると超えるならblock内を探索
                if (total_bit + b >= rank) {
                    uint64_t block = (bit) ? B.at(begin_block_idx + i) : ~B.at(begin_block_idx + i);
                    uint64_t x = activeSelectInBlock(block, rank - total_bit, (begin_block_idx + i) * blockBitNum, bit);

                    if (x == NOTFOUND) {
                        return NOTFOUND;
                    }

                    rank_pos = (begin_block_idx + i) * blockBitNum + x;
                    return rank_pos + 1;
                }
                total_bit += b;
            }
        }

        return NOTFOUND;
    }

    bool isActive(const uint64_t pos) {
        if (this->access(pos) == 0) {
            return this->state0.isActive(pos);
        }
        else {
            return this->state1.isActive(pos);
        }
    }

    void toActive(const uint64_t pos) {
        if (this->access(pos) == 0) {
            this->state0.toActive(pos);
        }
        else {
            this->state1.toActive(pos);
        }
    }

    void toAllActive() {
        this->state0.toAllActive();
        this->state1.toAllActive();
    }

    void toInactive(const uint64_t pos) {
        if (this->access(pos) == 0) {
            this->state0.toInactive(pos);
        }
        else {
            this->state1.toInactive(pos);
        }
    }

    uint64_t getNumOne() const {
        return numOne;
    }

    void debug() {
        std::cout << "LEVEL_L(" << L.size() << ")" << std::endl;
        for (uint64_t i = 0 ; i < L.size(); ++i) {
            std::cout << L.at(i) << ", ";
        }
        std::cout << std::endl;
        std::cout << "LEVEL_S(" << S.size() << ")" << std::endl;
        for (uint64_t i = 0 ; i < S.size(); ++i) {
            std::cout << S.at(i) << ", ";
        }
        std::cout << std::endl;
    }

private:
    uint64_t popCount(uint64_t x) {
        x = (x & 0x5555555555555555ULL) + ((x >> 1) & 0x5555555555555555ULL);
        x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
        x = x + (x >>  8);
        x = x + (x >> 16);
        x = x + (x >> 32);
        return x & 0x7FLLU;
    }

    uint64_t selectInBlock(uint64_t x, uint64_t rank) {
        uint64_t x1 = x - ((x & 0xAAAAAAAAAAAAAAAALLU) >> 1);
        uint64_t x2 = (x1 & 0x3333333333333333LLU) + ((x1 >> 2) & 0x3333333333333333LLU);
        uint64_t x3 = (x2 + (x2 >> 4)) & 0x0F0F0F0F0F0F0F0FLLU;

        uint64_t pos = 0;
        for (;;  pos += 8){
            uint64_t rank_next = (x3 >> pos) & 0xFFLLU;
            if (rank <= rank_next) break;
            rank -= rank_next;
        }

        uint64_t v2 = (x2 >> pos) & 0xFLLU;
        if (rank > v2) {
            rank -= v2;
            pos += 4;
        }

        uint64_t v1 = (x1 >> pos) & 0x3LLU;
        if (rank > v1){
            rank -= v1;
            pos += 2;
        }

        uint64_t v0  = (x >> pos) & 0x1LLU;
        if (v0 < rank){
            rank -= v0;
            pos += 1;
        }

        return pos;
    }

    uint64_t activeSelectInBlock(uint64_t block, uint64_t rank, int base, int bit) {
        for (int i = 0; base + i < this->size; ++i) {
            if (bit and not this->state1.isActive(base + i)) {
                continue;
            }
            if (not bit and not this->state0.isActive(base + i)) {
                continue;
            }

            if (block & (1 << i)) {
                rank--;
            }
            if (rank <= 0) {
                return i;
            }
        }

        return NOTFOUND;
    }
};
