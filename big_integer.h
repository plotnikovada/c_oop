#include <iostream>
#include <vector>
#include <deque>
#include <map>


class BigInteger {
public:
    int get_sign() const{
        return sign;
    }

    BigInteger() {
        sign = 1;
        main_sequence = {1};
        size = 1;
    }

    BigInteger(std::deque<int> &sequence) {
        if (sequence.empty()) {
            size = 1;
            main_sequence = {0};
            sign = 1;
        }
        else {
            sign = 1;
            main_sequence = sequence;
            size = main_sequence.size();
            while (size > 1 and main_sequence.back() == 0) {
                main_sequence.pop_back();
                size -= 1;
            }
            if (size == 1 and main_sequence[0] == 0) {
                sign = 0;
            }
        }

    }

    BigInteger(BigInteger &other) {
        main_sequence = other.main_sequence;
        size = other.size;
        sign = other.sign;
    }

    BigInteger(int other_sign, const BigInteger& other) {
        main_sequence = other.main_sequence;
        sign = other_sign;
        size = other.size;
    }

    BigInteger(std::string string1) {
        clear();
        sign = 1;
        size = 0;
        int i;
        if (string1[0] == '-') {
            sign = -1;
        }
        for (i = string1.size() % radix_n; i < static_cast<int>(string1.size()) - 3; i += radix_n) {
            int num1 = 0;
            for (int j = 0; j < radix_n; ++j) {
                num1 *= 10;
                num1 += sym_list[string1[i + j]];
            }
            size += 1;
            main_sequence.push_front(num1);
        }

        int num1 = 0;
        if (string1.size() - (string1[0] == '-') > size * radix_n) {
            for (i = string1[0] == '-'; i < static_cast<int>(string1.size()) % radix_n; ++i) {
                num1  *= 10;
                num1 += sym_list[string1[i]];
            }
            main_sequence.push_back(num1);
            size += 1;
        }

        while (main_sequence.back() == 0 and size > 1) {
            main_sequence.pop_back();
            size -= 1;
        }

        if (main_sequence[0] == 0 and size == 1) {
            sign = 0;}
    }

    BigInteger(int number) {
        BigInteger num = BigInteger(std::to_string(number));
        sign = num.sign;
        main_sequence = num.main_sequence;
        size = num.size;
    }


    BigInteger(const BigInteger& other) {
        sign = other.sign;
        size = other.size;
        main_sequence = other.main_sequence;
    }

    explicit operator bool() const {
        if (sign == 0) {
            return false;
        }
        return true;
    }

    void swap(BigInteger &number) {
        std::swap(number.main_sequence, main_sequence);
        std::swap(number.sign, sign);
        std::swap(number.size, size);
    }

    BigInteger &operator=(BigInteger other) {
        swap(other);
        return *this;
    }

    std::string toString() {
        std::string new_string;
        if (sign == -1) {
            new_string += '-';
        }
        for (int i = static_cast<int>(size - 1); i >= 0; --i) {
            std::string string1 = std::to_string(main_sequence[i]);
            while ((i < static_cast<int>(size - 1)) and static_cast<int>(string1.size()) < radix_n) {
                string1 = "0" + string1;
            }
            new_string.append(string1);
        }
        return new_string;
    }

    std::string toString() const{
        std::string new_string;
        if (sign == -1) {
            new_string += '-';
        }
        for (int i = static_cast<int>(size - 1); i >= 0; --i) {
            std::string string1 = std::to_string(main_sequence[i]);
            while ((i < static_cast<int>(size - 1)) and static_cast<int>(string1.size()) < radix_n) {
                string1 = "0" + string1;
            }
            new_string.append(string1);
        }
        return new_string;
    }

    void clear() {
        main_sequence.clear();
        size = 0;
        sign = 0;
    }

