#pragma once
#include <iostream>
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
		inline ULL operator^(const unsigned long long other) const { 

			// other가 -1이면 역원으로 처리.
			if (other == -1) {
				
				// this가 0이면 예외 처리
				if (this->value == 0) {
					std::cout << "error: 0의 역원은 정의되지 않았습니다." << std::endl;
					throw std::invalid_argument("error: 0의 역원은 정의되지 않았습니다.");
				}

				return *this ^ ULL(p - 2);
			}

			else{

				// 지수는 최대 p - 2
				return *this ^ ULL(other > p - 2 ? (other - (p - 2)) : other);
			}
		}

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
}