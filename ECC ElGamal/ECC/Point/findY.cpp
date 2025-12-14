#include <utility>
#include <optional>
#include "Point.h"
#include "../group/ULL.h"
#include "../ECC.h"

namespace ECC {
	
	// 2차 비잉여 찾기
	static ULL findQNR() noexcept {

		// z/p ≡ -1인 z를 순서대로 탐색
		for (ULL z = ULL(2), t;; z++ ) {

			// t = z/p = z^{(p-1)/2} (mod p) 계산
			t = z ^ ((ULL(p - 1)) >> ULL(1));

			// -1이면 z는 2차 비잉여이므로 반환
			if (t == ULL(p - 1)) return z;
		}
	}

	// Tonelli-Shanks 알고리즘
	static std::pair<ULL, ULL> Tonelli_Shanks(const ULL n) noexcept {

		// p-1 = q * 2^s 꼴로 분해 (q는 홀수)
		ULL q = ULL(p - 1);
		ULL s = 0;

		// q가 홀수가 될 때까지 2로 나누고 s 증가
		while ((q & ULL(1)) == ULL(0)) {
			q >>= ULL(1);
			s++;
		}

		// 2차 비잉여 z 찾기. static으로 선언하여 한 번만 연산.
		static ULL z = findQNR();

		// r, t, c, t2i, m 변수 초기화

		// r: 제곱근 후보
		// r = n^{(q+1)/2}
		ULL r = n ^ ULL((q + 1) >> 1);

		// t: r이 얼마나 틀렸는지 나타내는 오차. 제곱수가 2의 제곱수일 때 항상 1이 됨.
		// t = n^q = (r^2)/n
		// t = 1일 때, r^2 = n이므로 r 반환
		ULL t = n ^ q;

		// c: 제곱해도 쉽게 1이 되지 않는 방향을 제공하는 기준축
		// t를 1로 만드는 데 사용됨.
		ULL c = z ^ q;

		// m = s
		// 최소 i를 찾기 위한 상한
		ULL m = s;

		// while t != 0 and t != 1
		while (t != ULL(0) && t != ULL(1)) {
			ULL tt = t * t;
			ULL i = 1;

			while (i < m) {
				tt *= tt;
				if (tt == ULL(1)) break;
				i++;
			}

			// b = c^{2^{m-i-1}}
			// b^2 = c^{2^{m-i}}
			ULL b = c;

			// 2^{m-i-1} 값이 p보다 커지면 오버플로우 발생하므로 반복문으로 거듭제곱 수행
			for (ULL j = 0; j < m - i - 1; j++) {
				b *= b; // 거듭제곱 수행
			}

			// r <- (r * b)
			r *= b;

			// c <- (b * b)
			c = (b * b);

			// t <- (t * b * b)
			t *= c;

			// m <- i
			m = i;
		}

		// r, -r 반환
		return { r, ULL(p) - r };
	}

	// x로 가능한 y 값 찾기
	std::optional<std::pair<ULL, ULL>> Point::findY(const ULL x) {

		/* 
		* 오일러 판정법 사용.
		* 
		*					  ⌈ 1이면 n은 2차 잉여,
		* n^{(p-1)/2} mod p ≡ | -1이면 n은 2차 비잉여, 
		*					  ⌊ 0이면 n은 0.
		*/

		// ECC 우변 계산: n = x^3 + ax + b
		ULL n = getRHS(x);
		
		// n이 0인 경우, 0 반환
		if (n == 0) return { std::make_pair(ULL(0), ULL(0)) };

		// n^{(p-1)/2} mod p 계산.
		ULL legendre = n ^ (ULL(p - 1) >> ULL(1));

		// 2차 잉여인 경우
		if (legendre == ULL(1)) {

			// p ≡ 3 (mod 4)인 특수 경우 처리
			if (p % 4 == 3) {

				// y = n^{(p+1)/4} mod p 계산
				ULL y = n ^ ((ULL(p + 1)) >> ULL(2));
				return std::make_pair(y, -y);
			}

			// 모듈로가 소수인 2차 합동 방정식의 해를 찾는 Tonelli-Shanks 알고리즘 사용
			else {
				return Tonelli_Shanks(n);
			}
		}

		else {
			// 2차 비잉여인 경우, std::nullopt 반환
			return std::nullopt;
		}
	}
}