    BigInteger& operator+=(const BigInteger& other) {
        if (sign == 0) {
            *this = other;
            return *this;
        }
        if (other.sign == 0) {
            return *this;
        }
        BigInteger other_sign_biginteger = BigInteger(-other.get_sign(), other);
        *this -= other_sign_biginteger;
        return *this;
    }

    BigInteger abs() const{
        int t_sign = sign * sign;
        BigInteger answer = BigInteger(t_sign, *this);
        return answer;
    }

    BigInteger& operator-=(const BigInteger& other) {
        int x;
        if (sign == 0) {
            *this = other;
            sign = -other.sign;
            return *this;
        }
        if (other.sign == 0) {
            return *this;
        }
        if (-sign == other.sign) {
            int div = 0;
            if (size < other.size) {
                for (int i = 0; i < static_cast<int>(other.size) - static_cast<int>(size); ++i) {
                    main_sequence.push_back(0);
                }
                for (int i = 0; i < static_cast<int>(other.size); ++i) {
                    x = div + main_sequence[i] + other.main_sequence[i];
                    main_sequence[i] = x % radix;
                    div = x / radix;
                }
                if (div == 1) {
                    main_sequence.push_back(1);
                }
                size = main_sequence.size();
                return *this;
            }
            else {
                for (int i = 0; i < static_cast<int>(other.size); ++i) {
                    x = div + main_sequence[i] + other.main_sequence[i];
                    main_sequence[i] = x % radix;
                    div = x / radix;
                }
                for (int i = static_cast<int>(other.size); i < static_cast<int>(other.size); ++i) {
                    x = div + main_sequence[i];
                    main_sequence[i] = x % radix;
                    div = x / radix;
                }
                if (div == 1) {
                    main_sequence.push_back(1);
                }
                size = main_sequence.size();
                return *this;
            }
        }
        if (abs() < other.abs()) {
            int div = 0;
            for (int i = 0; i < static_cast<int>(size); ++i) {
                x = div - main_sequence[i] + other.main_sequence[i];
                if (x < 0) {
                    div = -1;
                    main_sequence[i] = x + radix;
                }
                else {
                    div = 0;
                    main_sequence[i] = x;
                }
            }
            for (int i = static_cast<int>(size); i < static_cast<int>(other.size); ++i) {
                x = div + other.main_sequence[i];
                if (x < 0) {
                    div = -1;
                    main_sequence.push_back(x + radix);
                }
                else {
                    div = 0;
                    main_sequence.push_back(x);
                }
            }
            while (main_sequence.back() == 0) {
                main_sequence.pop_back();
            }
            size = main_sequence.size();
            sign *= -1;
            return *this;
        }
        if (other.abs() < this->abs()) {
            int div = 0;
            for (int i = 0; i < static_cast<int>(other.size); ++i) {
                x = div + main_sequence[i] - other.main_sequence[i];
                if (x < 0) {
                    div = -1;
                    main_sequence[i] = x + radix;
                }
                else {
                    div = 0;
                    main_sequence[i] = x;
                }
            }
            for (int i = static_cast<int>(other.size); i < static_cast<int>(size); ++i) {
                x = div + main_sequence[i];
                if (x < 0) {
                    div = -1;
                    main_sequence[i] = x + radix;
                }
                else {
                    div = 0;
                    main_sequence[i] = x;
                }
            }
            while (main_sequence.back() == 0) {
                main_sequence.pop_back();
            }
            size = main_sequence.size();
            return *this;
        }
        main_sequence = {0};
        size = 1;
        sign = 0;
        return *this;
    }

    explicit operator int() const {
        int answer = 0;
        for (int i = std::min(static_cast<int>(size), radix) - 1; i >= 0; --i) {
            answer *= radix;
            answer += main_sequence[i];
        }
        if (sign == -1) {
            answer = answer * -1;
        }
        return answer;
    }

    BigInteger& operator++() {
        *this += 1;
        return *this;
    }

