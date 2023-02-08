#include <iostream>
#include <vector>




template <typename T>
class Deque {
private:
    static const size_t ARR_SIZE = 9;
    std::vector<T*> _external;
    size_t _deque_size;
    size_t _vector_size;
    size_t _first_array_index;
    size_t _last_array_index;
    size_t _first_element_index;
    size_t _last_element_index;

    void left_resize(std::vector<T*>& other_vector, size_t other_size, size_t new_size) {
        std::vector<T*> new_vector;
        for (size_t i = 0; i < new_size - other_size; ++i) {
            T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
            new_vector.push_back(new_arr);
        }
        for (size_t i = 0; i < other_size; ++i) {
            new_vector.push_back(other_vector[i]);
        }
        other_vector = new_vector;
    }

    void right_resize(std::vector<T*>& other_vector, size_t other_size, size_t new_size) {
        std::vector<T*> new_vector;
        for (size_t i = 0; i < other_size; ++i) {
            new_vector.push_back(other_vector[i]);
        }
        for (size_t i = 0; i < new_size - other_size; ++i) {
            T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
            new_vector.push_back(new_arr);
        }
        other_vector = new_vector;
    }
public:
    Deque(): _deque_size(0), _vector_size(1),
             _first_array_index(0), _last_array_index(0),
             _first_element_index(1), _last_element_index(0) {
        T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
    }

//    ~Deque() {
//        for(size_t i = _first_array_index; i <= _last_array_index; ++i) {
//            for(size_t j = 0; j < ARR_SIZE; ++j) {
//                (_external[i] + j)->~T();
//            }
//            delete[] reinterpret_cast<uint8_t*>(_external[i]);
//        }
//        _deque_size = 0;
//    }

    Deque(const Deque<T>& other): _deque_size(0), _vector_size(1),
                                  _first_array_index(0), _last_array_index(0),
                                  _first_element_index(1), _last_element_index(0) {
        T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
        for (size_t i = 0; i < other.size(); ++i) {
            try {
                push_back(other[i]);
            } catch (...) {
                for (size_t j = 0; j < i; ++j) {
                    pop_back();
                    throw;
                }
            }
        }
    }

    Deque& operator=(const Deque& other) {
        T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
        _deque_size = 0;
        _vector_size = 1;
        _first_array_index = 0;
        _last_array_index = 0;
        _first_element_index = 1;
        _last_element_index = 0;
        for (size_t i = 0; i < other.size(); ++i) {
            try {
                push_back(other[i]);
            }
            catch (...) {
                for (size_t j = 0; j < i; ++j) {
                    pop_back();
                }
                throw;
            }
        }
        return *this;
    }

    Deque(size_t number_of_elements): _deque_size(number_of_elements), _vector_size(number_of_elements / ARR_SIZE + 1),
                                      _first_array_index(0), _last_array_index((number_of_elements - 1) / ARR_SIZE),
                                      _first_element_index(0), _last_element_index((number_of_elements + ARR_SIZE - 1) % ARR_SIZE) {
        T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
        for (size_t i = 0; i < number_of_elements / ARR_SIZE; ++i) {
            T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
            _external.push_back(new_arr);
        }
        new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
    }

    Deque(size_t number_of_elements, const T& new_element): _deque_size(number_of_elements), _vector_size(number_of_elements / ARR_SIZE + 1),
                                                            _first_array_index(0), _last_array_index((number_of_elements - 1) / ARR_SIZE),
                                                            _first_element_index(0), _last_element_index((number_of_elements + ARR_SIZE - 1) % ARR_SIZE) {
        T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
        for (size_t i = 0; i < number_of_elements / ARR_SIZE; ++i) {
            T* new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
            _external.push_back(new_arr);
            for (size_t j = 0; j < ARR_SIZE; ++j) {
                try {
                    new (_external[i] + j) T(new_element);
                } catch (...) {
                    for (size_t t = 0; t < j; ++t) {
                        pop_back();
                    }
                    throw;
                }
            }
        }
        new_arr = reinterpret_cast<T*>(new uint8_t[ARR_SIZE * sizeof(T)]);
        _external.push_back(new_arr);
        for (size_t j = 0; j <= std::min(ARR_SIZE - 1, (number_of_elements + ARR_SIZE - 1) % ARR_SIZE); ++j) {
            new (_external[number_of_elements / ARR_SIZE] + j) T(new_element);
        }

    }

