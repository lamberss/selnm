#ifndef LIBSELNM_MAPPED_LIST_HPP_
#define LIBSELNM_MAPPED_LIST_HPP_

#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace libselnm {

template <class T>
class MappedList {
 public:
  MappedList(const std::string &name, const std::string &description)
      : max_id_width_(0), name_(name), description_(description){};

  void add(const std::string &id, const T &&value) {
    if (indices_.count(id) > 0) {
      std::stringstream msg;
      msg << "Cannot add item with ID=\"" << id << "\" to \"" << name_
          << "\" since the ID is already in use.";
      throw std::runtime_error(msg.str());
    }
    indices_.emplace(id, data_.size());
    data_.emplace_back(value);
    ids_.emplace_back(id);
    max_id_width_ = std::max(max_id_width_, id.length());
  }

  const std::string &id(std::size_t i) const { return ids_[i]; };
  const std::string &description() const { return description_; };
  const std::string &name() const { return name_; };
  std::size_t size() const { return data_.size(); }

  T &at(const std::size_t &i) { return data_.at(i); };
  const T &at(const std::size_t &i) const { return data_.at(i); };
  T &at(const std::string &id) { return data_.at(indices_.at(id)); };
  const T &at(const std::string &id) const {
    return data_.at(indices_.at(id));
  };

  T &operator[](const std::size_t &i) { return data_[i]; };
  const T &operator[](const std::size_t &i) const { return data_[i]; };
  T &operator[](const std::string &id) { return data_[indices_.at(id)]; };
  const T &operator[](const std::string &id) const {
    return data_[indices_.at(id)];
  };

  friend std::ostream &operator<<(std::ostream &os, const MappedList<T> &list) {
    // Compute widths
    std::vector<std::string> header({"Index", "ID", list.description()});
    std::stringstream largest_index;
    largest_index << list.size() - 1;
    std::size_t index_width =
        std::max(header[0].length(), largest_index.str().length());
    std::size_t id_width = std::max(header[1].length(), list.max_id_width_);

    // Write header
    os << std::setw(index_width) << header[0] << "  " << std::setw(id_width)
       << header[1] << "  " << header[2] << std::endl;

    // Write data
    for (std::size_t i = 0; i < list.size(); ++i) {
      os << std::setw(index_width) << i << "  " << std::setw(id_width)
         << list.id(i) << "  " << list[i] << std::endl;
    }
    return os;
  };

 private:
  std::vector<T> data_;
  const std::string description_;
  std::vector<std::string> ids_;
  std::map<std::string, T> indices_;
  std::size_t max_id_width_;
  const std::string name_;
};

};  // namespace libselnm

#endif  // LIBSELNM_MAPPED_LIST_HPP_