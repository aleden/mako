#pragma once

#include "MakoCommon.h"
#include <list>
using namespace std;

MAKO_BEGIN_NAMESPACE


//! LinkedLists are a kind of sequence containers. As such, 
//! their elements are ordered following a linear sequence.
//!
//! LinkedList containers are implemented as doubly-linked lists;
//! Doubly linked lists can store each of the elements they contain 
//! in different and unrelated storage locations. The ordering is 
//! kept by the association to each element of a link to the element 
//! preceding it and a link to the element following it.
//!
//! This provides the following advantages to LinkedList containers:
//!
//! \li Efficient insertion and removal of elements anywhere in the 
//! container (constant time).
//! \li Efficient moving elements and block of elements within the 
//! container or even between different containers (constant time).
//! \li Iterating over the elements in forward or reverse order (linear time).
//! \li Compared to other base standard sequence containers (ArrayLists),
//! lists perform generally better in inserting, extracting
//! and moving elements in any position within the container, and therefore
//! also in algorithms that make intensive use of these, like sorting algorithms.
//!
//! The main drawback of lists compared to these other sequence containers is
//! that they lack direct access to the elements by their position; For example,
//! to access the sixth element in a list one has to iterate from a known
//! position (like the beginning or the end) to that position, which takes
//! linear time in the distance between these. They also consume some extra memory
//! to keep the linking information associated to each element (which may
//! be an important factor for large lists of small-sized elements).
//!
//! Storage is handled automatically by the class, allowing lists to be
//! expanded and contracted as needed.
//! 
//! \note
//! This container, as well as other Mako containers, are identical to their inherited stl classes.
//! To find documentation on methods and more, look up these stl classes.
//! When C++0x is released, this will be a typedef instead
template <typename T>
class LinkedList : public std::list<T> {};


MAKO_END_NAMESPACE
