# Long finite sequences: computer search for Friedman's m(4) and n(3) bounds

Code and data for constructing long sequences with Friedman's property \* from
H. Friedman, *Long Finite Sequences*, J. Combin. Theory Ser. A 95 (2001), and for turning
them into "special" sequences, which give lower bounds for n(3).

## Results

| Quantity | Previously reported | This repository | File |
|---|---|---|---|
| m(4) = M_2(4) | ≥ 187205 (Dougherty, 1998, sequence not published) | **≥ 1310741** | `data/m4_1310741.txt` |
| Longest special sequence L | ≥ 187196 (Dougherty, 1998, sequence not published) | **≥ 245772** | `data/special_245772.txt` |
| n(3) | > A_7198(158386) (Friedman, Theorem 6.8) | **> A_9451(207952)** | via Lemma 4.3 with k = 9452 |

Also reproduced by exhaustive search: m(1) = 11, m(2) = 31, m(3) = 199, with 4, 16 and 32
longest sequences, as reported by Dougherty.

All of these are lower bounds found by heuristic search. Nothing here says how large m(4) or L
really are, and the searches that produced the m(4) bound had not finished when stopped.

### What the n(3) bound depends on

* The explicit special sequence in `data/special_245772.txt` (checked, see below).
* Friedman's Lemma 4.3, used as stated: for k ≥ 2, a special sequence of length ≥ 26k+8 gives
  n(3) > A_{k-1}(22k+8). Here 26·9452 + 8 = 245760 ≤ 245772 and 22·9452 + 8 = 207952.
  A is Friedman's hierarchy: A_1(n) = 2n, A_{k+1}(n) = A_k applied n times to 1.

The lemma itself is not re-proved or formalised here.

## Definitions

A sequence x[1..p] has **property \* from index n** if there are no n ≤ i < j ≤ p/2 such that
the block x[i..2i] is a subsequence of the block x[j..2j].

* n(k): longest length over k letters with property \* from index 1.
* M_k(n): the same from index n; m(k) = M_2(k). (x[1..n-1] lie in no block and are free.)
* A sequence a over {1,2,3} is **special** if it has property \* from index 1, has the form
  u 1 3^(n-1) with length 2n-2, and every block a[i..2i], 1 ≤ i ≤ n-1, contains a 1.

## The sequences

Run-length form (`0^5` means five 0s).

m(4) ≥ 1310741:

    0^4 1^2 0 1 0 1 0^5 1 0^2 1^14 0^5 1^13 0^27 1^12 0^68 1^11 0^149 1^10 0^310 1^9 0^631
    1^8 0^1272 1^7 0^2553 1^6 0^5114 1^5 0^10235 1^4 0^20476 1^3 0^40957 1^2 0^81918 1
    0^163840 1^327677 0 1^655358 0^4 1 0^22

After a short start, long runs of 0 roughly double, separated by the descending chain
1^14, 1^13, ..., 1; each separator buys one doubling. `data/m4_393219.txt` has the same shape
with a chain starting at 1^13.

Special sequence of length 245772 (n = 122887):

    1 2^2 1^2 3 1^8 3^5 1 3^7 1 3^6 1 3^22 1^2 3^4 1 3^53 1 3^5 1 3^113 1^2 3^3 1 3^234 1 3^4
    1 3^474 1^2 3^2 1 3^955 1 3^3 1 3^1915 1^2 3 1 3^3836 1 3^2 1 3^7676 1 3 1 3^15357 1^5
    3^30719 1^2 3^61442 1 3^122886

It comes from `data/m4_245779.txt` (an m(4) sequence of length 245779) by `scripts/to_special.py`:
a = 1 2 2 f(y4) f(y5) ..., where f sends the separator letter to 1 and the long-run letter to 3,
cut after the last separator. This needs y4 to be the separator letter, which is why the longer
m(4) sequences above do not convert: their long runs use the same letter as y4.

## Method

**Fast block test** (`src/core.h`). For a new block j let E(i) be the largest e such that
x[i..e] embeds in x[j..2j]. E is nondecreasing in i, and block i embeds in block j iff
E(i) ≥ 2i. Scanning i downward, if E(i) = e < 2i then any smaller i' with a bad embedding needs
2i' ≤ e, so the scan jumps to i' = ⌊e/2⌋. With run-indexed counts each E(i) costs O(runs), so
a block is tested in roughly logarithmically many steps instead of j.

**Stage search** (`src/macro.c`). Long sequences look like
start · c^r1 · S1 · c^r2 · S2 · ..., with r doubling and short separator words S_t.
The search tries every run length while the sequence is short, then switches to choosing
(separator, run length) pairs, longest separator first, accepting a separator only if the next
long run can grow by a factor 1.5. A small generic search finds an ending at each node.
With `SPECIAL=1` only structures that convert to special sequences are explored.

The search is a heuristic over a restricted space. Every sequence it writes has been checked
letter by letter as it was built, and the stored ones were re-checked independently.

## Verification

* `bin/lfs2 check 4 FILE`: fast test.
* `bin/lfs check 2 4 FILE`: brute force, every pair i < j with a plain greedy subsequence
  test (about 18 minutes for the longest file). Both accept all three m(4) files.
* Special sequence: form and the "every block has a 1" clause in Python, property \* from
  index 1 by the brute-force checker over three letters (`scripts/to_special.py`).
* `scripts/verify_special.py` is a pure-Python checker, practical only for short sequences.
* The fast and brute-force searches give identical node counts on m(1), m(2), m(3).

## Reproducing

    make                 # builds bin/
    make small           # m(1), m(2), m(3) exhaustively
    make verify          # re-checks the stored sequences (slow)

    bin/macro 4 20 32 13 1.5 300 1500                    # found 393219
    ASC=1 bin/macro 4 20 32 13 1.5 300 900               # found 1310741
    SPECIAL=1 bin/macro 4 20 32 13 1.5 300 1300          # found 245779 (special-compatible)
    python3 scripts/to_special.py data/m4_245779.txt out.txt 0

Arguments of `macro`: start index, length of the free phase, maximum shortening of a long run,
maximum separator length, growth factor, node budget per start, time limit in seconds.
Timings are for two cores of an ordinary cloud machine; results depend on the time limit.

Other programs: `rle` (restricted run-length search), `sweep` (ranks choices of the first
runs), `special` (direct three-letter search for special sequences).

## Open ends

* m(4) is probably larger than 1310741: the runs were stopped by a time limit.
* The special-compatible search with separators up to length 16 and a free phase of 20 letters
  finished without beating 245779; longer free phases have only been sampled.
* Deciding the true value of m(4) needs an argument, not a search of this kind.

## References

* H. Friedman, *Long Finite Sequences*, J. Combin. Theory Ser. A 95 (2001) 102–144.
  Preprint: https://bpb-us-w2.wpmucdn.com/u.osu.edu/dist/1/1952/files/2014/01/LongFinSeq98-2f0wmq3.pdf
* H. Friedman, FOM posting "21: Long Sequences/Update", October 1998 (reports Dougherty's results).
  https://cs.nyu.edu/pipermail/fom/1998-October/002339.html
