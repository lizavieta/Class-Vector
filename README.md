# Description 

The Vector class is a dynamic array implementation in C++ designed to manage collections of elements with efficient memory usage and access patterns. This class is templated, allowing it to handle elements of any type, making it versatile for a wide range of applications. It supports various standard operations such as adding, removing, and accessing elements. Additionally, the Vector class includes iterator support to ensure compatibility with Standard Template Library (STL) algorithms.

# Core functionality 

The Vector class maintains three primary instance variables: sz, max_sz, and values. The sz variable keeps track of the current number of elements in the vector, while max_sz represents the maximum capacity of the vector, i.e., the total amount of memory allocated. The values pointer points to a dynamically allocated array where the elements are stored.

# Construction and Destruction 

The class offers multiple constructors for flexible initialization:

- The default constructor creates an empty vector.
- The copy constructor allows creating a new vector with the same content as an existing one.
- Parameterized constructors enable the creation of a vector with a specified size or from an initializer list of elements.
  
To manage resources properly, the destructor is implemented to release any dynamically allocated memory, preventing memory leaks.

# Member functions 

The Vector class includes several member functions to facilitate efficient and safe manipulation of the vector:

- size() and empty() functions provide information about the vector's current state.
- clear(), reserve(size_t n), and shrink_to_fit() functions manage the memory and elements, allowing for efficient use of resources.
- push_back(T x) and pop_back() functions add and remove elements from the end of the vector, respectively.
  
Overloaded operator[] functions provide both mutable and immutable access to elements by index, with bounds checking to ensure safety.

# Iterators 

To integrate seamlessly with STL algorithms, the Vector class includes nested Iterator and ConstIterator classes. These iterators support forward iteration over the vector's elements. Key functionalities provided by these iterators include dereferencing, pointer access, comparison operations, and advancement operations (both prefix and postfix increments).
