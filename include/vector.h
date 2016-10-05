/*
 * Copyright 2016 Waizung Taam
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ==== C++ STL vector implementation ====
 *
 * To show every detail of the implementation, a minimal set of helper
 * functions and classes in namespace std are defined directly in this file,  
 * therefore no standard library is needed. Note that these helper 
 * functions are simplified, thus they will be less efficient than
 * the standard implementations, and some special cases are not covered.
 *
 * Some C++11 features are not covered in this implementation, however,
 * this code requires a good support for C++11 to compile.
 */

/* - 2016-10-04 
 * - waizungtaam@gmail.com
 *
 * - size_t, ptrdiff_t
 * - swap()
 * - move()
 * - copy(), copy_backward()
 * - fill(), fill_n()
 * - uninitialized_copy(), uninitialized_fill(), uninitialized_fill_n()
 * - is_integral<Tp>
 * - new_allocator<Tp>
 * 
 * - vector<Tp, Allocator>
 *   - public
 *     - ctors, op=, dtor
 *     - get_allocator()
 *     - assign()
 *     - accessors
 *     - iterators
 *     - capacity
 *     - modifiers
 *   - protected
 *     - initialize_aux()
 *     - range_initialize()
 *     - allocate_and_copy()
 *     - destroy_and_deallocate()
 *     - assign_aux()
 *     - range_check()
 *     - insert_aux()
 *     - range_insert()
 *     - data members
 * - comparisons of vectors
 */
#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "iterator.h"

namespace mystl {

typedef unsigned long size_t;
typedef long ptrdiff_t;

template <typename Tp>
void swap(Tp& x, Tp& y) {
  Tp tmp = x;
   x = y;
   y = tmp;
}

template <typename Tp>
struct remove_reference { typedef Tp type; };
template <typename Tp>
struct remove_reference<Tp&> { typedef Tp type; };
template <typename Tp>
struct remove_reference<Tp&&> { typedef Tp type; };
template <typename Tp>
typename remove_reference<Tp>::type&& move(Tp&& x) noexcept {
  return static_cast<typename remove_reference<Tp>::type&&>(x);
}

/* Note: Simple Version */
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
                           OutputIterator result) {
  for (; first != last; ++first, ++result) {
    *result = *first;
  }
  return result;
}
/* Note: Simple Version */
template <typename BidirectionalIterator_1, typename BidirectionalIterator_2>
inline BidirectionalIterator_2 copy_backward(
  BidirectionalIterator_1 first, BidirectionalIterator_1 last,
  BidirectionalIterator_2 result) {
  while (first != last) {
    *(--result) = *(--last);
  }
  return result;
}

/* Note: Simple Version */
template <typename ForwardIterator, typename Tp>
void fill(ForwardIterator first, ForwardIterator last, const Tp& value) {
  for (; first != last; ++first) {
    *first = value;
  }
}
/* Note: Simple Version */
template <typename OutputIterator, typename Size, typename Tp>
OutputIterator fill_n(OutputIterator first, Size n, const Tp& value) {
  for (; n > 0; --n, ++first) {
    *first = value;
  }
  return first;
}

/* Note: Simple Version */
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                   ForwardIterator result) {
  int i = 0;
  for (i = 0; first != last; ++first, ++i) {
    new(result + i) typename 
      iterator_traits<InputIterator>::value_type(*first);
  }
  return result + i;
}
/* Note: Simple Version */
template <typename ForwardIterator, typename Tp>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, 
                        const Tp& value) {
  for (; first != last; ++first) {
    new(first) typename iterator_traits<ForwardIterator>::value_type(value);
  }
}
/* Note: Simple Version */
template <typename ForwardIterator, typename Size, typename Tp>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, 
                                     const Tp& value) {
  int i = 0;
  for (i = 0; i != n; ++i) {
    new((Tp*)(first + i)) typename 
      iterator_traits<ForwardIterator>::value_type(value);
  }
  return first + i;
}


/* Note: Variant */
struct true_type {};
struct false_type {};
template <typename Tp> struct is_integral { 
  typedef false_type type; static const bool value = false; };
template <> struct is_integral<bool> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<char> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<signed char> {
  typedef true_type type; static const bool value = true; };
template <> struct is_integral<unsigned char> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<wchar_t> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<short> {
  typedef true_type type; static const bool value = true; };
template <> struct is_integral<unsigned short> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<int> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<unsigned int> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<long> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<unsigned long> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<long long> {
  typedef true_type type;  static const bool value = true; };
template <> struct is_integral<unsigned long long> { 
  typedef true_type type;  static const bool value = true; };

