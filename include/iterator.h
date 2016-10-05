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

/* ==== C++ STL Iterators ==== */

/* - 2016-10-03
 * - waizungtaam@gmail.com
 *
 * - class iterator
 *
 * - iterator tags
 *   - input_iterator_tag
 *   - output_iterator_tag
 *   - forward_iterator_tag
 *   - bidirectional_iterator_tag
 *   - random_access_iterator_tag
 *
 * - class iterator_traits
 *   - iterator_traits<Tp*>
 *   - iterator_traits<const Tp*>
 *
 * - Basic iterators
 *   - input_iterator
 *   - output_iterator
 *   - forward_iterator
 *   - bidirectional_iterator
 *   - random_access_iterator
 *
 * - distance()
 * - advance()
 * 
 * - Advanced iterators
 *   - back_insert_iterator 
 *   - front_insert_iterator
 *   - insert_iterator
 *   - reverse_bidirectional_iterator
 *   - reverse_iterator<Iterator>
 *   - reverse_iterator<Iterator, Tp, Reference, Distance>
 */
#ifndef MYSTL_ITERATOR_H
#define MYSTL_ITERATOR_H

namespace mystl {

typedef unsigned long size_t;
typedef long ptrdiff_t;

/* class iterator */
template <typename Category, typename Tp, typename Distance = ptrdiff_t,
          typename Pointer = Tp*, typename Reference = Tp&>
struct iterator {
  typedef Category iterator_category;
  typedef Tp value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
};

/* iterator tags */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/* iterator_traits */
template <typename Iterator>
struct iterator_traits {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
};
template <typename Tp>
struct iterator_traits<Tp*> {
  typedef random_access_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef ptrdiff_t difference_type;
  typedef Tp* pointer;
  typedef Tp& reference;
};
template <typename Tp>
struct iterator_traits<const Tp*> {
  typedef random_access_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef ptrdiff_t difference_type;
  typedef const Tp* pointer;
  typedef const Tp& reference;
};

/* basic iterators */
template <typename Tp, typename Distance>
struct input_iterator {
  typedef input_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef Distance difference_type;
  typedef Tp* pointer;
  typedef Tp& reference;
};
struct output_iterator {
  typedef output_iterator_tag iterator_category;
  typedef void value_type;
  typedef void difference_type;
  typedef void pointer;
  typedef void reference;
};
template <typename Tp, typename Distance>
struct forward_iterator {
  typedef forward_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef Distance difference_type;
  typedef Tp* pointer;
  typedef Tp& reference;
};
template <typename Tp, typename Distance>
struct bidirectional_iterator {
  typedef bidirectional_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef Distance difference_type;
  typedef Tp* pointer;
  typedef Tp& reference;
};
template <typename Tp, typename Distance>
struct random_access_iterator {
  typedef random_access_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef Distance difference_type;
  typedef Tp* pointer;
  typedef Tp& reference;
};

/* distance() */
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last, input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for (; first != last; ++first) {
    ++n;
  }
  return n;
}
template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
distance(RandomAccessIterator first, RandomAccessIterator last,
         random_access_iterator_tag) {
  return last - first;
}
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
  return distance(first, last, 
    typename iterator_traits<InputIterator>::iterator_category());
}

/* advance() */
template <typename InputIterator, typename Distance>
inline void advance(InputIterator& it, Distance n, input_iterator_tag) {
  for (; n > 0; --n) ++it;
}
template <typename BidirectionalIterator, typename Distance>
inline void advance(BidirectionalIterator& it, Distance n, 
                    bidirectional_iterator_tag) {
  if (n >= 0)
    for (; n != 0; --n) ++it;
  else
    for (; n != 0; ++n) --it;
}
template <typename RandomAccessIterator, typename Distance>
inline void advance(RandomAccessIterator& it, Distance n,
                    RandomAccessIterator) {
  it += n;
}
template <typename InputIterator, typename Distance>
inline void advance(InputIterator& it, Distance n) {
  advance(it, n, typename iterator_traits<InputIterator>::iterator_category());
}

/* back_insert_iterator */
template <typename Container>
class back_insert_iterator {
public:
  typedef Container container_type;
  typedef output_iterator_tag iterator_category;
  typedef void value_type;
  typedef void difference_type;
  typedef void pointer;
  typedef void reference;

  explicit back_insert_iterator(Container& c) : container_(&c) {}
  back_insert_iterator& operator=(
    const typename Container::value_type& value) {
    container_->push_back(value);
    return *this;
  }
  back_insert_iterator& operator*() { return *this; }
  back_insert_iterator& operator++() { return *this; }
  back_insert_iterator& operator++(int) { return *this; }

protected:
  Container* container_;
};

/* front_insert_iterator */
template <typename Container>
class front_insert_iterator {
public:
  typedef Container container_type;
  typedef output_iterator_tag iterator_category;
  typedef void value_type;
  typedef void difference_type;
  typedef void pointer;
  typedef void reference;

  explicit front_insert_iterator(Container& c) : container_(&c) {}
  front_insert_iterator& operator=(
    const typename Container::value_type& value) {
    container_->push_front(value);
    return *this;
  }

  front_insert_iterator& operator*() { return *this; }
  front_insert_iterator& operator++() { return *this; }
  front_insert_iterator& operator++(int) { return *this; }

protected:
  Container* container_;
};

/* insert_iterator */
template <typename Container>
class insert_iterator {
public:
  typedef Container container_type;
  typedef output_iterator_tag iterator_category;
  typedef void value_type;
  typedef void difference_type;
  typedef void pointer;
  typedef void reference;

