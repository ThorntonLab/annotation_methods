1#Intro

Taking Trinity's output and running the Augustus pipeline

#Reagents

[Augustus](http://bioinf.uni-greifswald.de/augustus/).  We use version 2.5.5.

[blat](http://hgwdev.cse.ucsc.edu/~kent/src/).  This can be a pain to install...

The "Trinity.fasta" file that comes out of Trinity.  See [here](https://github.com/ThorntonLab/annotation_methods) for how we do things.

##Compiling the tools (simple instructions)

###Augustus
```
wget http://bioinf.uni-greifswald.de/augustus/binaries/augustus.2.5.5.tar.gz
tar xzf augustus.2.5.5.tar.gz 
cd augustus.2.5.5/
```

The output of compiling is a bunch of executables in the "bin" directory.  The simplest thing is to copy/move them somewhere in your user's $PATH.

###blat
```
wget http://hgwdev.cse.ucsc.edu/~kent/src/blatSrc35.zip
unzip blatSrc35.zip
cd blatSrc
mkdir ~/bin
mkdir ~/bin/x86_64
MACHTYPE=x86_64 make
```

The output will be several binaries in ~/bin/x86_64.

__NOTE:__  the above compiling instructions are system-specific.  What one enters for MACHTYPE varies from system to system, and it is frustrating.  Usually, the "uname -a" command can give you some hints:

```
kevin@devlaeminck:~/tmp/blatSrc$ uname -a
Linux devlaeminck 3.2.0-58-generic #88-Ubuntu SMP Tue Dec 3 17:37:58 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux
```

You see the x86\_64 string repeated several times.  That is the MACHTYPE.

On my iMac:

```
uname -a
Darwin thornton01.bio.uci.edu 13.1.0 Darwin Kernel Version 13.1.0: Thu Jan 16 19:40:37 PST 2014; root:xnu-2422.90.20~2/RELEASE_X86_64 x86_64
```

And x86\_64 also appears to work there for MACHTYPE.  (Don't do this stuff on Macs--Linux only, please!)


#Running Augustus

This is our script, largely following instructions from Augustus' website:

```
#!sh

cd $1

export AUGUSTUS_CONFIG_PATH=/data/users/krthornt/src/augustus.2.5.5/config

INFILE=`ls $3/chrom*.fasta $3/therest.fasta | head -n $SGE_TASK_ID | tail -n 1`
INFILE_BASE=`basename $INFILE .fasta`
~/bin/x86_64-redhat-linux-gnu/blat -minIdentity=92 $INFILE $2/Trinity.fasta $INFILE_BASE.psl
perl /data/users/krthornt/src/augustus.2.5.5/scripts/blat2hints.pl --in=$INFILE_BASE.psl --out=$INFILE_BASE.hints.E.gff
/data/users/krthornt/src/augustus.2.5.5/bin/augustus --species=fly --hintsfile=$INFILE_BASE.hints.E.gff --extrinsicCfgFile=/data/users/krthornt/src/augustus.2.5.5/config/extrinsic/extrinsic.ME.cfg $INFILE --gff3=on --uniqueGeneId=true > $INFILE_BASE.gff3
```

Usage:
```
qsub -q krt,bio -hold_jid DYAKREFALL -t 1-7 ../augustus.sh `pwd` trinity_output ../references_for_augustus/dyak
```

The arguments are
<ol>
<li>path to working directory</li>
<li>path to folder containing Trinity.fasta</li>
<li>path to folder containing split up reference genome</li>
</ol>

We split up our reference genomes into the major arms + 1 FASTA file for "the rest".  The file names are chrom\*.fasta and therest.fasta.  This lets the blatting be a little faster and allows me to user OGE/SGE to use multiple cores for annotation.

This would all work fine, albeit slowly, on a single core.
