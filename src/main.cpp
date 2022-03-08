#include "include/test.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>

class A {};

int main() {
  std::cout << test::get_sum(5, 5) << std::endl;
  boost::shared_ptr<A> pA(new A);
  std::cout << pA.get() << std::endl;
}