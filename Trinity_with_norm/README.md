#De novo assembly of transcriptome data using Trinity with digital read normalization

Computational workflows used in the Thornton lab for this task.

##Software versions used

<ol>
<li>perl 5.16.2</li>
<li>[Trinity](http://trinityrnaseq.sourceforge.net/) 20130216</li>
<li>bowtie 0.12.8</li>
<li>samtools 0.1.18</li>
<li>java 1.6</li>
</ol>

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

for i in *.READ1*.gz
do
zcat $i | awk 'BEGIN{P=1}{if(P==1||P==2){gsub(/^[@]/,">");print}; if(P==4)P=0; P++}'|perl -p -i -e 's/\s1:.+$/\/1/\
go' >> left.fa
done

Repeat the above for *.READ2*.gz and redirect the ouput into right.fa.

##Running Trinity

Step 1 is to run the digital normalization:
>perl -I $HOME/lib \`which normalize_by_kmer_coverage.pl`` --seqType fa --JM 100G --max_cov 30 --left left.fa --right right.fa --pairs_together --PARALLEL_STATS --JELLY_CPU $CORES

The output of the digital normalization makes funny file names, so I just grep for what I need and store the result as two variables:
>LEFTFILE=\`ls | grep left.fa | grep normalized\`

>RIGHTFILE=\`ls | grep right.fa | grep normalized\`

The final step is to run Trinity:

>Trinity.pl --seqType fa --bflyHeapSpaceInit 1G --bflyHeapSpaceMax 8G --JM 7G --left $LEFTFILE --right $RIGHTFILE --output trinity_output --min_contig_length 300 --CPU $CORES --inchworm_cpu $CORES --bflyCPU $CORES