#pragma once
namespace ECC {

	// 유한체 상 unsigned long long
	// 일반 unsigned long long 형식은 상수로 취급
	class ULL {

	private:

		unsigned long long value;

	public:

		// 기본 생성자
		ULL(){}

		// 생성자
		ULL(unsigned long long v) : value(v % p) {}

		// ULL = unsigned long long
		// TODO: 정상 작동하는지 테스트 필요
		inline ULL& operator=(const unsigned long long v) { value = v % p; return *this; }

		// -ULL
		inline ULL operator-() const { return ULL(p - value); }

		// ULL + ULL
		ULL operator+(const ULL& other) const;

		// ULL + unsigned long long
		inline ULL operator+(const unsigned long long other) const { return *this + ULL(other); }

		// unsigned long long + ULL
		inline friend ULL operator+(unsigned long long lhs, const ULL& rhs) { return ULL(lhs) + rhs; }

		// ULL += ULL
		inline ULL& operator+=(const ULL& other) { *this = *this + other; return *this; }

		// ULL += unsigned long long
		inline ULL& operator+=(const unsigned long long other) { *this = *this + ULL(other); return *this; }

		// ULL - ULL
		ULL operator-(const ULL& other) const;

		// ULL - unsigned long long
		inline ULL operator-(const unsigned long long other) const { return *this - ULL(other); }

		// unsigned long long - ULL
		inline friend ULL operator-(unsigned long long lhs, const ULL& rhs) { return ULL(lhs) - rhs; }

		// ULL -= ULL
		inline ULL& operator-=(const ULL& other) { *this = *this - other; return *this; }

		// ULL -= unsigned long long
		inline ULL& operator-=(const unsigned long long other) { *this = *this - ULL(other); return *this; }

		// ULL * ULL
		ULL operator*(const ULL& other) const;

		// ULL * unsigned long long
		inline ULL operator*(const unsigned long long other) const { return *this * ULL(other); }

		// unsigned long long * ULL
		inline friend ULL operator*(unsigned long long lhs, const ULL& rhs) { return ULL(lhs) * rhs; }

		// ULL *= ULL
		inline ULL& operator*=(const ULL& other) { *this = *this * other; return *this; }

		// ULL *= unsigned long long
		inline ULL& operator*=(const unsigned long long other) { *this = *this * ULL(other); return *this; }

		// ULL / ULL
		ULL operator/(const ULL& other) const;

		// ULL / unsigned long long
		inline ULL operator/(const unsigned long long other) const { return *this / ULL(other); }

		// unsigned long long / ULL
		inline friend ULL operator/(unsigned long long lhs, const ULL& rhs) { return ULL(lhs) / rhs; }

		// ULL /= ULL
		inline ULL& operator/=(const ULL& other) { *this = *this / other; return *this; }
		
		// ULL /= unsigned long long
		inline ULL& operator/=(const unsigned long long other) { *this = *this / ULL(other); return *this; }

		// ULL ^ ULL
		ULL operator^(const ULL& other) const;

		// ULL ^ unsigned long long
		inline ULL operator^(const unsigned long long other) const { 

			// other가 -1이면 역원으로 처리. -1 외의 음수 처리 연구 필요.
			return *this ^ ULL(other != -1 ? p - 2 : other);
		}

		// unsigned long long ^ ULL
		inline friend ULL operator^(unsigned long long lhs, const ULL& rhs) { return ULL(lhs) ^ rhs; }

		// ULL ^= ULL
		inline ULL& operator^=(const ULL& other) { *this = *this ^ other; return *this; }

		// ULL ^= unsigned long long
		inline ULL& operator^=(const unsigned long long other) { *this = *this ^ ULL(other); return *this; }

		// ULL == ULL
		inline bool operator==(const ULL& other) const { return this->value == other.value; }

		// ULL == unsigned long long
		inline bool operator==(const unsigned long long other) const { return this->value == (other % p); }

		// unsigned long long == ULL
		inline friend bool operator==(const unsigned long long lhs, const ULL& rhs) { return (lhs % p) == rhs.value; }

		// ULL != ULL
		inline bool operator!=(const ULL& other) const { return this->value != other.value; }

		// ULL != unsigned long long
		inline bool operator!=(const unsigned long long other) const { return this->value != (other % p); }

		// unsigned long long != ULL
		inline friend bool operator!=(const unsigned long long lhs, const ULL& rhs) { return (lhs % p) != rhs.value; }

		// ULL < ULL
		inline bool operator<(const ULL& other) const { return this->value < other.value; }

		// ULL < unsigned long long
		inline bool operator<(const unsigned long long other) const { return this->value < (other % p); }

		// unsigned long long < ULL
		inline friend bool operator<(const unsigned long long lhs, const ULL& rhs) { return (lhs % p) < rhs.value; }

		// ULL <= ULL
		inline bool operator<=(const ULL& other) const { return this->value <= other.value; }

		// ULL <= unsigned long long
		inline bool operator<=(const unsigned long long other) const { return this->value <= (other % p); }

		// unsigned long long <= ULL
		inline friend bool operator<=(const unsigned long long lhs, const ULL& rhs) { return (lhs % p) <= rhs.value; }

		// ULL > ULL
		inline bool operator>(const ULL& other) const { return this->value > other.value; }

		// ULL > unsigned long long
		inline bool operator>(const unsigned long long other) const { return this->value > (other % p); }

		// unsigned long long > ULL
		inline friend bool operator>(const unsigned long long lhs, const ULL& rhs) { return (lhs % p) > rhs.value; }

		// ULL >= ULL
		inline bool operator>=(const ULL& other) const { return this->value >= other.value; }

		// ULL >= unsigned long long
		inline bool operator>=(const unsigned long long other) const { return this->value >= (other % p); }

		// unsigned long long >= ULL
		inline friend bool operator>=(const unsigned long long lhs, const ULL& rhs) { return (lhs % p) >= rhs.value; }
	};
}