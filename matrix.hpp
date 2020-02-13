/*
 * matrix.hpp
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
using std::fill;

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
  matrix (const size_t M, const size_t N)
    : m_matrix (new T[N * M]), m_rows (N), m_columns (M)
  {
  }
  
  // Size and value constructor.
  matrix (const size_t M, const size_t N, const T val)
    : m_matrix (new T[N * M]), m_rows (N), m_columns (M)
  {
    fill(this->begin(), this->end(), val);
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
    if (this != other)
    {
      swap (other.m_columns, m_columns);
      swap (other.m_rows, m_rows);
      swap (other.m_matrix, m_matrix);
    }
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
};

#endif