template <typename Tp>
class new_allocator {
public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Tp value_type;
  typedef Tp* pointer;
  typedef const Tp* const_pointer;
  typedef Tp& reference;
  typedef const Tp& const_reference;
  
  template <typename Tp1>
  struct rebind { typedef new_allocator<Tp1> other; };

  new_allocator() {}
  new_allocator(const new_allocator& other) {}
  template <typename Tp1>
  new_allocator(const new_allocator<Tp1>& other) {}
  ~new_allocator() {}

  size_type max_size() const { return size_type(-1) / sizeof(Tp); }

  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const { return &x; }

  pointer allocate(size_type n, const void* = 0) {
    if (n > max_size()) throw "Out-of-memory";
    return static_cast<Tp*>(::operator new(n * sizeof(Tp)));
  }
  void deallocate(pointer p, size_type) { ::operator delete(p); }

  void construct(pointer p, const Tp& value) { 
    ::new((void*)p) Tp(value); 
  }
  void destroy(pointer p) { p->~Tp(); }
};
template <typename Tp1, typename Tp2>
inline bool operator==(const new_allocator<Tp1>& x, 
                       const new_allocator<Tp2>& y) {
  return true;
}
template <typename Tp1, typename Tp2>
inline bool operator!=(const new_allocator<Tp1>& x, 
                       const new_allocator<Tp2>& y) {
  return false;
}


template <typename Tp, typename Allocator = new_allocator<Tp>>
class vector {
public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Tp value_type;
  typedef Tp* pointer;
  typedef const Tp* const_pointer;
  typedef Tp& reference;
  typedef const Tp& const_reference;
  typedef Tp* iterator;
  typedef const Tp* const_iterator;
  typedef mystl::reverse_iterator<iterator> reverse_iterator;  
  typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef Allocator allocator_type;

  vector() : 
    start_(0), finish_(0), end_of_storage_(0), allocator_(Allocator()) {}
  explicit vector(const Allocator& alloc) : 
    start_(0), finish_(0), end_of_storage_(0), allocator_(alloc) {}
  vector(size_type n, const Allocator& alloc = Allocator()) :
    start_(allocator_.allocate(n)), 
    finish_(uninitialized_fill_n(start_, n, Tp())), 
    end_of_storage_(start_ + n), allocator_(alloc) {}
  vector(size_type n, const Tp& value, const Allocator& alloc = Allocator()) :
    start_(allocator_.allocate(n)), 
    finish_(uninitialized_fill_n(start_, n, value)), 
    end_of_storage_(start_ + n), allocator_(alloc) {}
  template <typename InputIterator>
  vector(InputIterator first, InputIterator last, 
         const Allocator& alloc = Allocator()) :
    start_(0), finish_(0), end_of_storage_(0), allocator_(alloc) {
    initialize_aux(first, last, alloc, 
      typename is_integral<InputIterator>::type());
  }
  vector(const vector& other) : 
    start_(allocator_.allocate(other.size())), 
    finish_(uninitialized_copy(other.begin(), other.end(), start_)), 
    end_of_storage_(start_ + other.size()), allocator_(Allocator()) {}
  vector(const vector& other, const Allocator& alloc) : 
    start_(allocator_.allocate(other.size())), 
    finish_(uninitialized_copy(other.begin(), other.end(), start_)), 
    end_of_storage_(start_ + other.size()), allocator_(alloc) {}
  vector(vector&& other) : 
    start_(move(other.start_)), finish_(move(other.finish_)),
    end_of_storage_(move(end_of_storage_)), allocator_(move(other.alloc)) {}
  vector(vector&& other, const Allocator& alloc) : 
    start_(move(other.start_)), finish_(move(other.finish_)),
    end_of_storage_(move(end_of_storage_)), allocator_(alloc) {}

  vector& operator=(const vector& other) {
    if (&other != this) {
      if (other.size() > capacity()) {
        iterator new_start = allocate_and_copy(
          other.size(), other.begin(), other.end());
        destroy_and_deallocate();
        start_ = new_start;
        end_of_storage_ = start_ + other.size();
      } else if (other.size() > size()) {
        copy(other.begin(), other.begin() + size(), begin());
        uninitialized_copy(other.begin() + size(), other.end(), finish_);
      } else {
        iterator new_finish = copy(other.begin(), other.end(), start_);
        destroy(new_finish, finish_);
      }
      finish_ = start_ + other.size();
    }
    return *this;
  }
  vector& operator=(vector&& other) {
    start_ = move(other.start_);
    finish_ = move(other.finish_);
    end_of_storage_ = move(other.end_of_storage_);
    allocator_ = move(other.allocator_);
    return *this;
  }

