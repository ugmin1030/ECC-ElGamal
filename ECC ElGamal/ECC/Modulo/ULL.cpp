#include "ULL.h"
#include "../ECC.h"
#include <intrin.h>

namespace ECC {

	// 모듈로 연산 함수
	// ULL + ULL
	ULL ULL::operator+(const ULL& other) const {

		unsigned long long x = this->value, y = other.value;

		// 오버플로우 검사
		unsigned long long tmp = p - x;

		// x + y ≡ y - (p - x) mod p
		return ULL(tmp < y ? y - tmp : x + y);
	}

	// ULL - ULL
	ULL ULL::operator-(const ULL& other) const {

		unsigned long long x = this->value, y = other.value;

		// 언더플로우 검사
		// x - y ≡ p - (y - x) mod p
		return x < y ? (p - (y - x)) : (x - y);
	}

	// ULL * ULL
	ULL ULL::operator*(const ULL& other) const {
		unsigned long long x = this->value, y = other.value;

		// 128비트 중 상위 64비트와 하위 64비트
		unsigned long long hi;
		unsigned long long lo = _umul128(x, y, &hi);

		// 128비트 값을 p로 나눈 나머지
		unsigned long long result;

		// mod p 계산
		_udiv128(hi, lo, p, &result);

		return result;
	}

	// ULL ^ ULL
	ULL ULL::operator^(const ULL& other) const {
		ULL x = *this, n = other;

		ULL result = 1;

		while (n > 0) {

			// n의 마지막 비트가 1인 경우
			if (n.value & 1) {

				// result = (result * x) % p;
				result *= x;
			}

			// x = (x * x) % p;
			x *= x;

			// n을 오른쪽으로 1비트 시프트
			n.value >>= 1;
		}

		return result;
	}

	// ULL / ULL
	ULL ULL::operator/(const ULL& other) const {

		// x / y ≡ x * y^(-1) mod p
		return *this * other ^ (-1);
	}
}