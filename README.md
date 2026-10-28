# stosegen

This project aims to implement the __*stochastic sequential generator* (stosegen)__ and its related tools.
Stosegen follows a particulR paradigma of randomness. Its detailled study can be found in the [reasearch paper](). A Major aspect of this randomness paradigma is the endness stipulation. In simple worlds, it can be decided in linear time, if a bit series is random or not. At this effect, we observe the distribution of bit runs of one state or of the other. For the sake of simplcity we describe these states runs in [0,1]*. A run is to be considered in the common stochastic use of the term, like done for the *run test*, which this *stosegen* is inspired from. 

In a bit series, each run of eiter (0....0) n times or (1....1) n times has a length. We compute the distribution of the runs lengths (regardless of the state) and decide about the randomness of the bit series according to its distribution. 
By ascending order of length, a random series has the following distribution pattern:

Length | Magnitude
-----: | :-------- 
   1   | 2^(n-3) 
   2   | 2^(n-4)
   3   | 2^(n-5)
   ... | ...
  n-2  | 1
  n-1  | 0
  n    | 1

for a suitable n

Any series fulfilling this distribution rule is said to be random. All other are not. 

Furthermore we proove an expression for 2^n:

$2^n = n + (n-2) + \sum_{n=1}^{N-3} (N-n) 2^n$

which is desmonstrated by nested complete induction proof. See the research paper for more details

For a series of length 2^N, the formula applies directly. If N is a summ of pairwise distinct  powers of $N = \sum_1^p a_n 2^p,\ p \in {0,1}$, we just check on the trivial way that the cummulation of the above formula applies for the overall runs distribution. This way any bit series of any ending length can be said to be random or not.

If we wish to construct a random bit series of given length, we just consider the length of this series to be $N = \sum_1^p a_n 2^p,\ p \in {0,1}$ and infer the requested cummulated distribution from the above formula. We then construct the series directly out of this requierment, up to ist binary inverse, which is equally random. Each permutation of the runs order of this series is of course random too. 

This fairly doesn't match the common understanding of randomness. But this is not the aim of this work.

## provided tools

# stosegen
This is the main tool and the project's name, it provides a shuffeled random bit series according to the provided input. In a certain sense it compresses the input entropy up to it maximum, 

## compile and install
itscripts are privided for compilation and installation that makes it simple to perform.

First change to project directory

```bash
cd ~/path/to/stosegen
```

For compilation run

```bash
./compile.sh
```

For installation run

```bash
 ./install.sh
 ```

For desinstallation run

```bash
./uninstall.sh
```