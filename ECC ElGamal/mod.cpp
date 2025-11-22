#include "mod.h"
#include "ECC.h"
#include <intrin.h>

namespace ECC {
	// 모듈로 연산 함수

	// 모듈로 상 덧셈 함수
	unsigned long long modAdd(unsigned long long x, unsigned long long y) {

		// 오버플로우 검사
		unsigned long long tmp = p - x;

		// x + y ≡ y - (p - x) mod p
		return tmp < y ? y - tmp : x + y;
	}

	// 모듈로 상 뺄셈 함수
	unsigned long long modSub(unsigned long long x, unsigned long long y) {

		// x - y ≡ x + (p - y) mod p
		return modAdd(x, (p - y));
	}

	// 모듈로 상 곱셈을 위한 128비트 곱셈 함수
	unsigned long long modMul(unsigned long long x, unsigned long long y) {

		// 128비트 중 상위 64비트와 하위 64비트
		unsigned long long hi;
		unsigned long long lo = _umul128(x, y, &hi);

		// 128비트 값을 p로 나눈 나머지
		unsigned long long result;

		// mod p 계산
		_udiv128(hi, lo, p, &result);

		return result;
	}

	// 고속 지수 연산
	unsigned long long modPow(unsigned long long x, unsigned long long n) {

		unsigned long long result = 1;

		while (n > 0) {

			// n의 마지막 비트가 1인 경우
			if (n & 1) {

				// result = (result * x) % p;
				result = modMul(result, x);
			}

			// x = (x * x) % p;
			x = modMul(x, x);

			// n을 오른쪽으로 1비트 시프트
			n >>= 1;
		}

		return result;
	}

	// 곱셈에 대한 역원
	unsigned long long modInverse(unsigned long long x) {

		// x^(-1) ≡ x^(p-2) (mod p)
		return modPow(x, p - 2ULL);
	}
}