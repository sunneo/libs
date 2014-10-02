This is a b plus tree, designed to be simple and fast.
http://en.wikipedia.org/wiki/B%2B_tree. This code is released under
the ISC license.  Do not hesitate to contact me with questions,
suggestions, etc.

Credits:

Modification of b plus tree code by Amittai Aviram:
www.amittai.com/prose/bplustree.html Released under ISC License with
his permission.  Also consulted by Jan Jannink's deletion paper:
infolab.stanford.edu/~widom/cs346/jannink.pdf

Usage:

Usage is very simple, and should be completely explanatory from the
comments and declarations in bpt.h.  Behavior for duplicate key values
is currently undefined.

Internals:

The bpt structure holds a pointer to the root node and a pointer to
the comparison function.  The internal structure is unchanged from the
code by Amittai Aviram.  His description: Leaves and internal nodes
are both represented by bpt_node structures.  Leaves contain a maximum
of BPT_ORDER key-ptr pairs.  In an internal node, the first ptr refers
to lower nodes with keys less than the smallest key in the keys array.
Then, with indices i starting at 0, the pointer at i + 1 points to the
subtree with keys greater than or equal to the key in this node at
index i.  The nkeys field is used to keep track of the number of valid
keys.  In an internal node, the number of valid pointers is always
nkeys + 1.  In leaves, the number of valid pointers to data is always
nkeys.

Potential Improvements:

During deletion, when the number of key/ptrs in a node falls below the
minimum, the key/ptrs between the node and its neighbor are split
evenly, rather than the deficient node simply stealing one key/ptr.
The functions even_extra_left_steal and even_extra_right_steal
(currently in scratch.c) implement the latter strategy.  I have not
done sufficient testing to determine how much this affects delete and
lookup speeds at various order sizes.

Keys and pointers could be interleaved and binary search could be used
to search keys within a node.  I think the additional complexity of
these changes would offset theoretical gains.

During the insertion process, new nodes are not checked to ensure that
malloc has succeeded.  If an internal node is split, other lower nodes
have been altered, so returning immediately upon malloc failure would
leave the bpt in a broken state.  Therefore, the insertion process
will have to be significantly changed for allocation failure to be
handled gracefully.
