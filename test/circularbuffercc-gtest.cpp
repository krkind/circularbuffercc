/*
 * Unit test for the circular buffer
 */

#include "circularbuffer.hpp"
#include "gtest/gtest.h"

namespace {

#define BUF_SIZE 4u

// The fixture for testing class Foo.
class CircularBufferTest : public ::testing::Test {
   protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    CircularBufferTest() : cbuf_(BUF_SIZE) {
        // You can do set-up work for each test here.
    }

    ~CircularBufferTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for
    // CircularBuffer.
    circular_buffer<uint32_t> cbuf_;
};

// Tests that the Init operation does the intialalization.
TEST_F(CircularBufferTest, Init) {
    ASSERT_EQ(cbuf_.empty(), true);
    ASSERT_EQ(cbuf_.count(), 0u);
    ASSERT_EQ(cbuf_.space(), BUF_SIZE);
}

// Tests that Clear operation clears the circular buffer
TEST_F(CircularBufferTest, Clear) {
    // Fill the buffer
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        ASSERT_EQ(cbuf_.push_back(i), true);
    }

    // Test to clear the buffer
    cbuf_.clear();

    // Check that the buffer is cleared
    ASSERT_EQ(cbuf_.empty(), true);
    ASSERT_EQ(cbuf_.count(), 0u);
    ASSERT_EQ(cbuf_.space(), BUF_SIZE);
}

// Tests that PushBack operation add correct number of elements.
TEST_F(CircularBufferTest, PushBack) {
    // Test to fill the buffer
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        ASSERT_EQ(cbuf_.push_back(i), true);
    }

    // Check that the buffer is filled
    ASSERT_EQ(cbuf_.count(), BUF_SIZE);
    ASSERT_EQ(cbuf_.space(), 0u);

    // Test out of boundary
    ASSERT_EQ(cbuf_.push_back(10u), false);
    ASSERT_EQ(cbuf_.count(), BUF_SIZE);
}

// Tests that PopFront operation pop correct value and
// correct number of elements.
TEST_F(CircularBufferTest, PopFront) {
    // Fill the buffer
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        ASSERT_EQ(cbuf_.push_back(i), true);
    }

    // Test to get all the elements
    uint32_t data;

    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        ASSERT_EQ(cbuf_.pop_front(data), true);
        ASSERT_EQ(data, i);
    }

    // Check that all elements are removed
    ASSERT_EQ(cbuf_.count(), 0u);
    ASSERT_EQ(cbuf_.space(), BUF_SIZE);

    // Test out of boundary
    ASSERT_EQ(cbuf_.pop_front(data), false);
    ASSERT_EQ(cbuf_.count(), 0u);
}

// Tests that Peek operation return correct pointer.
TEST_F(CircularBufferTest, Peek) {
    uint32_t data[BUF_SIZE] = {0};

    // Fill the buffer.
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        data[i] = 10 + i;
        ASSERT_EQ(cbuf_.push_back(data[i]), true);
    }

    // Test to peek all the added elements
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        uint32_t* data_p;
        ASSERT_EQ(cbuf_.peek(i, data_p), true);
        ASSERT_EQ(*data_p, data[i]);
    }

    // Remove 2 elements and add 1 element.
    for (uint32_t i = 0; i < (BUF_SIZE / 2); i++) {
        ASSERT_EQ(cbuf_.pop_front(data[i]), true);
        data[i] = data[i + (BUF_SIZE / 2)];
    }
    for (uint32_t i = (BUF_SIZE / 2); i < BUF_SIZE - 1; i++) {
        data[i] = 20 + i;
        ASSERT_EQ(cbuf_.push_back(data[i]), true);
    }

    // Test to peek all added elements
    for (uint32_t i = 0; i < BUF_SIZE - 1; i++) {
        uint32_t* data_p;
        ASSERT_EQ(cbuf_.peek(i, data_p), true);
        ASSERT_EQ(*data_p, data[i]);
    }

    // Fill the buffer again
    for (uint32_t i = (BUF_SIZE / 2) + 1; i < BUF_SIZE; i++) {
        data[i] = 10 + i;
        ASSERT_EQ(cbuf_.push_back(data[i]), true);
    }

    // Test to peek all added element
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        uint32_t* data_p;
        ASSERT_EQ(cbuf_.peek(i, data_p), true);
        ASSERT_EQ(*data_p, data[i]);
    }

    // Test out of boundary
    uint32_t* data_p = nullptr;
    ASSERT_EQ(cbuf_.peek(BUF_SIZE, data_p), false);
    ASSERT_EQ(data_p, nullptr);
}

// Tests that Count and Space operations return correct value.
TEST_F(CircularBufferTest, CountAndSpace) {
    uint32_t data;

    // Test to fill the buffer
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        ASSERT_EQ(cbuf_.count(), i);
        ASSERT_EQ(cbuf_.space(), BUF_SIZE - i);
        ASSERT_EQ(cbuf_.push_back(data), true);
    }

    // Test to remove all the elements
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        ASSERT_EQ(cbuf_.count(), BUF_SIZE - i);
        ASSERT_EQ(cbuf_.space(), i);
        ASSERT_EQ(cbuf_.pop_front(data), true);
    }

    // Check that the buffer is empty
    ASSERT_EQ(cbuf_.count(), 0u);
    ASSERT_EQ(cbuf_.space(), BUF_SIZE);
}

// Tests that IsEmpty operation return correct value.
TEST_F(CircularBufferTest, IsEmpty) {
    ASSERT_EQ(cbuf_.empty(), true);
    ASSERT_EQ(cbuf_.push_back(1), true);
    ASSERT_EQ(cbuf_.empty(), false);
}

}  // namespace

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
