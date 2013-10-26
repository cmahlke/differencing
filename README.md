differencing
============

C implementation of the Karmarkar-Karp differencing algorithm with write-up

The Karmarkar-Karp heuristic begins by sorting the numbers in decreasing order.  At each step, the algorithm commits to placing the two largest numbers in different subsets, while differencing the decision about which subset each will go in.  The algorithm continues removing the two largest numbers, replacing by their difference in the sorted list, until there is only one number left, which is the value of the final subset difference.
To compute the actual partition, the algorithm builds a tree, with one node for each original number.  Each differencing operation adds an edge between nodes, to signify that the corresponding numbers must go in different subsets.  The resulting graph forms a spanning tree of the original nodes, which is then two-colored to determine the actual subsets, with al the numbers of one color going in one subset.  Since all the numbers must eventually be combined, and n ñ 1 edges be created, one for each differencing operation.  We can then color the nodes of the tree with two colors, so that no two adjacent nodes receive the same color, to get the final partition itself.  To two-color a tree, color one node arbitrarily, and then color any node adjacent to a colored node the opposite color.
The running time of this algorithm is O(nlogn) to sort the n numbers, O(nlogn) for the differencing, since each difference must be inserted into the sorted order, sing a heap for example, and finally O(n) to two-color the graph, for an overall time complexity of O(n log n).

