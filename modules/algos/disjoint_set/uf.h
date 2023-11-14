/**
 * https://github.com/ClaireLee22/Union-find-algorithm.git
 * 
*/
#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>  // std::iota
#include <ostream>

namespace algo {

template <unsigned int N>
struct UF {
    int count = N;              // number of components
    std::array<int, N> parent;  // parent[i] = parent of i

    UF() { std::iota(parent.begin(), parent.end(), 0); }

    // Returns the canonical element of the set containing element {@code p}.
    virtual int Find(int p) = 0;

    // Merges the set containing element {@code p} with the set * containing element {@code q}.
    virtual void Union(int p, int q) = 0;

    // Returns the number of sets.
    inline int Count() { return count; }

    // Returns true if the two elements are in the same set.
    inline bool Connected(int p, int q) { return Find(p) == Find(q); }

    virtual ~UF() = default;

    friend std::ostream& operator<<(std::ostream& os, UF& inst) {
        // Print Node Index
        os << std::left << std::setfill('.') << std::setw(15) << "Node ID: ";
        os << std::setfill(' ');
        for (int i = 0; i < inst.parent.size(); ++i) os << std::setw(5) << i;
        os << std::endl;

        // Print Parent Index
        os << std::left << std::setfill('.') << std::setw(15) << "Parent ID: ";
        os << std::setfill(' ');
        for (auto& p : inst.parent) os << std::setw(5) << p;
        os << std::endl;
        return os;
    }
};

template <unsigned int N>
struct QuickFind : public UF<N> {
    int Find(int p) override { return UF<N>::parent[p]; }

    void Union(int p, int q) override {
        int pID = UF<N>::parent[p];  // needed for correctness
        int qID = UF<N>::parent[q];  // to reduce the number of array accesses

        // p and q are already in the same component
        if (pID == qID) return;

        for (int i = 0; i < UF<N>::parent.size(); i++)
            if (UF<N>::parent[i] == pID) UF<N>::parent[i] = qID;
        UF<N>::count--;
    }

    virtual ~QuickFind() {}
};

// Weighted quick-union (without path compression)
template <unsigned int N>
struct QuickUnion : public UF<N> {
    int Find(int p) override {
        while (p != UF<N>::parent[p]) p = UF<N>::parent[p];
        return p;
    }

    void Union(int p, int q) override {
        int rootP = Find(p);
        int rootQ = Find(q);
        if (rootP == rootQ) return;
        UF<N>::parent[rootP] = rootQ;
        UF<N>::count--;
    }

    virtual ~QuickUnion() {}
};

template <unsigned int N>
struct WeightedQuickUnion : public UF<N> {
    std::array<int, N> size;  // size[i] = number of elements in subtree rooted at i

    WeightedQuickUnion() : UF<N>() { size.fill(0); }

    int Find(int p) override {
        while (p != UF<N>::parent[p]) p = UF<N>::parent[p];
        return p;
    }

    void Union(int p, int q) override {
        int rootP = Find(p);
        int rootQ = Find(q);
        if (rootP == rootQ) return;

        // make smaller root point to larger one
        if (size[rootP] < size[rootQ]) {
            this->parent[rootP] = rootQ;
            size[rootQ] += size[rootP];
        } else {
            this->parent[rootQ] = rootP;
            size[rootP] += size[rootQ];
        }
        UF<N>::count--;
    }

    virtual ~WeightedQuickUnion() {}
};

template <unsigned int N>
struct UnionFind : public UF<N> {
    std::array<int, N> rank;  // rank[i] = rank of subtree rooted at i (never more than 31)

    UnionFind() : UF<N>() { rank.fill(1); }

    int Find(int p) override {
        while (p != UF<N>::parent[p]) {
            UF<N>::parent[p] = UF<N>::parent[UF<N>::parent[p]];  // path compression by halving
            p = UF<N>::parent[p];
        }
        return p;
    }

    void Union(int p, int q) override {
        int rootP = Find(p);
        int rootQ = Find(q);
        if (rootP == rootQ) return;

        // make root of smaller rank point to root of larger rank
        /*
        if (rank[rootP] < rank[rootQ]) {
            UF<N>::parent[rootP] = rootQ;
        } else if (rank[rootP] > rank[rootQ]) {
            UF<N>::parent[rootQ] = rootP;
        } else {
            UF<N>::parent[rootQ] = rootP;
            rank[rootP]++;
        }
        */
        if (rank[rootP] < rank[rootQ]) {
            UF<N>::parent[rootP] = rootQ;
            rank[rootQ]++;
        } else {
            UF<N>::parent[rootQ] = rootP;
            rank[rootP]++;
        }
        UF<N>::count--;
    }

    virtual ~UnionFind() {}
};

}  // namespace algo