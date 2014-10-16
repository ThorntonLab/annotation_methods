#Annotation methods

This repo contains scripts, etc., that we use for RNA-seq based genome annotations.  The repo also serves as supplementary material for this paper:

* Rogers, R. L., L. Shao, J. Sanjak, P. Andolfatto and K. R. Thornton (Accepted) Sex-biased expression in the Drosophila melanogaster group.  G3: Genes, Genomes, Genetics. [Abstract](http://www.g3journal.org/content/early/2014/10/01/g3.114.013532.abstract)

The Trinity_with_norm folder contains the script for how we ran Trinity in the above paper.

The Augustus folder contains how we ran blat + Augustus to generate "hint-guided" annotations for each genome.  The files also include some editorializing on installing blat, which can be quite tricky.

##Future-proofing

In the case of future modification of these scripts, I have released version 0.1.0 of this repository to correspond to the command lines used in Rogers _et al._
