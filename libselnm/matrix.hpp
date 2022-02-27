#ifndef LIBSELNM_MATRIX_HPP_
#define LIBSELNM_MATRIX_HPP_

#include <cmath>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace libselnm {

template <class T>
class Matrix {
 public:
  Matrix<T>(const std::size_t &r, const std::size_t &c, const T &v)
      : num_rows_(r), num_cols_(c), data_(r * c, v){};
  Matrix<T>(const std::size_t &r, const std::size_t &c,
            const std::initializer_list<T> &v)
      : num_rows_(r), num_cols_(c), data_(r * c) {
    data_.assign(v);
  };

  virtual std::size_t columns() const { return num_cols_; };
  virtual std::size_t rows() const { return num_rows_; };

  virtual T &at(const std::size_t &i) { return data_.at(i); };
  virtual const T &at(const std::size_t &i) const { return data_.at(i); };

  virtual T &at(const std::size_t &i, const std::size_t &j) {
    if (i < 0 || i >= rows() || j < 0 || j >= columns()) {
      std::stringstream msg;
      msg << "Out of bounds with indices (" << i << "," << j << "), max("
          << rows() << "," << columns() << ")";
      throw std::out_of_range(msg.str());
    }
    return data_.at(Flatten(i, j));
  };
  virtual const T &at(const std::size_t &i, const std::size_t &j) const {
    if (i < 0 || i >= rows() || j < 0 || j >= columns()) {
      std::stringstream msg;
      msg << "Out of bounds with indices (" << i << "," << j << "), max("
          << rows() << "," << columns() << ")";
      throw std::out_of_range(msg.str());
    }
    return data_.at(Flatten(i, j));
  };

  virtual T &operator[](const std::size_t &i) { return data_[i]; };
  virtual const T &operator[](const std::size_t &i) const { return data_[i]; };

  virtual T &operator()(const std::size_t &i, const std::size_t &j) {
    return data_[Flatten(i, j)];
  };
  virtual const T &operator()(const std::size_t &i,
                              const std::size_t &j) const {
    return data_[Flatten(i, j)];
  };

  friend bool equal(const Matrix<T> &lhs, const Matrix<T> &rhs,
                    const T &tolerance = static_cast<T>(-1)) {
    bool result = lhs.columns() == rhs.columns() && lhs.rows() == rhs.rows();
    if (result) {
      // Only check element-by-element if sizes are the same
      for (std::size_t i = 0u; i < lhs.rows(); ++i) {
        for (std::size_t j = 0u; j < lhs.columns(); ++j) {
          if (tolerance <= static_cast<T>(0)) {
            result = result && lhs(i, j) == rhs(i, j);
          } else {
            result = result && std::abs(lhs(i, j) - rhs(i, j)) <= tolerance;
          }
          if (!result) return result;
        }
      }
    }
    return result;
  };
  friend bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    return equal(lhs, rhs);
  };
  friend bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    return !(lhs == rhs);
  };

  Matrix<T> &operator+=(const T &rhs) {
    for (auto &&d : data_) d += rhs;
    return *this;
  }
  friend Matrix<T> operator+(Matrix<T> lhs, const T &rhs) {
    lhs += rhs;
    return lhs;
  }

  Matrix<T> &operator-=(const T &rhs) {
    for (auto &&d : data_) d -= rhs;
    return *this;
  }
  friend Matrix<T> operator-(Matrix<T> lhs, const T &rhs) {
    lhs -= rhs;
    return lhs;
  }

  Matrix<T> &operator*=(const T &rhs) {
    for (auto &&d : data_) d *= rhs;
    return *this;
  }
  friend Matrix<T> operator*(Matrix<T> lhs, const T &rhs) {
    lhs *= rhs;
    return lhs;
  }

  Matrix<T> &operator/=(const T &rhs) {
    for (auto &&d : data_) d /= rhs;
    return *this;
  }
  friend Matrix<T> operator/(Matrix<T> lhs, const T &rhs) {
    lhs /= rhs;
    return lhs;
  }

  Matrix<T> &operator+=(const Matrix<T> &rhs) {
    if (rows() != rhs.rows() || columns() != rhs.columns()) {
      std::stringstream msg;
      msg << "Cannot add matrices with sizes (" << rows() << "," << columns()
          << ") and (" << rhs.rows() << "," << rhs.columns() << ")";
      throw std::invalid_argument(msg.str());
    }
    for (std::size_t i = 0; i < rows(); ++i) {
      for (std::size_t j = 0; j < columns(); ++j) {
        data_[Flatten(i, j)] += rhs(i, j);
      }
    }
    return *this;
  }
  friend Matrix<T> operator+(Matrix<T> lhs, const Matrix<T> &rhs) {
    lhs += rhs;
    return lhs;
  }

  friend Matrix<T> operator*(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    std::size_t m = lhs.rows();
    std::size_t n = lhs.columns();
    std::size_t p = rhs.rows();
    std::size_t q = rhs.columns();
    if (n != p) {
      std::stringstream msg;
      msg << "Cannot multiply matrices with sizes (" << m << "," << n
          << ") and (" << p << "," << q << ")";
      throw std::invalid_argument(msg.str());
    }
    Matrix<T> result(m, q, 0.0);
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < q; ++j) {
        for (std::size_t k = 0; k < p; ++k) {
          result(i, j) += lhs(i, k) * rhs(k, j);
        }
      }
    }
    return result;
  }

 private:
  inline std::size_t Flatten(const std::size_t &i, const std::size_t &j) const {
    return i * num_cols_ + j;
  };

  const std::size_t num_rows_;
  const std::size_t num_cols_;
  std::vector<T> data_;
};

};  // namespace libselnm

#endif  // LIBSELNM_MATRIX_HPP_