    BigInteger operator++(int) {
        const BigInteger answer = *this;
        *this += 1;
        return answer;
    }

    BigInteger& operator--() {
        *this -= 1;
        return *this;
    }

    BigInteger operator--(int) {
        BigInteger answer = BigInteger(*this);
        *this -= 1;
        return answer;
    }

    BigInteger operator*=(const BigInteger& other) {
        std::deque<int> first;
        for (int i = 0; i < static_cast<int>(size + other.size)+ 2; ++i) {
            first.push_back(0);
        }
        if (sign == 0 or other.sign == 0) {
            main_sequence = {0};
            size = 1;
            sign = 0;
            return *this;
        }
        int x;
        int div = 0;
        for (int i = 0; i < static_cast<int>(size + 1); ++i) {
            for (int j = 0; j < static_cast<int>(other.size + 1); ++j) {
                if (i == static_cast<int>(size) or j == static_cast<int>(other.size)) {
                    x = div + first[i + j];
                    first[i + j] = x % radix;
                    div = x / radix;
                    continue;
                }
                x = main_sequence[i] * other.main_sequence[j] + div + first[i + j];
                first[i + j] = x % radix;
                div = x / radix;
            }
        }
        main_sequence = first;

        while (main_sequence.back() == 0 and main_sequence.size() > 1) {
            main_sequence.pop_back();
        }
        size = main_sequence.size();
        sign *= other.sign;
        return *this;
    }

    BigInteger operator/=(const BigInteger& other) {
        if(other==BigInteger(0)){
        }
        if (sign == 0) {
            return *this;
        }
        if (this->abs() < other.abs()) {
            main_sequence = {0};
            size = 1;
            sign = 0;
            return *this;
        }
        std::deque<int> answer;
        BigInteger number;
        std::deque<int> sequence;
        sequence.push_back(0);
        for (int j = static_cast<int>(size - 1); j > static_cast<int>(size) - static_cast<int>(other.size); --j) {
            sequence.push_front(main_sequence[j]);
        }
        if (sequence.empty()) {
            number = 0;
        }
        else {
            number = BigInteger(sequence);
        }
        BigInteger other_abs = other.abs();
        for (int i = 0; i < static_cast<int>(size) - static_cast<int>(other.size) + 1; ++i) {
            number *= radix;
            number += main_sequence[int(size) - static_cast<int>(other.size) - i];
            int digit1 = 0;
            int digit2 = radix;
            int digit_m;
            while (digit2 - digit1 > 1) {
                digit_m = (digit1 + digit2) / 2;
                if (number < digit_m *  other.abs()) {
                    digit2 = digit_m;
                }
                else {
                    digit1 = digit_m;
                }
            }
            number -= digit1 * other_abs;
            if (digit2 * other_abs <= number) {
                digit1 = digit2;
            }
            answer.push_front(digit1);
        }
        int other_sign = other.sign * sign;
        *this = BigInteger(answer);
        sign = other_sign;
        return *this;
    }

