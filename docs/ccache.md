# ccache

```
$ ccache -s
Summary:
  Hits:             3319 / 11646 (28.50 %)
    Direct:         2552 / 12061 (21.16 %)
    Preprocessed:    767 /  9447 (8.12 %)
  Misses:           8327
    Direct:         9509
    Preprocessed:   8680
  Uncacheable:      7658
Primary storage:
  Hits:             7847 / 24060 (32.61 %)
  Misses:          16213
  Cache size (GB):  4.58 /  5.00 (91.68 %)
  Cleanups:          195

Use the -v/--verbose option for more details.
```

- Query key:
```
-s, --show-stats           show summary of configuration and statistics counters in human-readable format

-x, --show-compression     show compression statistics

-p, --show-config          show current configuration options in human-readable format
    --show-log-stats       print statistics counters from the stats log in human-readable format
```

- Set key:
```
    -F, --max-files NUM        set maximum number of files in cache to NUM (use 0 for no limit)

    -M, --max-size SIZE        set maximum size of cache to SIZE (use 0 for no limit); available suffixes: k, M, G, T (decimal) and Ki, Mi, Gi, Ti (binary); default suffix: G
```

- Example: set cache size limit to 32 GB
```
$ ccache -M 32
Set cache size limit to 32.0 GB
```