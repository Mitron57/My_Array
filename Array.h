#pragma once

#include <iostream>
#include <concepts>
#include <memory>
#include <iterator>
#include <array>
#include <cstring>
#include <algorithm>

template<typename Type, std::size_t Size>
struct Array {
private:
    Type *ptr_array;
protected:
    auto self() const noexcept { return this; }
    
    using iterator = Type*;
    using c_iterator = const Type*;
public:
    Array() : ptr_array(new Type[Size]) {}
    
    Array(std::initializer_list<Type> list): ptr_array(new Type[list.size()]) {
        memcpy(begin(), list.begin(), list.size()*sizeof(Type));
    }
    
    Array(Array<Type, Size> &array) : ptr_array(new Type[Size]) {
        memcpy(begin(), array.begin(), Size * sizeof(Type));
    }

    Array(Array<Type, Size> &&array) noexcept: ptr_array(array.ptr_array) {
        array.ptr_array = nullptr;
    }
    
    virtual ~Array() noexcept {
        delete[] ptr_array;
    };

    friend std::ostream &operator<<(std::ostream &stream, Array<Type, Size> &array) {
        std::for_each(array.begin(), array.end(), [&stream](const auto& elem) {
            stream<<elem<<" ";
        });
        return stream;
    }

    Type &operator[](std::int32_t index) const noexcept {
        if (index >= 0 && index <= Size) {
            return *(Array::ptr_array + index);
        } else if (std::abs(index) <= Size) {
            return *(Array::ptr_array + (Size + index));
        } else {
            throw std::length_error("Index is out of range.");
        }
    }

    Array &operator=(Array<Type, Size> const &array) {
        if (this == array.self())
            return *this;
        memcpy(this->ptr_array, array.ptr_array, Size * sizeof(Type));
        return *this;
    }

    Array &operator=(Array<Type, Size> &&array) noexcept {
        if (this == array.self()) {
            return *this;
        }
        this->ptr_array = array.ptr_array;
        array.ptr_array = nullptr;
        return *this;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return Size;
    }

    
    constexpr iterator begin() noexcept { return iterator(ptr_array); }
    constexpr c_iterator begin() const noexcept { return c_iterator(ptr_array); }
    
    constexpr iterator end() noexcept { return iterator(ptr_array + Size); }
    constexpr c_iterator end() const noexcept { return c_iterator(ptr_array + Size); }
};