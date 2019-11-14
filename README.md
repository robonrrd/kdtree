To make the programs included in this directory, simply invoke make in this directory:
```
 make
```

There are two programs included here:
 * build_kdtree - Takes a file containing a set of vectors, creates
   a kd tree and serializes it to disk
   Example: ./build_kdtree  kdtree_data_2015-11-30/sample_data.csv

 * query_kdtree - Takes a serialized kd tree file, a data file (for
   verification) and a file of query points.
   This file instantiates a kdtree from the serialized file, finds the
   nearest neighbor in this three for each query point and compares
   these results with a brute-force ground truth.  The file will
   create an output file containing indices of the closest point.
   Example: ./query_kdtree  kdtree_data_2015-11-30/sample_data.csv.kdtree  kdtree_data_2015-11-30/sample_data.csv kdtree_data_2015-11-30/query_data.csv
   
