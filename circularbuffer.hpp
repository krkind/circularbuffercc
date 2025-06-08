/*
 * MIT License (MIT)
 *
 * Copyright (c) 2019 Kristian Kinderlöv
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @addtogroup CIRCULARBUFFER
 * @ingroup MISC
 * @{
 *
 * @file        circularbuffer.hpp
 *
 * @brief       A Circular buffer template class.
 *
 * The implementation uses std::unique_ptr for automatically delete the
 * allocated dynamic memory when it not used anymore. For thread safety
 * std::mutex is used. Its require C++ latest revison 2011.
 */
#pragma once

#include <memory>
#include <mutex>
#include <optional>

template <class T>
class circular_buffer {
public:
    /**
     * @brief The circular buffer constructor.
     *
     * @param[in]   num     Total number of elements that the circular buffer
     *                      can hold.
     */
    explicit circular_buffer(size_t num) : buf_(std::make_unique<T[]>(num)), max_(num) {
        // Do nothing.
    }

    /**
     * @brief The circular buffer destructor.
     */
    virtual ~circular_buffer() {
        // Do nothing.
    }

    /**
     * @brief Removes all elements from the circular buffer.
     */
    void clear(void) {
        std::lock_guard<std::mutex> lock(mutex_);

        write_pos_ = 0;
        read_pos_ = 0;
        count_ = 0;
    }

    /**
     * @brief Adds a new element at the end of the buffer. The arguments are
     * perfectly forwarded to the element's constructor.
     *
     * @tparam Args Argument types for the element's constructor.
     * @param[in] args Arguments to forward to the element's constructor.
     * @return True if success, false if the buffer is full.
     */
    template <typename... Args>
    bool push_back(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex_);

        // Check if buffer is full
        if (count_ == max_) {
            return false;
        }

        buf_[write_pos_] = T(std::forward<Args>(args)...);
        write_pos_ = (write_pos_ + 1) % max_;
        ++count_;

        return true;
    };

    /**
     * @brief Removes the first element from the buffer and returns it as an optional.
     *
     * @return std::optional<T> containing the popped element, or std::nullopt if the buffer is empty.
     */
    std::optional<T> pop_front() {
        std::lock_guard<std::mutex> lock(mutex_);

        if (count_ == 0) {
            return std::nullopt;
        }

        T elem = buf_[read_pos_];
        read_pos_ = (read_pos_ + 1) % max_;
        --count_;

        return std::make_optional(std::move(elem));
    }

    /**
     * @brief Peeks the "num" element from the buffer.
     *
     * The "num" argument shall be less than the number of elements added to
     * the buffer.
     *
     * @param[in]   num     The number of the element to peek.
     * @param[out]  elem    Pointer to reference to the "num" element.
     * @return              True if success, false if the buffer is empty or the
     *                      "num" is out of bound.
     */
    bool peek(size_t num, T *&elem) {
        std::lock_guard<std::mutex> lock(mutex_);

        // Check if empty buffer
        if (count_ == 0) {
            return false;
        }

        // Check if index is out of bounds
        if (num >= count_) {
            return false;
        }

        auto peek_pos = (read_pos_ + num) % max_;
        elem = (buf_.get() + peek_pos);

        return true;
    };

    /**
     * @brief Gets the number of added elements in the buffer.
     *
     * @return              The number of added elements.
     */
    size_t count() const { return count_; };

    /**
     * @brief Gets the number of free elements in the buffer.
     *
     * @return              The number of free elements.
     */
    size_t space() const { return (max_ - count_); };

    /**
     * @brief Checks if the buffer is empty.
     *
     * @return              True if the buffer is empty otherwise false.
     */
    bool empty() const { return (count_ == 0); };

private:
    std::mutex mutex_;
    std::unique_ptr<T[]> buf_;  // Pointer to the buffer
    size_t write_pos_ = 0;      // Write pointer
    size_t read_pos_ = 0;       // Read pointer
    size_t count_ = 0;          // Number of added elements in the buffer
    const size_t max_;          // Max Number of elements in the buffer
};

/** @} */
