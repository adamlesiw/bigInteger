A class to represent arbitrarily large integers
*/
//include statements to for compiler
#include "bigint.h"
#include <iostream>
#include <ostream>
#include <fstream>

typedef unsigned short vec_bin;

//strips the zeros from before the big ints
void bigint::strip_zeros() {
    while(!number.empty() && number.back() == 0) number.pop_back();
    if(number.empty()) number.push_back(0);
    return;
}

bigint::bigint(){
    number.clear();
    number.push_back(0);
}

//sets the number equal to "that"
bigint::bigint(const std::vector<vec_bin> &that) {
    number.clear();
    number = that;
    this->strip_zeros();
}

//mods the numbs integer by 10 to yeild a bigint to the integer provided
bigint::bigint(unsigned long long i) {
    number.clear();
    do {
        number.push_back((vec_bin)(i % 10));
    }while(i /= 10);

}

//creates a big int equal to the value of the given string
bigint::bigint(std::string str) {
    number.clear();

    for (int i = str.length(); i > 0; --i){
        number.push_back((vec_bin)(str[i-1])-'0');

    }
    this->strip_zeros();

}

//makes a bigint based on the string given in the infile
bigint::bigint(std::ifstream &infile) {
    if(!infile.fail()){
        std::string line, result;
        while(getline(infile,line)) result += line;
        *this = result;
    }
    else std::cerr<<"could not open the file" <<std::endl;

}

//sets number variable equal to that parameter from getNumber function
bigint::bigint(const bigint &that) {
    number.clear();
    number = that.getNumber();
}


/* Number Access
 *
 * */
 //returns the number from the getNumber function
const std::vector<vec_bin> &bigint::getNumber() const {
    return number;

}

//returns the index in number from the size_t vecbin
vec_bin bigint::operator[](size_t index) const {
    return number[index];

}


/** Comparators
 *
 * */

//boolean operators to yield digits when it is not equal
bool bigint::operator==(const bigint &that) const {
    return number == that.number;
}

//boolean operator to yield ints when it is equal
bool bigint::operator!=(const bigint &that) const {
    return !(that == *this);
}

//operator to return number pointing from this
bool bigint::operator<(const bigint &that) const {
    if (number.size() < that.number.size()){
        return true;
    }
    else if(that.number.size() < number.size()){
        return false;
    }
    else{
        for(size_t i = number.size(); i > 0; i--){
            if(number[i - 1] < that[i - 1]){
                return true;
            }
            else if(that[i - 1] < number[i - 1]){
                return false;
            }
        }
    }
    return false;
}

//yields ints greater than or equal to the number pointed from that
bool bigint::operator<=(const bigint &that) const {
    return !(that < *this);

}

//yields ints less than pointer "this"
bool bigint::operator>(const bigint &that) const {
    return (that < *this);

}

//yields ints less than or equal to number pointed from this
bool bigint::operator>=(const bigint &that) const {
    return !(*this < that);

}


/** Addition
 *
 * */

//returns a bigint that is the sum of this and that
bigint bigint::add(const bigint &that) const {

    size_t n_this = number.size();
    size_t n_that = that.number.size();
    size_t n = n_this > n_that ? n_this: n_that;

    std::vector<vec_bin> answer;

    //variables for addition and carrying
    int x = 0;
    int carrier = 0;

    //carries excess numbers
    for(size_t i = 0; i < n; ++i){

        x = (i < n_this ? number[i]: 0) + (i < n_that ? that[i]: 0) + carrier;
        answer.push_back(x % 10);
        carrier = x/10;
    }

    if(carrier > 0){
        answer.push_back(carrier);
    }

    //returns the answer of bigint
    return bigint(answer);

}

//addition operators
bigint bigint::operator+(const bigint &that) const {
    return this -> add(that);
}

bigint &bigint::operator+=(const bigint &that) {
    *this = this -> add(that);
    return *this;
}

bigint &bigint::operator++() {
    return *this += 1;
}

bigint bigint::operator++(int) {
    bigint x = *this;
    ++*this;
    return x;
}


/** Subtraction
 *
 * */

