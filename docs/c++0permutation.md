# [Permutation](https://en.wikipedia.org/wiki/Permutation#Algorithms_to_generate_permutations)

## Algorithms to generate permutations

In computing it may be required to generate permutations of a given sequence of values. The methods best adapted to do this depend on whether one wants some randomly chosen permutations, or all permutations, and in the latter case if a specific ordering is required. Another question is whether possible equality among entries in the given sequence is to be taken into account; if so, one should only generate distinct multiset permutations of the sequence.

## Generation in lexicographic order
There are many ways to systematically generate all permutations of a given sequence.[52] One classic, simple, and flexible algorithm is based upon finding the next permutation in lexicographic ordering, if it exists. It can handle repeated values, for which case it generates each distinct multiset permutation once. Even for ordinary permutations it is significantly more efficient than generating values for the Lehmer code in lexicographic order (possibly using the factorial number system) and converting those to permutations. It begins by sorting the sequence in (weakly) increasing order (which gives its lexicographically minimal permutation), and then repeats advancing to the next permutation as long as one is found. The method goes back to Narayana Pandita in 14th century India, and has been rediscovered frequently.

The following algorithm generates the next permutation lexicographically after a given permutation. It changes the given permutation in-place.

- Find the largest index k such that a[k] < a[k + 1]. If no such index exists, the permutation is the last permutation.
- Find the largest index l greater than k such that a[k] < a[l].
- Swap the value of a[k] with that of a[l].
- Reverse the sequence from a[k + 1] up to and including the final element a[n].

For example, given the sequence _[1, 2, 3, 4]_ (which is in increasing order), and given that the index is zero-based, the steps are as follows:

- Index k = 2, because 3 is placed at an index that satisfies condition of being the largest index that is still less than a[k + 1] which is 4.
- Index l = 3, because 4 is the only value in the sequence that is greater than 3 in order to satisfy the condition a[k] < a[l].
- The values of a[2] and a[3] are swapped to form the new sequence [1, 2, 4, 3].
- The sequence after k-index a[2] to the final element is reversed. Because only one value lies after this index (the 3), the sequence remains unchanged in this instance. Thus the lexicographic successor of the initial state is permuted: [1, 2, 4, 3].

Following this algorithm, the next lexicographic permutation will be [1, 3, 2, 4], and the 24th permutation will be [4, 3, 2, 1] at which point a[k] < a[k + 1] does not exist, indicating that this is the last permutation.

This method uses about 3 comparisons and 1.5 swaps per permutation, amortized over the whole sequence, not counting the initial sort

## Examples

- leetcode: https://leetcode.cn/problems/next-permutation/solutions/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china

- modules/basis/stl_algorithm/next_permutation.cpp
