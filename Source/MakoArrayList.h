#pragma once

#include "MakoCommon.h"
#include <vector>
using namespace std;

MAKO_BEGIN_NAMESPACE


//! ArrayLists are a kind of sequence containers. As such, their
//! elements are ordered following a strict linear sequence.
//!
//! ArrayList containers are implemented as dynamic arrays; Just 
//! as regular arrays, ArrayList containers have their elements stored
//! in contiguous storage locations, which means that their elements
//! can be accessed not only using iterators but also using offsets
//! on regular pointers to elements.
//!
//! But unlike regular arrays, storage in ArrayLists is handled automatically,
//! allowing it to be expanded and contracted as needed.
//!
//! ArrayLists are good at:
//!
//! \li Accessing individual elements by their position index (constant time).
//! \li Iterating over the elements in any order (linear time).
//! \li Add and remove elements from its end (constant amortized time).
//! \li Compared to arrays, they provide almost the same performance for these
//! tasks, plus they have the ability to be easily resized. Although, they usually 
//! consume more memory than arrays when their capacity is handled automatically
//! (this is in order to accomodate for extra storage space for future growth).
//!
//! Compared to the other base standard sequence containers (LinkedLists), 
//! ArrayLists are generally the most efficient in time for accessing elements and 
//! to add or remove elements from the end of the sequence. For operations that 
//! involve inserting or removing elements at positions other than the end, they 
//! perform worse than deques and lists, and have less consistent iterators and 
//! references than lists.
//!
//! Internally, ArrayLists -like all containers- have a size, which represents the
//! amount of elements contained in the ArrayList. But ArrayLists, also have a capacity, 
//! which determines the amount of storage space they have allocated, and which 
//! can be either equal or greater than the actual size. The extra amount of storage
//! allocated is not used, but is reserved for the ArrayList to be used in the case 
//! it grows. This way, the ArrayList does not have to reallocate storage on each
//! occasion it grows, but only when this extra space is exhausted and a new element 
//! is inserted (which should only happen in logarithmic frequence in relation with 
//! its size).
//!
//! Reallocations may be a costly operation in terms of performance, since they 
//! generally involve the entire storage space used by the ArrayList to be copied to 
//! a new location. Therefore, whenever large increases in size are planned for a 
//! ArrayList, it is recommended to explicitly indicate a capacity for the ArrayList using 
//! member function ArrayList::reserve.
//!
//! \note
//! This container, as well as other Mako containers, are identical to their inherited stl classes.
//! To find documentation on methods and more, look up these stl classes.
//! When C++0x is released, this will be a typedef instead
//!
template <typename T>
class ArrayList : public std::vector<T>
{
public:
	//! Empty deconstructor
	MAKO_INLINE ArrayList() {}

	//! Constructor constructs with a size
	//! \param[in] size The initial size of the array
	MAKO_INLINE ArrayList(UInt32 size) : std::vector<T>(size) {}

	//! Empy deconstructor
	MAKO_INLINE ~ArrayList() {}
};

template <typename T>
MAKO_INLINE ArrayList<T> Construct1ItemArrayList(const T& firstItem)
{ ArrayList<T> al(1); al[0] = firstItem; return al; }


MAKO_END_NAMESPACE