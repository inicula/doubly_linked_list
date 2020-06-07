#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <random>
#include "DoublyLinkedList.h"

using dll = DoublyLinkedList<int>;

int main()
{
  constexpr size_t n = 17;
  {
    // create default
    dll mylist;

    // resize
    mylist.resize(n);

    std::iota(mylist.begin(), mylist.end(), 1);

    mylist.print();

    std::cout << "resize lower size:\n";
    mylist.resize(n - 5);
    mylist.print();

    std::cout << "Reset list\n";
    mylist.erase(mylist.begin(), mylist.end());

    std::cout << "append vector:\n";
    mylist.push_back(100);
    std::vector<int> vec(n);
    std::iota(vec.begin(), vec.end(), 0);

    std::copy(vec.begin(), vec.end(), std::back_inserter(mylist));
    std::copy_n(vec.begin(), 10, std::back_inserter(mylist));

    mylist.print();

    auto pos = std::find(mylist.cbegin(), mylist.cend(), 10);

    std::cout << "Element: " << 10
              << (pos == mylist.cend() ? " not found" : " found") << '\n';

    std::cout << "delete element 10\n";
    mylist.erase(pos);
    pos = std::find(mylist.cbegin(), mylist.cend(), 10);
    mylist.print();
    std::cout << "Element: " << 10
              << (pos == mylist.cend() ? " not found" : " found") << '\n';

    std::cout << "remove odd elements:\n";
    mylist.erase(std::remove_if(mylist.begin(), mylist.end(),
                                [](const auto& el) { return el % 2 == 1; }),
                 mylist.end());
    mylist.print();

    std::cout << "Reinitialize with shuffled vector\n";
    vec = std::vector<int>(mylist.cbegin(), mylist.cend());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);

    mylist = dll(vec.begin(), vec.end());

    mylist.print();

    std::cout << "Check if the list is sorted:\n";
    std::cout << "List is "
              << (std::is_sorted(mylist.cbegin(), mylist.cend()) ? "sorted"
                                                                 : "not sorted")
              << '\n';

    std::cout << "Try sort, check if the resulting list is sorted:\n";
    mylist.sort();
    mylist.print();
    std::cout << "List is "
              << (std::is_sorted(mylist.cbegin(), mylist.cend()) ? "sorted"
                                                                 : "not sorted")
              << '\n';

    std::cout << "List sum: "
              << std::accumulate(mylist.crbegin(), mylist.crend(), 0) << '\n';
  }

  dll mylist0 = {1, 2, 3, 4, 5, 6};
  dll mylist1 = {1, 2, 3, 4, 5, 6};

  mylist0.print();
  mylist1.print();

  if(mylist0 == mylist1)
  {
    std::cout << "The lists are equal\n";
  }
  else
  {
    std::cout << "The lists are not equal\n";
  }

  std::vector<int> vec;
  for(size_t sz = 0; sz < n; ++sz)
  {
    vec.push_back(std::rand());
  }

  std::cout << "Sort test:\n";
  mylist0 = dll(vec.cbegin(), vec.cend());
  mylist1 = dll(vec.cbegin(), vec.cend());

  mylist0.print();

  mylist0.sort();
  mylist1.quick_sort();

  if(mylist0 == mylist1 && std::is_sorted(mylist0.cbegin(), mylist0.cend()) &&
     std::is_sorted(mylist1.cbegin(), mylist1.cend()))
  {
    std::cout << "The lists are sorted and equal:\n";
  }
  mylist0.print();
  mylist1.print();
}
