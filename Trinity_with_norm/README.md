#De novo assembly of transcriptome data using Trinity with digital read normalization

Computational workflows used in the Thornton lab for this task.

##Software versions used
<ol>
<li>perl 5.16.2</li>
<li>[Trinity](http://trinityrnaseq.sourceforge.net/) 20130216</li>
<li>[bowtie](http://bowtie-bio.sourceforge.net/index.shtml) 0.12.8</li>
<li>[samtools](https://github.com/samtools) 0.1.18</li>
<li>java 1.6</li>
</ol>

###Notes:

Your perl installation must have [PerlIO::gzip](http://search.cpan.org/~nwclark/PerlIO-gzip-0.17/gzip.pm) available.

To install this for your user:

```shell
wget http://search.cpan.org/CPAN/authors/id/N/NW/NWCLARK/PerlIO-gzip-0.18.tar.gz
tar xzf PerlIO-gzip-0.18.tar.gz
cd PerlIO-gzip-0.18
perl Makefile.PL PREFIX=~/lib LIB=~/lib
make 
make install
```

Further instructions on installation of Perl modules may be found [here](http://www.perlmonks.org/index.pl?node_id=128077).

Now that the module is installed locally, you will likely need to say something like the following to get scripts using PerlIO::gzip working:

```shell
perl -I $HOME/lib script.pl
```
##Reagents needed

<ol>
<li>Paired-end RNA-seq data.  If there is > 1 pair of FASTQ files for a sample, please concatenate them into a single pair of FASTA files.</li>
<li>All of the software from the previous section available in your user's $PATH</li>
</ol>

###How to turn FASTQ files into FASTA files using the shell

(Note that Trinity is kind of insane and requires that the files be uncompressed. In a saner world, we'd also pipe the following through gzip...)

      zcat file.fastq.gz | awk 'BEGIN{P=1}{if(P==1||P==2){gsub(/^[@]/,">");print}; if(P==4)P=0; P++}'|perl -p -i -e 's/\s1:.+$/\/1/go' > file.fasta

###Catenating multiple FASTQ files into one big FASTA file:

On a Linux machine:

To iterate over all the "left" reads and make a single FASTA file:

```shell
   for i in *.READ1*.gz
   do
   zcat $i | awk 'BEGIN{P=1}{if(P==1||P==2){gsub(/^[@]/,">");print}; if(P==4)P=0; P++}'|perl -p -i -e 's/\s1:.+$/\/1/\go' >> left.fa
   done
```

Repeat the above for \*.READ2\*.gz and redirect the ouput into right.fa.

##Running Trinity

NOTE: for all of the commands below, $CORES represents how many CPU you want to use.  I am assuming that you have A LOT of RAM available to you.  All of our runs are on 512GB machines.

Step 1 is to run the digital normalization:

     perl -I $HOME/lib `which normalize_by_kmer_coverage.pl` --seqType fa --JM 100G --max_cov 30 --left left.fa --right right.fa --pairs_together --PARALLEL_STATS --JELLY_CPU $CORES

The output of the digital normalization makes funny file names, so I just grep for what I need and store the result as two variables:

    LEFTFILE=`ls | grep left.fa | grep normalized`

    RIGHTFILE=`ls | grep right.fa | grep normalized`

The final step is to run Trinity:

    Trinity.pl --seqType fa --bflyHeapSpaceInit 1G --bflyHeapSpaceMax 8G --JM 7G --left $LEFTFILE --right $RIGHTFILE --output trinity_output --min_contig_length 300 --CPU $CORES --inchworm_cpu $CORES --bflyCPU $CORES

##How long will this take

Many hours to a few days.  Without digital normalization (which you'd do by simply concatenating all FASTQ files and running Trinity on left.fa and right.fa), it could take weeks.

##What is the relevant output?

The assembled contigs will be in a file called trinity_output/Trinity.fasta

#Script for UCI cluster

Assuming the following:

<ol>
<li>You have a directory set up with all your paired-end FASTQ files from your RNA-seq experiment and nothing else.</li>
<li>The fastq files have names like sample-tissue-READ1.fastq.gz and sample-tissue-READ2.fastq.gz</li>
</ol>

The following command will run Trinity with digital read normalization on your samples on the UCI [HPC](http://hpc.oit.uci.edu/):

	find . |grep READ |  sort | xargs qsub -q bio -pe openmp 64 -N SPECIESNAME trinity.sh `pwd`

The trinity.sh script is found in this repository