  ~vector() { destroy_and_deallocate(); }

  allocator_type get_allocator() const { return allocator_; }

  void assign(size_type n, const Tp& value) { fill_assign(n, value); }
  template <typename InputIterator>
  void assign(InputIterator first, InputIterator last) {
    assign_aux(first, last, 
      typename iterator_traits<InputIterator>::iterator_category());
  }

  reference at(size_type pos) {
    range_check(pos);
    return *(start_ + pos);
  }
  const_reference at(size_type pos) const {
    range_check(pos);
    return *(start_ + pos);
  }
  reference operator[](size_type pos) { return *(start_ + pos); }
  const_reference operator[](size_type pos) const { return *(start_ + pos); }
  reference front() { return *start_; }
  const_reference front() const { return *start_; }
  reference back() { return *(finish_ - 1); }
  const_reference back() const { return *(finish_ - 1); }
  pointer data() { return start_; }
  const_pointer data() const { return start_; }

  iterator begin() { return start_; }
  const_iterator begin() const { return start_; }
  const_iterator cbegin() const { return start_; }
  iterator end() { return finish_; }
  const_iterator end() const { return finish_; }
  const_iterator cend() const { return finish_; }
  reverse_iterator rbegin() { return reverse_iterator(finish_); }
  const_reverse_iterator rbegin() const { return reverse_iterator(finish_); }
  const_reverse_iterator crbegin() const { return reverse_iterator(finish_); }
  reverse_iterator rend() { return reverse_iterator(start_); }
  const_reverse_iterator rend() const { return reverse_iterator(start_); }
  const_reverse_iterator crend() const { return reverse_iterator(start_); }

  bool empty() const { return start_ == finish_; }
  size_type size() const { return size_type(finish_ - start_); }
  size_type max_size() const { return size_type(-1) / sizeof(Tp); }
  size_type capacity() const { return size_type(end_of_storage_ - start_); }

  void reserve(size_type n) {
    if (capacity() < n) {
      size_type old_size = size();
      iterator new_start = allocate_and_copy(n, start_, finish_);
      destroy_and_deallocate();
      start_ = new_start;
      finish_ = new_start + old_size;
      end_of_storage_ = start_ + n;
    }
  }
  void shrink_to_fit() {
    end_of_storage_ = finish_;
  }

