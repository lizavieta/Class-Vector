#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<initializer_list>
#include<stdexcept>

template<typename T>
class Vector {
    public:
    class ConstIterator;
    class Iterator;
    using value_type = T; 
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type *;
    using iterator = Vector<T>::Iterator;
    using const_iterator = Vector<T>::ConstIterator;
    private:
        size_type sz;
        size_type max_sz;
        pointer values;
        void resize(size_type n) { 
            if (max_sz < n) {
                size_type new_max{max_sz*2+1};
                while (new_max < n)
                    new_max*=2;
                pointer new_values = new value_type[new_max];
                if (values != nullptr) {
                    for (size_type i{0}; i < sz; ++i)
                        new_values[i] = values[i];
                }
                delete[] values;
                values = new_values; 
                max_sz = new_max;
            }
        }
    public:
        Vector() : sz{0}, max_sz{0}, values{nullptr} {}
        Vector (size_type n) : sz{0}, max_sz{n}, values {n ? new value_type[n] : nullptr} {} 
        Vector (const Vector& v) : Vector(v.size()) {
            for (; sz < v.sz; ++sz) 
                values[sz] = v.values[sz];
        }
        Vector (std::initializer_list<value_type> l) : Vector(l.size()) { 
            for (const auto& value : l)
                values[sz++] = value;
        }
        ~Vector () {
            delete[] values; 
        }
        Vector<T>& operator= (const Vector& v) {
            if (this == &v) {
                return *this; 
            }
            delete[] values;
            sz = v.sz;
            max_sz = v.max_sz;
            values = new value_type[max_sz];
            for (size_type i {0}; i < sz; i++)
                values[i] = v.values[i];
            return *this;  
        }
        size_type size() const {
            return sz;
        }
        bool empty() const {
            return (sz == 0);
        }
        void clear() {
            delete[] values;
            sz = 0;
            values = new value_type[max_sz]; 
        }
        void reserve (size_type n) { 
            if (max_sz < n) {
                pointer new_values = new value_type[n];
                for (size_type i{0}; i < sz; ++i)
                    new_values[i] = values[i];
                delete[] values;
                max_sz = n;
                values = new_values;
            }
        }
        void shrink_to_fit() {
            max_sz = sz;
        }
        void push_back(value_type value) {
            resize(sz+1);
            values[sz++] = value;  
        }
        void pop_back() {
            if (empty()) 
                throw std::runtime_error("Empty Vector");
            values[sz-1] = 0;
            --sz;
        }
        reference operator[] (size_type index) {
            if (index < 0 || index >= sz)
                throw std::runtime_error("Out of bound");
            return values[index];
        }
        const_reference operator[] (size_type index) const {
            if (index < 0 || index >= sz)
                throw std::runtime_error("Out of bound");
            return values[index];
        }
        size_type capacity() const {
            return max_sz;
        }
        std::ostream& print(std::ostream& o) const {
            o << "[";
            bool first{true};
            for (size_type i{0}; i < sz; ++i) {
                if (first)
                    first = false;
                else
                    o << ", ";
                o << values[i];
            }
            return o << "]";
        }
        iterator begin() {
            iterator begin_it {values, values+sz, this};
            return begin_it;
        }
        iterator end() {
            iterator end_it {values+sz, values+sz, this};
            return end_it;
        }
        const_iterator begin() const {
            const_iterator begin_it {values, values+sz, this};
            return begin_it;
        }
        const_iterator end() const {
            const_iterator end_it {values+sz, values+sz, this};
            return end_it;
        }

    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        if (sz >= max_sz)
            reserve(max_sz * 2 + 1); 
        for (auto i{sz}; i-- > current;)
            values[i + 1] = values[i];
        values[current] = val;
        ++sz;
        return iterator{values + current, values+sz, this};
    }

    iterator erase(const_iterator pos) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) >= sz)
            throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        for (auto i{current}; i < sz - 1; ++i)
            values[i] = values[i + 1];
        --sz;
        return iterator{values + current, values+sz, this}; 
    }

    //class Iterator and class ConstIterator

    class Iterator {
    public:
      using value_type = Vector<T>::value_type;
      using reference = Vector<T>::reference;
      using pointer = Vector<T>::pointer;
      using difference_type = Vector<T>::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      pointer ptr; 
      pointer check;
      Vector<T>* obj;
    public:
      Iterator() : ptr{nullptr}, check{nullptr}, obj{nullptr} {}
      Iterator(pointer ptr, pointer check = nullptr, Vector<T>* obj = nullptr) : ptr(ptr), check(check), obj(obj) {} 
      reference operator*() const { 
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr)  
            return *ptr;
        else 
            throw std::runtime_error("Non-dereferencable");
      }
      pointer operator->() const { 
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr)  
          return ptr;
        else 
            throw std::runtime_error("Out of bound");
      }
      operator const_iterator() const {
        return const_iterator(ptr, check, obj);
      }
      friend bool operator== (const iterator& lop, const const_iterator& rop)  {
        const_iterator compare(lop);
        return (compare == rop);
      }

      friend bool operator!= (const iterator& lop, const const_iterator& rop)  {
        const_iterator compare(lop);
        return !(compare == rop);
      }
      iterator& operator++() { 
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr)   
            ++ptr;
        return *this;
      }
      iterator operator++(int) { 
        iterator old = *this;
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr) 
            ++ptr;
        return old;
      }

    };
    class ConstIterator {
    public:
      using value_type = Vector<T>::value_type;
      using reference = Vector<T>::const_reference;
      using pointer = Vector<T>::const_pointer;
      using difference_type = Vector<T>::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      const_pointer ptr; 
      const_pointer check;
      const Vector* obj; 
    public:
      ConstIterator() : ptr{nullptr}, check{nullptr}, obj{nullptr} {}
      ConstIterator(const_pointer ptr, const_pointer check = nullptr, const Vector<T>* obj = nullptr) : ptr(ptr), check(check), obj(obj) {}
      friend bool operator== (const const_iterator& lop, const const_iterator& rop)  {
        return (lop.check == rop.check && lop.ptr == rop.ptr);
      }
      friend bool operator!= (const const_iterator& lop, const const_iterator& rop)  {
        return !(lop.check == rop.check && lop.ptr == rop.ptr);
      }
      reference operator*() const { 
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr)  
            return *ptr;
        else 
            throw std::runtime_error("Non-dereferencable");
      }
      pointer operator->() const { 
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr)  
          return ptr;
        else 
            throw std::runtime_error("Out of bound");
      }
      const_iterator& operator++() { 
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr) 
            ++ptr;
        return *this;
      }
      const_iterator operator++(int) { 
        const_iterator old = *this;
        if (ptr != check && check != nullptr && check == obj->end().check && obj != nullptr)     
            ++ptr;
        return old;
      }
      friend Vector::difference_type operator-(const const_iterator& lop, const const_iterator& rop) {
        return lop.ptr - rop.ptr;
      }
    };

    friend std::ostream& operator<< (std::ostream& o, const Vector<T>& v) {
        return v.print(o);
    }
};
#endif