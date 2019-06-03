#include<bits/stdc++.h>


enum {
    NOTFOUND = 0xFFFFFFFFFFFFFFFFLLU
};

class SuccinctBitVector {
private:
    const uint64_t size;    // ビットベクトルのサイズ
    static const uint64_t blockBitNum = 16;
    static const uint64_t LEVEL_L = 512;
    static const uint64_t LEVEL_S = 16;

    std::vector<uint64_t> L;   // 大ブロック
    std::vector<uint16_t> S;   // 小ブロック
    std::vector<uint16_t> B;   // ビットベクトル

    uint64_t numOne = 0;       // 1bitの数

public:
    explicit SuccinctBitVector(const uint64_t n) : size(n) {
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
        if (bit == 1) { B.at(blockPos) |= (1LLU << offset); }
        else          { B.at(blockPos) &= (~(1LLU << offset)); }
    }

    // B[pos]
    uint64_t access(const uint64_t pos) {
        assert(pos < this->size);
        const uint64_t blockPos = pos / blockBitNum;
        const uint64_t offset   = pos % blockBitNum;
        return ((B.at(blockPos) >> offset) & 1);
    }

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

    // B[0, pos)のbitの数
    uint64_t rank(const uint64_t bit, const uint64_t pos) {
        assert(bit == 0 or bit == 1);
        assert(pos <= this->size);

        if (bit) {
            return L[pos / LEVEL_L] + S[pos / LEVEL_S] + popCount(B[pos / blockBitNum] & ((1 << (pos % blockBitNum)) - 1));
        }
        else {
            return pos - rank(1, pos);
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
};
