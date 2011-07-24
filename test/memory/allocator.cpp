#include <memory>
#include <gtest/gtest.h>
#include "sml/memory/allocator.hpp"

namespace {

using testing::StaticAssertTypeEq;

struct Counter {
  Counter(int i) : id(i) {
    Counter::throw_except();
    ++Counter::count;
  }

  Counter(Counter const& r) : id(r.id) {
    Counter::throw_except();
    ++Counter::count;
  }

  ~Counter() {
    --Counter::count;
  }

  static void throw_except() {
    if (Counter::count == Counter::except_count) {
      throw Counter::count;
    }
  }

  static int count;
  static int except_count;
  int id;
};

int Counter::count = 0;
int Counter::except_count = -1;

typedef sml::memory::allocator< std::allocator<Counter> > allocator_type;
typedef allocator_type::pointer pointer;
typedef allocator_type::base_allocator_type base_allocator_type;
typedef allocator_type::size_type size_type;

class Allocator : public testing::Test {
protected:
  void SetUp() {
    Counter::count = 0;
    Counter::except_count = -1;
  }
};

TEST_F(Allocator, Constructor) {
  allocator_type allocator;

  ASSERT_EQ(pointer(), allocator.get());
  ASSERT_EQ(pointer(), allocator.release());
}

TEST_F(Allocator, CopyConstructor) {
  base_allocator_type const base;
  allocator_type allocator(base);

  ASSERT_EQ(pointer(), allocator.get());
  ASSERT_EQ(pointer(), allocator.release());
}

TEST_F(Allocator, Destructor) {
  size_type const size = 5;
  int const id = 9823;

  {
    allocator_type allocator;
    allocator.construct(size, Counter(id));
  }

  ASSERT_EQ(0, Counter::count);
}

TEST_F(Allocator, Construct) {
  size_type const size = 5;
  int const id = 3829;
  allocator_type allocator;

  allocator.construct(size, Counter(id));

  ASSERT_EQ(size,      Counter::count);
  ASSERT_NE(pointer(), allocator.get());

  for (size_type i = 0; i < size; ++i) {
    ASSERT_EQ(id, allocator.get()[i].id);
  }
}

TEST_F(Allocator, Release) {
  size_type const size = 5;
  int const id = 1023;
  pointer getted = pointer();
  pointer released = pointer();

  {
    allocator_type allocator;
    allocator.construct(size, Counter(id));
    getted = allocator.get();
    released = allocator.release();

    ASSERT_EQ(pointer(), allocator.get());
    ASSERT_EQ(pointer(), allocator.release());
  }

  ASSERT_EQ(released,  getted);
  ASSERT_EQ(size,      Counter::count);

  for (size_type i = 0; i < size; ++i) {
    ASSERT_EQ(id, released[i].id);
  }

  allocator_type().destroy(size, released);
}

TEST_F(Allocator, Destroy) {
  size_type const size = 5;
  int const id = 7081;
  allocator_type allocator;
  allocator.construct(size, Counter(id));

  pointer released = allocator.release();
  allocator_type().destroy(size, released);

  ASSERT_EQ(0, Counter::count);
}

TEST_F(Allocator, ExceptionInValueCopyConstructor) {
  Counter::except_count = 3;

  size_type const size = 5;
  int const id = 7081;

  {
    allocator_type allocator;
    try {
      allocator.construct(size, Counter(id));
      FAIL() << "a exception must be thrown";
    }
    catch (...) {
      ASSERT_EQ(2, Counter::count);
    }
  }

  ASSERT_EQ(0, Counter::count);
}

TEST_F(Allocator, Types) {
  typedef allocator_type::base_allocator_type base;

  StaticAssertTypeEq<std::allocator<Counter>, base>();
  StaticAssertTypeEq<base::value_type,      allocator_type::value_type>();
  StaticAssertTypeEq<base::pointer,         allocator_type::pointer>();
  StaticAssertTypeEq<base::const_pointer,   allocator_type::const_pointer>();
  StaticAssertTypeEq<base::reference,       allocator_type::reference>();
  StaticAssertTypeEq<base::const_reference, allocator_type::const_reference>();
  StaticAssertTypeEq<base::size_type,       allocator_type::size_type>();
  StaticAssertTypeEq<base::difference_type, allocator_type::difference_type>();
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