//subtracts two bigints by taking the difference of the smaller one from the bigger one
bigint bigint::subtract(const bigint &that) const {

    int minus = 0;
    int x = 0;

    size_t n_this = number.size();
    size_t n_that = that.number.size();
    std::vector<vec_bin> answer;

    //ternary operator for subtraction
    for(size_t i = 0; i < n_this; i++){
        x = number[i] - (i < n_that ? that[i]: 0) - minus;

        //if the subtraction is negative, borrow a number
        if(x < 0){
            x += 10;
            minus = 1;
        }
        else{
            minus = 0;
        }
        //pushes back the result
        answer.push_back(x);
    }
    return bigint(answer);

}

//operators for subtraction
bigint bigint::operator-(const bigint &that) const {
    return this -> subtract(that);
}

bigint &bigint::operator-=(const bigint &that) {
    *this = this -> subtract(that);
    return *this;
}

bigint &bigint::operator--() {
    return *this -= 1;

}

bigint bigint::operator--(int) {
    bigint x = *this;
    -- *this;
    return x;
}


/** Multiplication
 *
 * */

//multiplies two bigints and returns the product
bigint bigint::multiply(const bigint &that) const {
    // std::vector<bigint> answer = *this;
    // bigint x = that;

    if(*this == 0 || that == 0){
        return 0;
    }

    //group addition, += this for multiplication purposes
    bigint answer = number;
    bigint x = that.getNumber();
    while(--x != 0){
        answer += *this;
    }
    return answer;

}

//operators for multiplication
bigint bigint::operator*(const bigint &that) const {
    bigint x = this -> multiply(that);
    return x;
}

bigint &bigint::operator*=(const bigint &that) {
    *this = this -> multiply(that);
    return *this;
}


/** Division
 *
 * */

//divides two bigints and returns a quotient
bigint bigint::divide(const bigint &that) const {

    //variables for division
    bigint answer = 0;
    bigint x = *this;

    //group subtraction
    while(x >= that){
        x -= that;
        ++ answer;
    }
    return answer;
}

//operators for division
bigint bigint::operator/(const bigint &that) const {
    bigint x = this->divide(that);
    return x;

}

bigint &bigint::operator/=(const bigint &that) {
    *this = this->divide(that);
    return *this;

}


// /** Modulo
//  *
//  * */

//mods two bigints
bigint bigint::mod(const bigint &that) const {

    bigint answer = *this;

    //mods answer by that for the result
    while(answer >= that){
        answer -= that;
    }
    return answer;
}

//operators for modulus
bigint bigint::operator%(const bigint &that) const {
    bigint x = this->mod(that);
    return x;
}

 bigint &bigint::operator%=(const bigint &that) {
     *this = this->mod(that);
     return *this;
 }


/** Power
 *
 * */
 //power function for two bigints
 bigint bigint::pow(unsigned long long n) {

    //if the power is 0, it is equal to 1 since anything to the 0th power is 1
     if( n == 0){
         return 1;
     }

    //variable for *this
     bigint answer = *this;

    //answer times equals for repeated multiplication
     while(n-- > 1){
         answer *= *this;
     }
     return answer;
}


// /** Display methods
//  *
//  * */

//takes a refernce to an output stream and a bigint and returns a reference to an output stream
std::ostream &operator<<(std::ostream &os, const bigint &bigint1) {
    return os << bigint1.to_string(true);

}

//makes a string version of the bigint, making commas where necessary
std::string bigint::to_string(bool commas) const {
    std::string result;
    result += (static_cast<char>(number[number.size() -1] + '0'));

    for (int i = number.size() - 2; i >= 0; --i){
        if(i % 3 == 2 && commas) result += ',';
        result += (static_cast<char>(number[i] + '0'));
    }

    return result;
}

//converts the big int into scientific notation
std::string bigint::scientific(unsigned int decimal_points) const {
    std::string result;
    int power = (number.size()-1);
    result += char('0' + number[power]);
    result += ".";
    for(int i = 0; i <(int)(decimal_points); i++){
        if(power -i - 1 < 0){
            result += '0';
        }
        else{
            result += char('0' + number[power - i - 1]);
        }
    }
    result += 'E';
    std::string x = std::to_string(power);
    result += x;
    return result;

}

//writes the bigint to a file
void bigint::to_file(std::ofstream &outfile, unsigned int wrap) {
    std::string characters = this->to_string();

    for(int i = 0; i < (int)characters.length(); ++i){
        outfile << characters[i];
        if(i % wrap == wrap - 1) outfile << std::endl;
    }
    return;
}
