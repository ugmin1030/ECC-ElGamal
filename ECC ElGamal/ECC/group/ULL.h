#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../ECC.h"

namespace ECC {

	// 유한체 상 unsigned long long
	// 일반 unsigned long long 형식은 상수로 취급
	class ULL {

	private:

		unsigned long long value;

	public:

		// 기본 생성자
		ULL() noexcept :value(0) {}

		// 생성자
		ULL(unsigned long long v) noexcept : value(v % p) {}

		// 상수로 unsigned long long을 내는 함수
		unsigned long long getValue() const noexcept { return value; }

		// 문자열을 ULL 벡터로 인코딩 (암호화에 사용)
		friend std::vector<ULL> stringToULL(const std::string& str);

		// ULL 벡터를 문자열로 디코딩 (복호화에 사용)
		friend std::string ULLToString(std::vector<ULL> ull_vec);

		// ULL = unsigned long long
		inline ULL& operator=(const unsigned long long v) noexcept { this->value = v % p; return *this; }

		// -ULL
		inline ULL operator-() const noexcept { return ULL(p - value); }

		// ULL + ULL
		ULL operator+(const ULL& other) const noexcept;

		// ULL += ULL
		inline ULL& operator+=(const ULL& other) noexcept { *this = *this + other; return *this; }

		// ULL++
		inline ULL& operator++(int) noexcept {
			ULL result = *this; 
			this->value = (this->value + 1) % p; 
			return result; 
		}

		// ULL - ULL
		ULL operator-(const ULL& other) const noexcept;

		// ULL -= ULL
		inline ULL& operator-=(const ULL& other) noexcept { *this = *this - other; return *this; }

		// ULL * ULL
		ULL operator*(const ULL& other) const noexcept;

		// ULL *= ULL
		inline ULL& operator*=(const ULL& other) noexcept { *this = *this * other; return *this; }

		// ULL / ULL
		ULL operator/(const ULL& other) const;

		// ULL /= ULL
		inline ULL& operator/=(const ULL& other) { *this = *this / other; return *this; }

		// ULL ^ ULL
		ULL operator^(const ULL& other) const;

		// ULL ^ unsigned long long
		ULL operator^(const unsigned long long other) const;

		// ULL ^= ULL
		inline ULL& operator^=(const ULL& other) { *this = *this ^ other; return *this; }

		// ULL == ULL
		inline bool operator==(const ULL& other) const noexcept { return this->value == other.value; }

		// ULL != ULL
		inline bool operator!=(const ULL& other) const noexcept { return this->value != other.value; }

		// ULL < ULL
		inline bool operator<(const ULL& other) const noexcept { return this->value < other.value; }

		// ULL <= ULL
		inline bool operator<=(const ULL& other) const noexcept { return this->value <= other.value; }

		// ULL > ULL
		inline bool operator>(const ULL& other) const noexcept { return this->value > other.value; }

		// ULL >= ULL
		inline bool operator>=(const ULL& other) const noexcept { return this->value >= other.value; }

		// (bool) ULL
		explicit inline operator bool() const noexcept { return this->value != 0; }

		// 출력
		friend std::ostream& operator<<(std::ostream& os, const ULL& v) noexcept {
			return os << v.value;
		}

		// 입력
		friend std::istream& operator>>(std::istream& is, ULL& v) noexcept {
			unsigned long long input;
			is >> input;
			v = ULL(input);
			return is;
		}

		// ULL & ULL
		inline ULL operator&(const ULL& other) const noexcept { return ULL(this->value & other.value); }

		// ULL >> ULL
		inline ULL operator>>(const ULL other) const noexcept { return ULL(this->value >> other.value); }

		// ULL >>= ULL
		inline ULL& operator>>=(const ULL other) noexcept { *this = *this >> other; return *this; }
	};

	std::vector<ULL> stringToULL(const std::string& str);
	std::string ULLToString(std::vector<ULL> ull_vec);
}