  insert_iterator(Container& c, typename Container::iterator it) :
    container_(&c), iterator_(it) {}
  insert_iterator& operator=(const typename Container::value_type& value) {
    iterator_ = container_->insert(iterator_, value);
    ++iterator_;
    return *this;
  }
  insert_iterator& operator*() { return *this; }
  insert_iterator& operator++() { return *this; }
  insert_iterator& operator++(int) { return *this; }

protected:
  Container* container_;
  typename Container::iterator iterator_;
};

/* reverse_bidirectional_iterator */
template <typename BidirectionalIterator, typename Tp, 
          typename Reference = Tp&, typename Distance = ptrdiff_t>
class reverse_bidirectional_iterator {
public:
  typedef bidirectional_iterator_tag iterator_category;
  typedef Tp value_type;
  typedef Distance difference_type;
  typedef Tp* pointer;
  typedef Tp& reference;

  reverse_bidirectional_iterator() {}
  explicit reverse_bidirectional_iterator(BidirectionalIterator it) :
    current_(it) {}
  BidirectionalIterator base() const { return current_; }
  reference operator*() const {
    BidirectionalIterator it = current_;
    return *(--it);
  }
  pointer operator->() const { return &(operator*()); }

  reverse_bidirectional_iterator& operator++() {
    --current_;
    return *this;
  }
  reverse_bidirectional_iterator operator++(int) {
    reverse_bidirectional_iterator ret = *this;
    --current_;
    return ret;
  }
  reverse_bidirectional_iterator& operator--() {
    ++current_;
    return *this;
  }
  reverse_bidirectional_iterator operator--(int) {
    reverse_bidirectional_iterator ret = *this;
    ++current_;
    return ret;
  }

protected:
  BidirectionalIterator current_;
};

template <typename BidirectionalIterator, typename Tp, typename Reference,
          typename Distance>
inline bool operator==(
  const reverse_bidirectional_iterator<
  BidirectionalIterator, Tp, Reference, Distance>& x,
  const reverse_bidirectional_iterator<
  BidirectionalIterator, Tp, Reference, Distance>& y) {
  return x.base() == y.base();
}
template <typename BidirectionalIterator, typename Tp, typename Reference,
          typename Distance>
inline bool operator!=(
  const reverse_bidirectional_iterator<
  BidirectionalIterator, Tp, Reference, Distance>& x,
  const reverse_bidirectional_iterator<
  BidirectionalIterator, Tp, Reference, Distance>& y) {
  return !(x == y);
}

/* reverse_iterator<Iterator> */
template <typename Iterator>
class reverse_iterator {
public:
  typedef typename iterator_traits<Iterator>::iterator_category 
                   iterator_category;
  typedef typename iterator_traits<Iterator>::value_type value_type;
  typedef typename iterator_traits<Iterator>::difference_type difference_type;
  typedef typename iterator_traits<Iterator>::pointer pointer;
  typedef typename iterator_traits<Iterator>::reference reference;
  typedef Iterator iterator_type;

  reverse_iterator() {}
  explicit reverse_iterator(iterator_type it) : current_(it) {}
  reverse_iterator(const reverse_iterator& other) : current_(other.current_) {}
  template <typename Iter>
  reverse_iterator(const reverse_iterator<Iter>& other) : 
    current_(other.base()) {}

  iterator_type base() const { return current_; }
  reference operator*() const {
    Iterator it = current_;
    return *(--it);
  }
  pointer operator->() const { return &(operator*()); }

  reverse_iterator& operator++() {
    --current_;
    return *this;
  }
  reverse_iterator operator++(int) {
    reverse_iterator ret = *this;
    --current_;
    return ret;
  }
  reverse_iterator& operator--() {
    ++current_;
    return *this;
  }
  reverse_iterator operator--(int) {
    reverse_iterator ret = *this;
    ++current_;
    return ret;
  }

  reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(current_ - n);
  }
  reverse_iterator& operator+=(difference_type n) {
    current_ -= n;
    return *this;
  }
  reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(current_ + n);
  }
  reverse_iterator& operator-=(difference_type n) {
    current_ += n;
    return *this;
  }

  reference operator[](difference_type n) const { return *(*this + n); }

protected:
  Iterator current_;
};

template <typename Iterator>
inline bool operator==(const reverse_iterator<Iterator>& x,
                       const reverse_iterator<Iterator>& y) {
  return x.base() == y.base();
}
template <typename Iterator>
inline bool operator<(const reverse_iterator<Iterator>& x,
                      const reverse_iterator<Iterator>& y) {
  return y.base() < x.base();  // reversed order for reverse_iterator
}

template <typename Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& x,
                       const reverse_iterator<Iterator>& y) {
  return !(x == y);
}
template <typename Iterator>
inline bool operator>(const reverse_iterator<Iterator>& x,
                      const reverse_iterator<Iterator>& y) {
  return y < x;
}
template <typename Iterator>
inline bool operator<=(const reverse_iterator<Iterator>& x,
                       const reverse_iterator<Iterator>& y) {
  return !(y < x);
}
template <typename Iterator>
inline bool operator>=(const reverse_iterator<Iterator>& x,
                       const reverse_iterator<Iterator>& y) {
  return !(x < y);
}
template <typename Iterator>
inline typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& x, 
          const reverse_iterator<Iterator>& y) {
  return y.base() - x.base();
}
template <typename Iterator>
inline reverse_iterator<Iterator> 
operator+(typename reverse_iterator<Iterator>::difference_type n, 
          const reverse_iterator<Iterator>& it) {
  return it + n;
}

}  // namespace mystl

#endif  // MYSTL_ITERATOR_H