# VivadoHLSProjects

There are three projects in this repository.

These projects implement simple algorithms which can be used in Vivado_HLS to compile and synthesize.

HT project is a very simple sum of all regions above a threshold from Phase-1 upgrade, i.e., Stage-1/Layer-2, Stage-3a/Layer-2

ClusterFinder is the core of Phase-2 L1TCaloLayer1

ClusterTrackLinker is the core of Phase-2 L1TPFPLayer1

Testing on a Mac or Linux system:

```bash
 cd HT
 cc -o TestMakeHT *.c
 ./TestMakeHT
```

```bash
 cd ClusterFinder
 c++ -o testClusterFinder *.cc
 ./testClusterFinder
```

```bash
 cd ClusterTrackLinker
 c++ -o testClusterTrackLinker *.cc
 ./testClusterTrackLinker
```

Vivado HLS Instructions:

 Open a Vivado HLS from the project directory
 Create a new project
 Set your project up using the files
    Make sure you add the core algorithm and testbench
    Simulate and synthesize away