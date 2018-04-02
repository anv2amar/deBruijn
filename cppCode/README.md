
## Addition of Dynamic Edges to Debruijn graph



# Introduction:

De Bruijn graphs enable us to represent a sequence in terms of k-mers. The graph is basically a directed graph
with unique nodes representing k-tuples. These k-tuples are decided by starting with the first k nucleotides and
then moving forward with the next k overlapping nucleotides excluding the first nucleotide in the present tuple. 
Later, the tuples that are identical are joined and a directed graph is formed. The direction of the arrows are
such that the arrow originate from the k-mer with the last k-1 overlapping nucleotides and ending towards the k-mer
with the first k-1 overlapping nucleotides.

De Bruijn graphs are basically used to improve this memory situation by handling the assembly of reads having 
repetitive regions. This will improve the space efficiency.

There are various approaches to constructing Debruijn graph and **minimal perfect hashing** is one of them.

# Algorithm:
The dataset which we are about to use in our algorithm is a fastq file of **E-coli K-12 MG1655 illumina sequencing 
genome** which contains a sequence genome in GBs.
The entire algorithm is designed to work in the given steps:

**Construction of Debruijn Graph:**
  -  Each read is passed sequentially into the the DBGraph Construction which constructs a De Bruijn graph dynamically 
     adding new vertices and edges into the graph DB.
  -  For each sequence of size N split the sequence into array of size (n − k) each element with length k. Initialize 
     the Graph DB with matrix of size N * σ (σ being the size of the distinct alphabets of sequence)
  -  For each sequence reads, construct k-mers of the reads. Add consecutive k-mer1 and k-mer2 vertices if not present. 
     Then add an edge between those vertices using AddDynamicEdge algorithm.
     
**Adding Dynamice Edges:**
  -  Given a Debruijn Graph DB and 2 K-mers, we compute the hash of U and V for finding the indices of the IN and OUT
     matrices. 
  -  Initialize all the constants desiredsz, minht, maxht,midht. IN and OUT matrices are set to 1 for adding an edge
     between 2 K-mers.    
  -  Check if both the K-mers are in different components in the G in O(k log2 σ) time using Depth First Traversal:
     - If the SzU and SzV components are both < desiredsz then Merge both the components. 
     - If only one of the U or V component’s size is lesser than desiredsz then we check for height of the bigger 
       component. 
     - If the height of the bigger component is greater than the midht, then we need to break the bigger component 
       into two components as its height might cross maxht.
     - If the height of the bigger Component is < midht then we might need to just Merge with no break needed in the 
       bigger component as addition of new component of height < desiredsz doesn’t affect any violation to our conditions.
  -  We repeat this process till all the Edges are added into the G.

**Merge two connected components:**
  -  Given a graph G and 2 K-mers Ui and Vi (Indices of the K-mers), there is a need to merge U to V . This merge
     function’s 2nd and the 3rd parameters are always the smaller components . Within the Merge method, it changes the 
     direction of all the parent pointers in U’s component to point to the vertex U. This takes the time O(k log2 σ). 
  -  Once all the parent pointers are changed to U, mark U as the new root of U’s Component.
  -  Now, the bigger component might be needed to break into two parts in order to satisfy the height conditions. If the
     size of the bigger component is > midht, then the condition(< maxht) on height may be violated when we merge the 
     smaller component into the bigger one. 
  -  We traverse the desiredsz height in bigger component and break all the parent points at the level of height
     desiredsz-1.We choose one of the boundary as the new root of the U component. We sample the nodes in the broken 
     component and change the directions of parent pointers for finding the new root of the broken component.

**Performance evaluations:**
  We are using a small portion of code that would be dedicated to time and memory consumption of the entire project. To 
  check for the time utilized in this algortihm, we set a timer from the sequence read till the end of the merging components.
  
  However, to check for memory performance, we are checking out the process id or process name for our current code and 
  passing it as argument to the handle for Operating System in the code which gives the memory performance in bits. 
  Note: With different operating systems, there are different libraries to run code for memory performance.ex:- POSIX 
  library in case of Linux and windows.h library in Windows. 
  
  There are few third-party tools existing to monitor code performance like Valgrind.
  
# Steps to run the code:

- Our algorithms have been written in C++. We have used **CLion editor** to write and run the code. 
- It is really important to have a FASTQ file for above stated sequence which you get from 
  [European Nucleotide Archive](https://www.ebi.ac.uk/ena/data/view/ERX008638)
- Once the GB data is taken, check for your code in the Clion editor. Then click on 'Run' button to start your algorithm
  of generating dynamic edge on Debruijn graphs.
- If you get any errors or invalid references in the console, these might be due to some external libraries missing from
  your code. Make sure to mention the libraries in the TARGET_LINK_LIBRARIES of CMakeLists.txt which is a build configuration 
  file that comes along with any project you create in CLion.
  