  void clear() { erase(begin(), end()); }
  iterator insert(iterator pos, const Tp& value) {
    size_type n = pos - begin();
    if (finish_ != end_of_storage_ && pos == end()) {
      allocator_.construct(finish_, value);
      ++finish_;
    } else {
      insert_aux(pos, value);
    }
    return begin() + n;
  }
  iterator insert(iterator pos, Tp&& value) {
    size_type n = pos - begin();
    if (finish_ != end_of_storage_ && pos == end()) {
      allocator_.construct(finish_, move(value));
      ++finish_;
    } else {
      insert_aux(pos, move(value));
    }
    return begin() + n;
  }
  iterator insert(iterator pos, size_type n, const Tp& value) {
    if (finish_ + n - 1 != end_of_storage_ && pos == end()) {
      uninitialized_fill_n(finish_, n, value);
      finish_ += n;
    } else {
      insert_aux(pos, n, value);
    }
    return begin() + n;
  }
  template <typename InputIterator>
  iterator insert(iterator pos, InputIterator first, 
                  InputIterator last) {
    return insert_aux(pos, first, last, 
      typename is_integral<InputIterator>::type());
  }
  iterator erase(iterator pos) {
    if (pos + 1 != end()) {
      copy(pos + 1, finish_, pos);
    }
    --finish_;
    allocator_.destroy(finish_);
    return pos;
  }
  iterator erase(iterator first, iterator last) {
    iterator new_finish = copy(last, finish_, first);
    finish_ = new_finish;
    return first;
  }
  void push_back(const Tp& value) {
    if (finish_ != end_of_storage_) {
      allocator_.construct(finish_, value);
      ++finish_;
    } else {
      insert_aux(end(), value);
    }
  }
  void push_back(Tp&& value) {
    if (finish_ != end_of_storage_) {
      allocator_.construct(finish_, move(value));
      ++finish_;
    } else {
      insert_aux(end(), move(value));
    }
  }
  void pop_back() {
    --finish_;
    allocator_.destroy(finish_);
  }
  void resize(size_type n) { resize(n, Tp()); }
  void resize(size_type n, const Tp& value) {
    if (n < size()) {
      erase(begin() + n, end());
    } else {
      insert(end(), n - size(), value);
    }
  }
  void swap(vector& other) {
    using mystl::swap;
    swap(start_, other.start_);
    swap(finish_, other.finish_);
    swap(end_of_storage_, other.end_of_storage_);
  }

protected:
  template <typename Integral>
  void initialize_aux(Integral n, Integral value, const Allocator& alloc, 
                      true_type) {
    start_ = allocator_.allocate(n);
    finish_ = uninitialized_fill_n(start_, n, value);
    end_of_storage_ = start_ + n;
    allocator_ = alloc;
  }
  template <typename InputIterator>
  void initialize_aux(InputIterator first, InputIterator last, 
                      const Allocator& alloc, false_type) {
    range_initialize(first, last, alloc,
      typename iterator_traits<InputIterator>::iterator_category());
  }  
  template <typename InputIterator>
  void range_initialize(InputIterator first, InputIterator last, 
                        input_iterator_tag) {
    for (; first != last; ++first) {
      push_back(*first);
    }
  }
  template <typename ForwardIterator>
  void range_initialize(ForwardIterator first, ForwardIterator last,
                        const Allocator& alloc, forward_iterator_tag) {
    size_type n = distance(first, last);
    start_ = allocator_.allocate(n);
    finish_ = uninitialized_copy(first, last, start_);
    end_of_storage_ = start_ + n;
    allocator_ = alloc;
  }
  iterator allocate_and_copy(size_type n, const_iterator first, 
                             const_iterator last) {
    iterator result = allocator_.allocate(n);
    uninitialized_copy(first, last, result);
    return result;
  }
  void destroy(iterator first, iterator last) {
    for (; first != last; ++first) {
      allocator_.destroy(&*first);
    }
  }
  void destroy_and_deallocate() {
    destroy(start_, finish_);
    allocator_.deallocate(start_, end_of_storage_ - start_);
  }
  void fill_assign(size_type n, const Tp& value) {
    if (n > capacity()) {
      swap(vector(n, value, get_allocator()));
    } else if (n > size()) {
      fill(begin(), end(), value);
      finish_ = uninitialized_fill_n(finish_, n - size(), value);
    } else {
      erase(fill_n(begin(), n, value), end());
    }
  }
  template <typename InputIterator>
  void assign_aux(InputIterator first, InputIterator last, 
                  input_iterator_tag) {
    iterator it = begin();
    for (; first != last && it != end(); ++it, ++first) {
      *it = *first;
    }
    if (first == last) {
      erase(it, end());
    } else {
      insert(end(), first, last);
    }
  }
  template <typename ForwardIterator>
  void assign_aux(ForwardIterator first, ForwardIterator last, 
                  forward_iterator_tag) {
    size_type n = distance(first, last);
    if (n > capacity()) {
      iterator new_start = allocate_and_copy(n, first, last);
      destroy_and_deallocate();
      start_ = new_start;
      finish_ = start_ + n;
      end_of_storage_ = start_ + n;
    } else if (n > size()) {
      ForwardIterator mid = first;
      advance(mid, size());
      copy(first, mid, begin());
      finish_ = uninitialized_copy(mid, last, finish_);
    } else {
      iterator new_finish = copy(first, last, begin());
      destroy(new_finish, finish_);
      finish_ = new_finish;
    }
  }
  void range_check(size_type n) {
    if (n >= size()) throw "Out-of-memory";
  }
  void insert_aux(iterator pos, const Tp& value) {
    if (finish_ != end_of_storage_) {
      allocator_.construct(finish_, *(finish_ - 1));
      ++finish_;
      copy_backward(pos, finish_ - 2, finish_ - 1);
      *pos = value;
    } else {
      size_type old_size = size();
      size_type new_capacity = old_size != 0 ? 2 * old_size : 1;
      iterator new_start = allocator_.allocate(new_capacity);
      iterator new_finish = uninitialized_copy(start_, pos, new_start);
      allocator_.construct(new_finish, value);
      ++new_finish;
      new_finish = uninitialized_copy(pos, finish_, new_finish);
      destroy_and_deallocate();
      start_ = new_start;
      finish_ = new_finish;
      end_of_storage_ = start_ + new_capacity;
    }
  }
  void insert_aux(iterator pos, size_type n, const Tp& value) {
    if (finish_ + n - 1 != end_of_storage_) {
      iterator old_finish = finish_;
      uninitialized_copy(finish_ - n, finish_, finish_);
      finish_ += n;
      copy_backward(pos, old_finish - n, old_finish);
      fill_n(pos, n, value);
    } else {
      size_type old_size = size();
      size_type new_capacity = old_size >= n ? 2 * old_size : old_size + n;
      iterator new_start = allocator_.allocate(new_capacity);
      iterator new_finish = uninitialized_copy(start_, pos, new_start);
      fill_n(pos, n, value);
      new_finish += n;
      new_finish = uninitialized_copy(pos, finish_, new_finish);
      destroy_and_deallocate();
      start_ = new_start;
      finish_ = new_finish;
      end_of_storage_ = start_ + new_capacity;
    }
  }
  template <typename Integral>
  iterator insert_aux(iterator pos, Integral n, Integral value, true_type) {
    if (finish_ + n - 1 != end_of_storage_ && pos == end()) {
      uninitialized_fill_n(finish_, n, value);
      finish_ += n;
    } else {
      insert_aux(pos, n, value);
    }
    return begin() + n;    
  }
  template <typename InputIterator>
  iterator insert_aux(iterator pos, InputIterator first, InputIterator last,
                  false_type) {
    size_type n = pos - begin();
    range_insert(pos, first, last, 
      typename iterator_traits<InputIterator>::iterator_category());
    return begin() + n;
  }
  template <typename InputIterator>
  void range_insert(iterator pos, InputIterator first, InputIterator last,
                    input_iterator_tag) {
    for (; first != last; ++first) {
      pos = insert(pos, *first);
      ++pos;
    }
  }
  template <typename ForwardIterator>
  void range_insert(iterator pos, ForwardIterator first, ForwardIterator last,
                    forward_iterator_tag) {
    if (first == last) return;
    size_type n = distance(first, last);
    if (n <= size_type(end_of_storage_ - finish_)) {
      size_type back_len = finish_ - pos;
      iterator old_finish = finish_;
      if (back_len > n) {
        uninitialized_copy(finish_ - n, finish_, finish_);
        finish_ += n;
        copy_backward(pos, old_finish - n, old_finish);
        copy(first, last, pos);
      } else {
        ForwardIterator mid = first;
        advance(mid, back_len);
        uninitialized_copy(mid, last, finish_);
        finish_ += n - back_len;
        uninitialized_copy(pos, old_finish, finish_);
        finish_ += back_len;
        copy(first, mid, pos);
      }
    } else {
      size_type old_size = size();
      size_type new_capacity = old_size >= n ? 2 * old_size : old_size + n;
      iterator new_start = allocator_.allocate(new_capacity);
      iterator new_finish = uninitialized_copy(start_, pos, new_start);
      new_finish = uninitialized_copy(first, last, new_finish);
      new_finish = uninitialized_copy(pos, finish_, new_finish);
      destroy_and_deallocate();
      start_ = new_start;
      finish_ = new_finish;
      end_of_storage_ = start_ + new_capacity;      
    }
  }

