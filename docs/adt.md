# ADT(Abstract Data Type)
Simply put, an __ADT__ (Abstract Data Type) is more of a logical description, while a __Data Structure__ is concrete. Think of an __ADT__ as a picture of the data and the operations to manipulate and change it. A __Data Structure__ is the the real, concrete thing. It can be implemented and used within an algorithm.

- https://abrickshort.wordpress.com/2005/03/06/abstract-data-types-vs-data-structures/

First thing I got to know is.. the difference between the abstract data types (ADT) and datastructures. Earlier I thought they are same.

Before starting that, we need to be clear about the logical and implementation level of data. Let us take an example of a simple built-in data type integer. At the logic level, we application programmers know only about what are the operations an integer data type can perform, ie., addition, subtraction etc., But we are no way aware of how the data type is actually implemented. At the implementation level, it is about how the data type integer is implemented in the machine level, ie., it could either of binary-coded decimal, unsigned binary, sign-and-magnitude binary, One's complement and Two's complement notation.

Now.. for the understanding the ADT and data structure, we need to assume a higher level abstraction where we have the built-in types at the implementation level.

To put it simple, ADT is a logical description and data structure is concrete. ADT is the logical picture of the data and the operations to manipulate the component elements of the data. Data structure is the actual representation of the data during the implementation and the algorithms to manipulate the data elements. ADT is in the logical level and data structure is in the implementation level.

As you can see, ADT is implementation independent. For example, it only describes what a data type List consists (data) and what are the operations it can perform, but it has no information about how the List is actually implemented.

Whereas data structure is implementation dependent, as in the same example, it is about how the List implemented ie., using array or linked list. Ultimately, data structure is how we implement the data in an abstract data type.

This is something new of what I have learnt.