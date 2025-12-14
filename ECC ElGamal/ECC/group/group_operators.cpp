#include "ULL.h"
#include "../ECC.h"
#include <intrin.h>

namespace ECC {

	// 모듈로 연산 함수
	// ULL + ULL
	ULL ULL::operator+(const ULL& other) const noexcept {

		unsigned long long x = this->value, y = other.value;

		// 오버플로우 검사
		unsigned long long tmp = p - x;

		// x + y ≡ y - (p - x) mod p
		return ULL(tmp < y ? y - tmp : x + y);
	}

	// ULL - ULL
	ULL ULL::operator-(const ULL& other) const noexcept {

		unsigned long long x = this->value, y = other.value;

		// 언더플로우 검사
		// x - y ≡ p - (y - x) mod p
		return x < y ? (p - (y - x)) : (x - y);
	}

	// ULL * ULL
	ULL ULL::operator*(const ULL& other) const noexcept {

		// 오버플로우 방지 연산 라이브러리 사용
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

		// 0의 0 제곱은 예외 처리
		if ((this->value == 0) && (other.value == 0)) {
			std::cout << "error: 0의 0 제곱은 정의되지 않았습니다." << std::endl;
			throw std::invalid_argument("error: 0의 0 제곱은 정의되지 않았습니다.");
		}

		// 지수가 0인 경우 1 반환
		if ((other.value == 0)) return ULL(1);
		
		// 지수가 1이거나 밑이 0 또는 1인 경우 자기 자신 반환
		if ((other.value == 1) || (this->value == 1) || (this->value == 0)) return *this;

		ULL x = *this;
		ULL n = other;

		ULL result = 1;

		// 고속 지수 연산 사용
		while (n > 0) {

			// n의 마지막 비트가 1인 경우
			if (n & 1) {

				// result = (result * x) % p;
				result *= x;
			}

			// x = (x * x) % p;
			x *= x;

			// n을 오른쪽으로 1비트 시프트
			n >>= 1;
		}

		return result;
	}

	// ULL ^ unsigned long long
	ULL ULL::operator^(const unsigned long long other) const {

		// other가 -1이면 역원으로 처리.
		if (other == -1) {

			// this가 0이면 예외 처리
			if (this->value == 0) {
				std::cout << "error: 0의 역원은 정의되지 않았습니다." << std::endl;
				throw std::invalid_argument("error: 0의 역원은 정의되지 않았습니다.");
			}

			return *this ^ ULL(p - 2);
		}

		else {

			// 지수는 최대 p - 2
			return *this ^ ULL(other > p - 2 ? (other - (p - 2)) : other);
		}
	}

	// ULL / ULL
	ULL ULL::operator/(const ULL& other) const {

		// 분모가 0인 경우 예외 처리
		if (other.value == 0) {
			std::cout << "error: 0으로 나눌 수 없습니다." << std::endl;
			throw std::invalid_argument("error: 0으로 나눌 수 없습니다.");
		}

		// x / y ≡ x * y^(-1) mod p
		return *this * (other ^ (-1));
	}
}