    BigInteger operator%= (const BigInteger &other) {
        BigInteger answer = BigInteger(*this);
        answer /= other;
        *this -= other * answer;
        return *this;
    }

private:
    int radix = 10000;
    int radix_n = 4;
    std::map<char, int> sym_list = {{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
                                    {'5', 5}, {'6', 6}, {'7' ,7}, {'8', 8}, {'9', 9}};
    int sign;
    std::deque<int> main_sequence;
    size_t size;
    friend bool operator>(const BigInteger& first, const BigInteger& second);
    friend bool operator<(const BigInteger& first, const BigInteger& second);
    friend BigInteger operator+(const BigInteger& first, const BigInteger& second);
    friend BigInteger operator-(const BigInteger& first, const BigInteger& second);
    friend BigInteger operator-(const BigInteger& first);
    friend BigInteger operator-(BigInteger&first);
    friend BigInteger operator*(const BigInteger& first, const BigInteger& second);
    friend bool operator>=(const BigInteger& first, const BigInteger& second);
    friend bool operator<=(const BigInteger& first, const BigInteger& second);
    friend bool operator==(const BigInteger& first, const BigInteger& second);
    friend bool operator!=(const BigInteger& first, const BigInteger& second);
    friend std::ostream& operator<<(std::ostream &out, const BigInteger &number);
    friend std::istream& operator>>(std::istream &in, BigInteger &number);
};

std::ostream& operator<<(std::ostream &out, const BigInteger &number) {
    out << number.toString();
    return out;
}

std::istream& operator>>(std::istream &in, BigInteger &number) {
    std::string string1;
    in >> string1;
    number = BigInteger(string1);
    return in;
}

bool operator<(const BigInteger& first, const BigInteger& second) {
    if (first.sign < second.sign) {
        return true;
    }
    if (first.sign > second.sign) {
        return false;
    }
    if (first.sign == 0) {
        return false;
    }
    if (first.sign == 1) {
        if (first.size < second.size) {
            return true;
        }
        if (first.size > second.size) {
            return false;
        }
        for (size_t i = first.size; i >= 1; --i) {
            if (first.main_sequence[i - 1] < second.main_sequence[i - 1]) {
                return true;
            }
            if (first.main_sequence[i - 1] > second.main_sequence[i - 1]) {
                return false;
            }
        }
        return false;
    }
    if (first.sign == -1) {
        return (not (first.abs() < second.abs()));
    }
    return false;
}

bool operator==(const BigInteger& first, const BigInteger& second) {
    return !(first < second) && !(second < first);
}


bool operator>=(const BigInteger& first, const BigInteger& second) {
    return ! (first < second);
}

bool operator<=(const BigInteger& first, const BigInteger& second) {
    return ! (first > second);
}

bool operator!=(const BigInteger& first, const BigInteger& second) {
    return  (first < second or second < first);
}

bool operator>(const BigInteger& first, const BigInteger& second) {
    return second < first;
}

BigInteger operator-(BigInteger &first) {
    BigInteger answer = BigInteger((-first.sign), first);
    return answer;
}
BigInteger operator-(const BigInteger &first) {
    BigInteger answer = BigInteger((-first.sign), first);
    return answer;
}

BigInteger operator+(const BigInteger& first, const BigInteger& second) {
    BigInteger answer(first);
    answer += second;
    return answer;
}

BigInteger operator-(const BigInteger& first, const BigInteger& second) {
    BigInteger answer(first);
    answer -= second;
    return answer;
}

BigInteger operator/(const BigInteger& first, const BigInteger& second) {
    BigInteger answer(first);
    answer /= second;
    return answer;
}

BigInteger operator%(const BigInteger& first, const BigInteger& second) {
    BigInteger answer(first);
    answer %= second;
    return answer;
}

BigInteger operator*(const BigInteger& first, const BigInteger& second) {
    BigInteger answer(first);
    answer *= second;
    return answer;
}

class Rational {
public:
    Rational(const Rational& other) {
        numerator = other.numerator;
        denominator = other.denominator;
    }

    void swap(Rational &number) {
        std::swap(number.numerator, numerator);
        std::swap(number.denominator, denominator);
    }

    Rational &operator=(Rational other) {
        swap(other);
        return *this;
    }

    Rational() {
        numerator = BigInteger(1);
        denominator = BigInteger(1);
    }

    Rational(int other) {
        numerator = BigInteger(other);
        denominator = BigInteger(1);
    }

    Rational(BigInteger other) {
        numerator = BigInteger(other);
        denominator = BigInteger(1);
    }

    void Irreducible_fraction() {
        BigInteger GSD1 = denominator.abs();
        BigInteger GSD2 = numerator.abs();
        while (GSD1 and GSD2) {
            if (GSD1 > GSD2) {
                GSD1 %= GSD2;
            }
            else {
                GSD2 %= GSD1;
            }
        }
        if (GSD1 > GSD2) {
            denominator /= GSD1;
            numerator /= GSD1;
        }
        else {
            denominator /= GSD2;
            numerator /= GSD2;
        }
    }

