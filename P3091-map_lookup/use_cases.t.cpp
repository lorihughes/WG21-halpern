/* use_cases.t.cpp                  -*-C++-*-
 *
 * Copyright (C) 2016 Pablo Halpern <phalpern@halpernwightsoftware.com>
 * Distributed under the Boost Software License - Version 1.0
 *
 * Use cases/tests for PXXXX Non-mutating lookups for `map` and `unordered_map`
 */

#include <xmap.h>
#include <string>
#include <iostream>
#include <utility>
#include <cassert>

using std::experimental::map;

class NotDefaultConstructible
{
  // This class does not have a default ctor
  int m_value;

public:
  explicit NotDefaultConstructible(int v) : m_value(v) { }

  int value() const { return m_value; }

  friend std::strong_ordering operator<=>(NotDefaultConstructible a,
                                          NotDefaultConstructible b) = default;
};

bool operator==(NotDefaultConstructible a, int b) { return a.value() == b; }

void nonZero()
{
  map<int, unsigned> data = {
    { 0, 10 },
    { 4, 8 },
    { 5, 2 },
    { 8, 6 },
    { 11, 9 }
  };

  assert(data.size() == 5);

  // Find the minimum of value for odd-numbered keys
  unsigned smallest = 100;
  for (int i = 1; i < 15; i += 2)
    smallest = std::min(smallest, data.get(i, 100));

  assert(data.size() == 5);  // No new items added
  assert(2 == smallest);     // Correctly computed smallest value
}

void constMap()
{
  const map<const char*, int> m{ { "one", 1 }, { "two", 2}, { "three", 3 }};

//  int v = m["two"];  // Won't compile
  int v = m.get("two");  // OK

  assert(2 == v);
}

void noDefaultCtor()
{
  map<std::string, NotDefaultConstructible> m;

  m.try_emplace("hello", 5);

//  auto e = m["hello"];          // Won't compile
  auto e = m.get("hello", 99);  // OK

  assert(5 == e);
}

struct Person
{
  std::string first_name;
  std::string last_name;
  std::string address;

  friend bool operator==(const Person&, const Person&) = default;
};

void bigValue()
{
  const Person nobody{};

  map<unsigned, Person> id_to_person;
  unsigned id = 0;

  Person        p1 = id_to_person.get(id);              // Copies element
  Person const& p2 = id_to_person.get_ref(id, nobody);  // No copies made

  assert(p1.first_name.empty());
  assert(&p1 != &nobody);
  assert(p2.first_name.empty());
  assert(&p2 == &nobody);

  // Person const& p3 = id_to_person.get_ref(id, Person{});  // Won't compile
}

void convertedValue()
{
  const map<int, std::string> m{
    { 1, "one one one one one one one one one one one one one" },
    { 2, "two two two two two two two two two two two two two" },
    { 3, "three three three three three three three three three" }
  };

  std::string_view sv1 = m.get(1, "none");  // Dangling reference
  std::string_view sv2 = m.get_as<std::string_view>(2, "none");  // OK
  std::string_view sv3 = m.get_as<std::string_view>(5, "nonex", 4);  // OK

  (void) sv1;

  // assert("one"  == sv1.substr(0, 3));  // Fails (as expected) in gcc
  assert("two"  == sv2.substr(0, 3));
  assert("none" == sv3);
}

int main()
{
  nonZero();
  constMap();
  noDefaultCtor();
  bigValue();
  convertedValue();
}