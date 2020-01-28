/*
 * Matrix.hpp
 * 
 * C++ matrix class.
 * 
 * Author: John Hynes
 * 
 * Notes:
 *   Personal project to apply some of what I've learned in my courses
 * this semester (Fall 2019). Emphasis on maintaining clean and expressive
 * code throughout the project.
*/

// Macro guard
#ifndef MATRIX_HPP
#define MATRIX_HPP

// System includes
#include <algorithm>
#include <utility>

// Using statements
using std::copy_n;
using std::exchange;
using std::swap;

// Matrix implementation
template<typename T>
struct matrix
{
private:
  /**************************************************************************/
  // Member values.
  /**************************************************************************/

  // Stores the elements of this matrix.
  T* m_matrix;
  // Stores the number of rows this matrix has.
  size_t m_rows;
  // Stores the number of columns this matrix has.
  size_t m_columns;

  // Public functions.
public:
  /**************************************************************************/
  // Constructors and Destructors.
  /**************************************************************************/

  // Size constructor.
  matrix (const size_t N, const size_t M)
    : m_matrix (new T[N * M]), m_rows (N), m_columns (M)
  {
  }

  // Copy constructor.
  matrix (const matrix& other)
    : m_matrix (new T[other.rows () * other.columns ()])
    , m_rows (other.rows ())
    , m_columns (other.columns ())
  {
    copy_n (other.m_matrix, rows () * columns (), m_matrix);
  }

  // Move constructor.
  matrix (matrix&& other)
    : m_matrix (exchange (other.m_matrix, nullptr))
    , m_rows (exchange (other.m_rows, 0))
    , m_columns (exchange (other.m_columns, 0))
  {
  }

  // Copy assignment operator.
  matrix&
  operator= (const matrix& other)
  {
    if (&other != this)
    {
      matrix copy (other);
      swap (copy.m_columns, m_columns);
      swap (copy.m_rows, m_rows);
      swap (copy.m_matrix, m_matrix);
    }
    return *this;
  }

  // Move assignment operator.
  matrix&
  operator= (matrix&& other)
  {
    swap (other.m_columns, m_columns);
    swap (other.m_rows, m_rows);
    swap (other.m_matrix, m_matrix);
    return *this;
  }

  // Destructor.
  ~matrix ()
  {
    delete[] m_matrix;
  }

  /**************************************************************************/
  // Accessors.
  /**************************************************************************/

  // Index accessor.
  // Returns a reference the element of m_matrix located at (i,j).
  T&
  operator() (const size_t i, const size_t j)
  {
    return m_matrix[i * m_columns + j];
  }

  // Const index accessor.
  // Returns a const reference to the element of m_matrix located at (i,j).
  const T&
  operator() (const size_t i, const size_t j) const
  {
    return m_matrix[i * m_columns + j];
  }

  // Row accessor.
  // Returns the number of rows in this matrix.
  size_t
  rows ()
  {
    return m_rows;
  }

  // Const row accessor.
  // Returns the number of rows in this matrix.
  const size_t
  rows () const
  {
    return m_rows;
  }

  // Column accessor.
  // Returns the number of columns in this matrix.
  size_t
  columns ()
  {
    return m_columns;
  }

  // Const column accessor.
  // Returns the number of columns in this matrix.
  const size_t
  columns () const
  {
    return m_columns;
  }

  /**************************************************************************/
  // Iterator functions.
  /**************************************************************************/

  // Begin function.
  // Returns an iterator pointing to the beginning of the matrix.
  T*
  begin ()
  {
    return m_matrix;
  }

  // Const begin function.
  // Returns a const iterator pointing to the beginning of the matrix.
  const T*
  begin () const
  {
    return m_matrix;
  }

  // End function.
  // Returns an iterator pointing to the end of the matrix.
  T*
  end ()
  {
    return m_matrix + (m_rows * m_columns);
  }

  // Const end function.
  // Returns a const iterator pointing to the end of the matrix.
  const T*
  end () const
  {
    return m_matrix + (m_rows * m_columns);
  }

  /**************************************************************************/
  // Member functions.
  /**************************************************************************/

  // Copy function.
  // Returns a copy of this matrix.
  matrix<T>&
  copy ()
  {
    // Return a new matrix allocated using the copy constructor.
    return matrix<T> (*this);
  }

  // Row swap function.
  // Swaps two rows of the matrix.
  void
  rowSwap (const size_t row1, const size_t row2)
  {
    size_t row1_index = row1 * m_columns;
    size_t row2_index = row2 * m_columns;

    // Iterate over every element of row1 and row2.
    for (size_t i = 0; i < m_columns; ++i)
    {
      // Swap elements of row1 and row2.
      swap (m_matrix[row1_index], m_matrix[row2_index]);

      // Iterate to the next element.
      ++row2_index;
      ++row1_index;
    }
  }

  // Transpose function.
  // Returns the transpose of this matrix.
  matrix<T>
  transpose ()
  {
    // Initializing transpose matrix.
    matrix<T> trans_matrix (m_columns, m_rows);

    // If the size of the matrix is small, use a transpose algorithm
    //   with a low asymtotic complexity
    if (m_rows < 32 && m_columns < 32)
    {
      simpleTranspose (trans_matrix);
    }

    // Otherwise, if this matrix is large, use a more optimized algorithm.
    else
    {
      blockedTranspose (trans_matrix);
    }

    // Returning transpose matrix.
    return trans_matrix;
  }

  // Private helper functions.
private:
  // Blocked transpose function.
  // Transposes this matrix with blocking for cache optimization and returns
  //   the result.
  void
  blockedTranspose (matrix<T>& trans_matrix)
  {
    // Initializing variables for block bounds.
    size_t col_block, row_block, col_block_end, row_block_end, i, j;
    size_t block_size = 16;

    // Iterating through each block of the matrix.
    for (row_block = 0; row_block < m_rows; row_block += block_size)
    {
      for (col_block = 0; col_block < m_columns; col_block += block_size)
      {
        // Calculating block bounds to save operations.
        row_block_end = row_block + block_size;
        col_block_end = col_block + block_size;

        // Iterating over each element within the current block.
        for (i = row_block; i < m_columns && i < row_block_end; ++i)
        {
          for (j = col_block; j < m_rows && j < col_block_end; ++j)
          {
            // Copying element from this matrix to the transpose matrix.
            trans_matrix (j, i) = (*this) (i, j);
          }
        }
      }
    }
  }

  // Simple transpose function.
  // Performs a simple transpose on this matrix and returns the result.
  void
  simpleTranspose (matrix<T>& trans_matrix)
  {
    size_t i, j;

    // Iterating over every element in the matrix.
    for (i = 0; i < m_rows; ++i)
    {
      for (j = 0; j < m_columns; ++j)
      {
        // Copying element from this matrix to the transpose matrix.
        trans_matrix (j, i) = (*this) (i, j);
      }
    }
  }
};

#endif