    Rational& operator+=(const Rational &other) {
        numerator = numerator * other.denominator + other.numerator * denominator;
        denominator = denominator * other.denominator;
        Irreducible_fraction();
        return *this;
    }

    Rational& operator-=(const Rational &other) {
        numerator = numerator * other.denominator - other.numerator * denominator;
        denominator = denominator * other.denominator;
        Irreducible_fraction();
        return *this;
    }

    Rational& operator*=(const Rational &other) {
        numerator = numerator * other.numerator;
        denominator = denominator * other.denominator;
        if (denominator.get_sign() == -1) {
            denominator = -denominator;
            numerator = -numerator;
        }
        Irreducible_fraction();
        return *this;
    }

    Rational& operator/=(const Rational &other) {
        if (other.numerator == BigInteger(0)) {
            std::cerr<< "attention";
        }
        if(other.numerator == 0){
            std::cerr<<"attention";
        }
        numerator = numerator * other.denominator;
        denominator = other.numerator * denominator;
        if (denominator.get_sign() == -1) {
            denominator = -denominator;
            numerator = -numerator;
        }
        Irreducible_fraction();
        return *this;
    }

    std::string toString() {
        std::string  ans = numerator.toString();
        if (denominator != 1) {
            ans += '/' + denominator.toString();
        }
        return ans;
    }

    std::string asDecimal(int size_t_precision=0) {
        std::string answer = "";
        BigInteger number = numerator.abs();
        if (numerator.get_sign() == -1) {
            answer = "-";
            answer += (numerator / denominator).toString();
        }
        else {
            answer += (numerator / denominator).toString();
        }
        number %= denominator;
        number *= 10;
        if (size_t_precision) {
            answer += '.';
        }
        for (int i = 0; i < size_t_precision; ++i) {
            answer += sym_list[static_cast<int>(number / denominator)];
            number %= denominator;
            number *= 10;
        }
        return answer;
    }

    explicit operator double () const {
        double answer = 1;
        return answer * static_cast<int>(numerator) / std::max(1, static_cast<int>(denominator));
    }

private:
    std::map<int, char> sym_list = {{0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'},
                                    {5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'}};
    BigInteger denominator;
    BigInteger numerator;
    friend Rational operator-(const Rational& first);
    friend Rational operator-(Rational&first);
    friend bool operator<(const Rational& first, const Rational& second);

};


Rational operator+(const Rational& first, const Rational& second) {
    Rational answer = Rational(first);
    answer += second;
    return answer; // RVO (Return Value Optimization)
}

Rational operator-(const Rational& first, const Rational& second) {
    Rational answer(first);
    answer -= second;
    return answer; // RVO (Return Value Optimization)
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational answer(first);
    answer /= second;
    return answer; // RVO (Return Value Optimization)
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational answer(first);
    answer *= second;
    return answer;
}

Rational operator-(const Rational &first) {
    Rational answer = Rational((-first.numerator)) / first.denominator;
    return answer;
}

Rational operator-(Rational &first) {
    Rational answer = Rational(-first.numerator) / first.denominator;
    return answer;
}

bool operator<(const Rational &first, const Rational &second) {
    return ((first.numerator * second.denominator) < (second.numerator * first.denominator));
}

bool operator>(const Rational &first, const Rational &second) {
    return second < first;
}

bool operator<=(const Rational &first, const Rational &second) {
    return (!(first > second));
}

bool operator>=(const Rational &first, const Rational &second) {
    return (!(first < second));
}

bool operator==(const Rational &first, const Rational &second) {
    return (!(first < second or second < first));
}

bool operator!=(const Rational &first, const Rational &second) {
    return ((first < second or second < first));
}