    size_t size() const {
        return _deque_size;
    }

    T& operator[](size_t index) {
        return *(_external[(index + _first_element_index) / ARR_SIZE + _first_array_index] + (index + _first_element_index) % ARR_SIZE);
    }

    const T& operator[](size_t index) const {
        return *(_external[(index + _first_element_index) / ARR_SIZE + _first_array_index] + (index + _first_element_index) % ARR_SIZE);
    }

    T& at(size_t index) {
        if (index > _deque_size - 1 or index < 0) {
            throw std::out_of_range("at out of range");
        }
        return *(_external[(index + _first_element_index) / ARR_SIZE + _first_array_index] + (index + _first_element_index) % ARR_SIZE);
    }

    const T& at(size_t index) const {
        if (index > _deque_size - 1 or index < 0) {
            throw std::out_of_range("at out of range");
        }
        return *(_external[(index + _first_element_index) / ARR_SIZE + _first_array_index] + (index + _first_element_index) % ARR_SIZE);
    }


    void push_back(const T& value) {
        if (_last_element_index == ARR_SIZE - 1 and _last_array_index == _external.size() - 1) {
            try {
                right_resize(_external, _external.size(), _external.size() + _external.size());
                new (_external[_last_array_index + 1]) T(value);
                _vector_size += _vector_size;
                _last_array_index += 1;
                _last_element_index = 0;
            } catch (...) {
                pop_back();
                throw;
            }
        } else if (_last_element_index == ARR_SIZE - 1) {
            try {
                _external[_last_array_index + 1] = new (_external[_last_array_index + 1]) T(value);
                _last_array_index += 1;
                _last_element_index = 0;
            } catch (...) {
                pop_back();
                throw;
            }
        } else {
            try {
                new (_external[_last_array_index] + _last_element_index + 1) T(value);
                _last_element_index += 1;
            } catch (...) {
                pop_back();
                throw;
            }
        }
        ++_deque_size;
    }

    void push_front(const T& value) {
        if (_first_element_index == 0 and _first_array_index == 0) {
            try {
                left_resize(_external, _external.size(), _external.size() + _external.size());
                _first_array_index += _vector_size;
                _last_array_index += _vector_size;
                new (_external[_first_array_index - 1] + ARR_SIZE - 1) T(value);
                _vector_size += _vector_size;
                _first_array_index -= 1;
                _first_element_index = ARR_SIZE - 1;
            } catch (...) {
                pop_front();
                throw;
            }
        } else if (_first_element_index == 0) {
            try {
                new (_external[_first_array_index - 1] + ARR_SIZE - 1) T(value);
                _first_array_index -= 1;
                _first_element_index = ARR_SIZE - 1;
            } catch (...) {
                pop_front();
                throw;
            }
        } else {
            try {
                new (_external[_first_array_index] + _first_element_index - 1) T(value);
                _first_element_index -= 1;
            } catch (...) {
                pop_front();
                throw;
            }
        }
        ++_deque_size;
    }

    void pop_back() {
        if (size() == 1) {
            _first_element_index = 1;
            _last_element_index = 0;
            _deque_size = 0;
        } else if (_last_element_index == 0) {
            _last_element_index = ARR_SIZE - 1;
            _last_array_index -= 1;
            --_deque_size;
        } else {
            _last_element_index -= 1;
            --_deque_size;
        }
    }


    void pop_front() {
        if (size() == 1) {
            _first_element_index = 1;
            _last_element_index = 0;
            _deque_size = 0;
        } else if (_first_element_index == ARR_SIZE - 1) {
            _first_element_index = 0;
            _first_array_index += 1;
            --_deque_size;
        } else {
            _first_element_index += 1;
            --_deque_size;
        }
    }


    template<bool C>
    struct Iterator {
    private:
        size_t _index_in_array; //deque_index + 1
        T** _array_pointer;
    public:
        using pointer = typename std::conditional_t<C, const T*, T*>;
        using reference = typename std::conditional_t<C, const T&, T&>;
        Iterator() = default;

