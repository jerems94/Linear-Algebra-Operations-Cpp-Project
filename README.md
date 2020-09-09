# Linear-Algebra-Operations-Project c++

This project in c++ is based on an homework I had at Technion which aims to implement operations between different types of matrices, vectors etc..

One of the challenging part of the code was to prevent user from writing in the zero part of a triangular matrix since my initial implementation of a matrix was a vector of vector.
Once you get in the vector number i you want to get to the j element, you lost the information you are currently in vector i (number of line) so you can't compare between
those 2 indexes.

I used the the advantages of c++ and OOP to overcome this problem. see in Matrix.cpp