  Tp* start_;
  Tp* finish_;
  Tp* end_of_storage_;
  Allocator allocator_;
};

template <typename Tp, typename Allocator>
bool operator==(const vector<Tp, Allocator>& x, 
                const vector<Tp, Allocator>& y) {
  if (x.size() != y.size()) return false;
  typename vector<Tp, Allocator>::const_iterator 
           it_x = x.cbegin(), it_y = y.cbegin();
  for (; it_x != x.cend(); ++it_x, ++it_y) {
    if (*it_x != *it_y) return false;
  }
  return true;
}
template <typename Tp, typename Allocator>
bool operator<(const vector<Tp, Allocator>& x, 
               const vector<Tp, Allocator>& y) {
  typename vector<Tp, Allocator>::const_iterator 
           it_x = x.cbegin(), it_y = y.cbegin();
  for (; it_x != x.cend() && it_y != y.cend(); ++it_x, ++it_y) {
    if (*it_x < *it_y) return true;
    if (*it_y < *it_x) return false;
  }
  return it_x == x.cend() && it_y != y.cend();
}
template <typename Tp, typename Allocator>
bool operator!=(const vector<Tp, Allocator>& x, 
                const vector<Tp, Allocator>& y) {
  return !(x == y);
}
template <typename Tp, typename Allocator>
bool operator>(const vector<Tp, Allocator>& x, 
               const vector<Tp, Allocator>& y) {
  return y < x;
}
template <typename Tp, typename Allocator>
bool operator<=(const vector<Tp, Allocator>& x, 
                const vector<Tp, Allocator>& y) {
  return !(y < x);
}
template <typename Tp, typename Allocator>
bool operator>=(const vector<Tp, Allocator>& x, 
                const vector<Tp, Allocator>& y) {
  return !(x < y);
}

}  // namespace mystl

#endif  // MYSTL_VECTOR_H