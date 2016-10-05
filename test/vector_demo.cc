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

/* ==== STL vector demo ==== */

// $ valgrind --leak-check=full ./vector_demo.o

#include "../include/vector.h"
#include <vector>

#include <iostream>

template <typename Tp>
using Vector = mystl::vector<Tp>;

template <typename Tp>
void print(const Vector<Tp>& v) {
  std::cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << "\t";
  for (const int& x : v) std::cout << x << " ";
  std::cout << "\n";  
}

int main() {
  Vector<int> a;
  for (int i = 0; i < 10; ++i) {
    a.push_back(i);
    print(a);
  }
  for (int i = 0; i < 10; ++i) {
    a.pop_back();
    print(a);
  }

  Vector<int> b(10);
  print(b);

  Vector<int> c(10, 1);
  print(c);

  Vector<int> d = c;
  c[0] = 0;
  print(c);
  print(d);
  d[9] = 9;
  print(c);
  print(d);

  Vector<int> e(d.begin(), d.end());
  d[0] = 10;
  print(d);
  print(e);

  a = d;
  d[1] = 11;
  print(a);
  print(d);

  std::cout << d.front() << " " << d.back() << "\n";

  print(d);
  d.push_back(2);
  print(d);
  d.shrink_to_fit();
  print(d);
  d.push_back(3);
  print(d);

  e.clear();
  print(e);

  print(d);
  d.insert(d.begin() + 2, 12);
  print(d);
  d.insert(d.begin() + 2, 3, 13);
  print(d);
  d.insert(d.begin() + 2, c.begin(), c.begin() + 2);
  print(d);

  d.erase(d.begin());
  print(d);
  d.erase(d.begin() + 5, d.end());
  print(d);

  d.resize(7);
  print(d);
  d.resize(4);
  print(d);

  print(c);
  d.swap(c);
  print(d);
  print(c);
}