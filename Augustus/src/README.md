#Additional utilities.

[libsequence](https://github.com/molpopgen/libsequence) is required for compilation.

##splitall

Usage:
```
zcat reference.fasta.gz | splitall
```

or:

```
cat reference.fasta | splitall
```

For every sequence in reference.fasta > 1MB, a single FASTA-format file is output.  Sequences < 1MB are collected in a file called "therest.fasta".  The larger chromosomes are put in chrom1.fasta, chrom2.fasta, etc.

This utility is useful for quickly splitting up reference genomes so that things like blat and Augustus runs may be naively parallelized on clusters.