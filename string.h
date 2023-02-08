#include <iostream>
#include <cstring>


class String {
public:
    String(char symbol) {
        size_of_array = 16;
        string = new char[size_of_array];
        string[0] = symbol;
        size = 1;
    }

    String(const char* other_string) {
        size_t new_size = strlen(other_string);
        size = new_size;
        size_of_array = 2 * new_size;
        string = new char[size_of_array];
        memcpy(string, other_string, size);
    }

    String() {
        size = 0;
        size_of_array = 0;
        string = nullptr;
    }

    String(size_t n, char symbol) {
        size = n;
        size_of_array = 2 * n;
        string = new char[size_of_array];
        memset(string, symbol, size);
    }

    ~String() {
        delete[] string;
    }

    String(const String& other) {
        size = other.size;
        size_of_array = other.size_of_array;
        string = new char[size_of_array];
        const char* other_string = other.string;
        memcpy(string, other_string, size);
    }

    void swap(String &other_string) {
        std::swap(other_string.string, string);
        std::swap(other_string.size_of_array, size_of_array);
        std::swap(other_string.size, size);

    }

    void new_size_string(size_t new_size, const String&  other) {
        size_of_array = new_size;
        char* other_string = new char[size_of_array];
        memcpy(other_string, string, size);
        for (size_t i = 0; i < other.size; ++i) {
            other_string[i + size] = other[i];
        }

        delete[] string;
        string = new char[size_of_array];
        memcpy(string, other_string, size + other.size);
        delete[] other_string;
    }

    String& operator=(String other) {
        swap(other);
        return *this;
    }

    size_t length() const{
        return size;
    }

    bool operator==(const String& other) {
        bool ans = true;
        if (size != other.size) {
            ans = false;
            return ans;
        }
        for (size_t i = 0; i < size; ++i) {
            if (other.string[i] != string[i]) {
                ans = false;
                return ans;
            }
        }
        return ans;
    }

    char& front() {
        return string[0];
    }

    char& back() {
        return string[size - 1];
    }

    char front() const{
        return string[0];
    }

    char back() const{
        return string[size - 1];
    }

    void clear() {
        delete[] string;
        size = 0;
        size_of_array = 32;
        string = new char[size_of_array];
    }

    String& operator+=(const String& other) {
        if (size + other.size <= size_of_array) {
            for (size_t i = size; i < size + other.size; ++i) {
                string[i] = other[i - size];
            }
        } else {
            new_size_string(2 * (size + other.size), other);
        }
        size += other.size;
        return *this;
    }

    String& operator+=(const char other) {
        if (size == size_of_array) {
            new_size_string(2 * (size + 1), String(1, other));
        }
        else {
            string[size] = other;
        }
        size += 1;
        return *this;

    }

    char& operator[](size_t index) {
        return string[index];
    }

    char operator[](size_t index) const {
        return string[index];
    }

    void push_back(char other) {
        if (size == size_of_array) {
            new_size_string((1 + size) * 2, String(1, other));
        }
        else {
            string[size] = other;
        }
        size += 1;

    }

    void pop_back() {
        if (size * 4 - 4 < size_of_array - 1) {
            size_of_array /= 2;
            char* other_string = new char[size_of_array];
            memcpy(other_string, string, size - 1);
            delete[] string;
            string = new char[size_of_array];
            memcpy(string, other_string, size - 1);

        }
        else {
            string[size - 1] = '\0';
        }
        size -= 1;
    }

    bool empty() {
        return (size == 0);
    }

    size_t find(String other_string) const {
        for (size_t i = 0; i <= (size - other_string.size); ++i) {
            if (substr(i, other_string.size) == other_string) {
                return i;
            }
        }
        return size;
    }

    String substr(size_t start, size_t count) const {
        String new_str = String(0, '\0');
        for (size_t i = 0; i < count; ++i) {
            new_str += string[start + i];
        }
        return new_str;
    }

    size_t rfind(String other_string) const {
        for (int i = static_cast<int>(size - other_string.size); i >= 0; --i) {
            if (substr(i, other_string.size) == other_string) {
                return i;
            }}
        return size;
    }

private:
    friend std::istream& operator>>(std::istream &in, String &other_string);
    friend std::ostream& operator<<(std::ostream &out, const String &other_string);
    size_t size_of_array;
    char* string;
    size_t size;
};


std::ostream& operator<<(std::ostream &out, const String &other_string) {
    for (size_t i = 0; i < other_string.size; ++i) {
        char symbol = other_string.string[i];
        out << symbol;
    }
    return out;
}

std::istream& operator>>(std::istream &in, String &other_string) {
    other_string.clear();
    char new_symbol;
    while(in.get(new_symbol)) {
        if (new_symbol == '\n' or new_symbol == ' ' or new_symbol == '\t') break;
        other_string += new_symbol;
    }
    return in;
}

String operator+(const String& first, const String& second) {
    String answer(first);
    answer += second;
    return answer;
}

String operator+(const String& first, char second) {
    String answer(first);
    answer += second;
    return answer;
}

String operator+(char first, const String& second) {
    String answer = String(first);
    answer += second;
    return answer;
}
