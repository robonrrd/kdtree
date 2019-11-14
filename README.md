# KD Tree #

## What is this? ##
This is a header-only implementation of an arbitrary-dimension kd-tree which supports offline construction, nearest neighbor queries, and (de)serialization of the tree. 

## Building ##
To make the programs included in this directory, simply invoke make in this directory:
```
 make
```

## Contents ##

There are two programs included here:
 * build_kdtree - Takes a file containing a comma-seperated set of vectors, creates
   a kd tree and serializes it to disk:
   ```
   ./build_kdtree  kdtree_sample_data.csv
   ```

 * query_kdtree - Takes a serialized kd tree file, a data file (for
   verification) and a file of query points.
   This file instantiates a kdtree from the serialized file, finds the
   nearest neighbor in this three for each query point and compares
   these results with a brute-force ground truth.  
   
   The file will create an output file containing indices of the closest point.
   ```
   ./query_kdtree  kdtree_sample_data.kdtree  kdtree_sample_data.csv kdtree_query_data.csv
   ```

## Analysis ##

I implemented this kd tree using templated classes and std containers in order to have maximum
flexibility as well as clean resource handling. For example, all pointers are wrapped in unique_ptr so
the implementation doesn’t have to be concerned with memory management.
I used ```std::nth_element``` function in order to find median elements and weakly order the other
points, because this is an O(n) operation, rather than an O(n logn) which a full sort would require.
I also heavily used the move-constructor in my function’s return values, which would make this code
suboptimal in C++98, but is more efficient than passing references to constructed return values in C++11
onward.

## Future Work ##
No piece of software is ever truly finished. Given more time and interest, several improvements to this code could be made.

The biggest performance improvement could come from not representing the kd tree as a tree to be
navigated with pointers, but instead as elements in an array. That is, instead of a node referring to its
left and right children via pointers to new nodes, they would instead be accessed by indices (```2*node_index+1``` and ```2*node_index+2``` respectively) into an array that stored the node data (the
separating axis and point). This would save space (no pointers would have to be stored) and increase
speed by increasing cache coherency. The disadvantages of this approach are twofold:
1) The array must be allocated and live in contiguous memory.
2) Memory would be wasted if the tree wasn’t balanced.

Another improvement could be made to the tree depth (and thus its query efficiency) by more intelligently
selecting separating axes during construction. Currently, we naively select the axis of the separating
plane at each node, by starting at the 0 th axis at the top level and incrementing (modulo dimension) as
we add depth. Ideally, we would determine the smallest dimension of the bounding box of the
construction points, and use that axis as the first separating plane. (Why smallest? We wish to divide
points by the largest axis, not along the largest axis). If we wished to trade off even more construction
time for a potentially even-shallower tree, we could do this same bounding-box-and-selection when
every node is constructed, rather than just once at the beginning.

We could also trade off a small amount of performance for space by having more than one point in the
leaf nodes. That is, rather than creating a leaf node for each point, our leaves would contain a vector of
points, which would then be linearly searched for the best match.

The serialization implementation is, frankly, weak. While simple to implement and easy to debug (which were my goals), it could be
made much more compact, faster to write, and faster to read if I used a binary format.

Similarly, I strongly enforce (via asserts and program termination) the correctness of both the building
and querying datasets. It would be fairly simple to sanitize our data by automatically expanding all input points, for example, to be the dimension of the largest dimensioned point. (However, would that be
correct? It depends..) Dealing with an incorrectly dimensioned query point would require a similar
design decision (do we truncate/expand the query or reject it?). 

In the same vein of robustness, I have try/catch blocks around the kd tree construction code and nearest
neighbor query code, since those are the locations most likely to run into issues (resource allocation
occurs there, as well as pointer dereferencing). However, a more finely grained set of exceptions
would be better.