/*
 * matrixoperations.hpp
 * 
 * C++ matrix operation class.
 * 
 * Author: John Hynes
 * 
 * Notes:
 *   Personal project to apply some of what I've learned in my courses
 * this semester (Fall 2019). Emphasis on maintaining clean and expressive
 * code throughout the project.
*/

// Macro guard.
#ifndef MATRIXOPERATIONS_HPP
#define MATRIXOPERATIONS_HPP

// Include statements.

// Local includes.
#include "matrix.hpp"

// Using statements.
using std::swap;

// Row swap function.
// Performs a row swap on two rows of a matrix.
template<typename T>
void
rowswap (matrix<T>& m, const size_t r1, const size_t r2)
{
  // Iterate over every row element.
  for (size_t i = 0; i < m.columns (); ++i)
  {
    // Swap elements.
    swap (m (r1, i), m (r2, i));
  }
}

// Column swap function.
// Performs a column swap on two columns of a matrix.
template<typename T>
void
columnswap (matrix<T>& m, const size_t c1, const size_t c2)
{
  // Iterate over every row element.
  for (size_t i = 0; i < m.columns (); ++i)
  {
    // Swap elements.
    swap (m (i, c1), m (i, c2));
  }
}

// Simple transpose function.
// Performs a simple transpose on this matrix and returns the result.
// This changes the entire matrix, and thus is not pointer safe.
template<typename T>
matrix<T>&
transpose (matrix<T>& m)
{
  matrix<T> t (m.columns (), m.rows ());

  // Iterating over every element in the matrix.
  for (size_t i = 0; i < m.rows (); ++i)
  {
    for (size_t j = 0; j < m.columns (); ++j)
    {
      // Copying element from this matrix to the transpose matrix.
      t (j, i) = m (i, j);
    }
  }
  m = t;
  return m;
}

// Blocked transpose function.
// Transposes this a matrix with blocking for cache optimization and returns
// the result. More efficient than transpose() only for matrices with very
// large sizes, such as 1024x1024.
// This changes the entire matrix, and thus is not pointer safe.
template<typename T>
matrix<T>&
blocktranspose (matrix<T>& m)
{
  // Initializing transpose matrix.
  matrix<T> t (m.columns (), m.rows ());

  // Setting a block size.
  size_t block_size = 16;

  // Iterating through each block of the matrix.
  for (size_t row_block = 0; row_block < m.rows (); row_block += block_size)
  {
    for (size_t col_block = 0; col_block < m.columns ();
         col_block += block_size)
    {
      // Calculating block bounds to save operations.
      size_t row_block_end = row_block + block_size;
      size_t col_block_end = col_block + block_size;

      // Iterating over each element within the current block.
      for (size_t i = row_block; i < m.columns () && i < row_block_end; ++i)
      {
        for (size_t j = col_block; j < m.rows () && j < col_block_end; ++j)
        {
          // Copying element from this matrix to the transpose matrix.
          t (j, i) = m (i, j);
        }
      }
    }
  }
  // Setting source matrix to be the transpose matrix.
  m = t;
  return m;
}

#endif