        Iterator(const Iterator<false>& other): _index_in_array(other._index_in_array), _array_pointer(other._array_pointer) {}

        Iterator& operator=(const Iterator<false>& other) {
            _index_in_array = other._index_in_array;
            _array_pointer = other._array_pointer;
            return *this;
        }

        ~Iterator() = default;

        Iterator(T** other_pointer, size_t other_index) {
            _index_in_array = other_index;
            _array_pointer = other_pointer;
        }

        Iterator& operator++() {
            if (_index_in_array == ARR_SIZE - 1) {
                _index_in_array = 0;
                _array_pointer += 1;
            } else {
                _index_in_array += 1;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator answer(*this);
            ++(*this);
            return answer;
        }

        Iterator& operator--() {
            if (_index_in_array == 0) {
                _index_in_array = ARR_SIZE - 1;
                _array_pointer -= 1;
            } else {
                _index_in_array -= 1;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator answer(*this);
            --(*this);
            return answer;
        }

        Iterator operator+(int other) const {
            if (other > 0) {
                Iterator answer(*this);
                answer._array_pointer += other / ARR_SIZE;
                if ((other % ARR_SIZE) + _index_in_array > ARR_SIZE - 1) {
                    answer._array_pointer += 1;
                }
                answer._index_in_array = (_index_in_array + other) % ARR_SIZE;
                return answer;
            } else {
                Iterator answer(*this);
                answer._array_pointer -= (-other) / ARR_SIZE;
                if (answer._index_in_array < ((-other) % ARR_SIZE)) {
                    answer._array_pointer -= 1;
                }
                answer._index_in_array = (answer._index_in_array - ((-other) % ARR_SIZE) + ARR_SIZE) % ARR_SIZE;
                return answer;
            }
        }

        Iterator operator-(int other) const {
            Iterator answer(*this + (-other));
            return answer;
        }

        bool operator<(const Iterator& other) const {
            return (_array_pointer < other._array_pointer or (_array_pointer == other._array_pointer and _index_in_array < other._index_in_array));
        }

        bool operator==(const Iterator& other) const {
            return (_array_pointer == other._array_pointer and _index_in_array == other._index_in_array);
        }

        bool operator!=(const Iterator& other) const {
            return (_array_pointer != other._array_pointer or _index_in_array != other._index_in_array);
        }

        bool operator>(const Iterator& other) const {
            return (other < *this);
        }

        bool operator<=(const Iterator& other) const {
            return (*this < other or *this == other);
        }

        bool operator>=(const Iterator& other) const {
            return (*this > other or *this == other);
        }

        int operator-(const Iterator& other) const {
            return (_array_pointer - other._array_pointer) * ARR_SIZE + _index_in_array - other._index_in_array;
        }

        reference operator*() const {
            return *(*(_array_pointer) + _index_in_array);
        }
        pointer operator->() const {
            return *(_array_pointer) + _index_in_array;
        }

    };

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;
    using reverse_iterator = std::reverse_iterator<T>;
    using const_reverse_iterator = const std::reverse_iterator<T>;

    iterator begin() {
        return iterator(&(_external[_first_array_index]), _first_element_index);
    }

    const_iterator begin() const {
        return const_iterator(const_cast<T**>(&(_external[_first_array_index])), _first_element_index);
    }

    iterator end() {
        return iterator(const_cast<T**>(&(_external[_last_array_index])), _last_element_index + 1);
    }

    const_iterator end() const {
        return const_iterator(const_cast<T**>(&(_external[_last_array_index])), _last_element_index + 1);
    }

    const_iterator cbegin() const {
        return const_iterator(const_cast<T**>(&(_external[_first_array_index])), _first_element_index);
    }

    const_iterator cend() const {
        return const_iterator(const_cast<T**>(&(_external[_last_array_index])), _last_element_index + 1);
    }

    void insert(iterator it, const T& new_element) {
        push_back(new_element);
        iterator start = end();
        for (iterator it1 = end() - 1; it1 > it; --it1) {
            *it1 = *(it1 - 1);
        }
        *it = new_element;
    }

    void erase(iterator it) {
        for (iterator it1 = it; it1 < end() - 1; ++it1) {
            *it1 = *(it1 + 1);
        }
        pop_back();
    }

};
