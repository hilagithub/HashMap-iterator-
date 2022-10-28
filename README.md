# HashMap-iterator-

/**
 * this header contains an implementation of hashmap including a
 * const iterator . the hashmap implemented as follows:
 * #_map- is a pointer to pointers "buckets"
 *   of elem as each element is a pair of key and
 *   value (template).
 *   each "bucket" is a linked list of elements
 *   if empty- points to nullptr
 * #size_buckets_- contains the sizes of each "bucket"
 * #size_- size of the hashmap (how many elements are in)
 * #capacity_-number of "buckets"
 *
 *
 * #const iterator-iterate over the elements using operators it++
 * and ++it also != and == operators * and ->
 *
 * */
