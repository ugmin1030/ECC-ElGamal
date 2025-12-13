#include "Point.h"
#include "../group/ULL.h"
#include "../ECC.h"

namespace ECC {

	// 점 덧셈
	Point Point::operator+(const Point& other) const noexcept {

		// 항등원인 무한 원점과의 연산 처리
		if (this->_isInfinity) return other;
		if (other._isInfinity) return *this;

		// 역원 관계 연산 시 항등원 반환
		if (*this == -other) return getInfinity();

		// 점을 이은 선분의 기울기
		ULL d;

		// 새로운 좌표
		ULL newX;
		ULL newY;

		// 점이 같은 경우 doubling
		if (*this == other) {	

			// 두 점이 모두 y = 0인 경우엔 항등원 반환
			if (y == 0) return getInfinity();

			// d = (3x^2 + a) / (2y) mod p
			d = ((ULL)3 * x * x + a) / ((ULL)2 * y);

			// newX = d^2 - 2x mod p
			newX = d * d - (ULL)2 * x;

			// newY = d(x - newX) - y mod p
			newY = d * (x - newX) - y;

		}

		// 점이 다른 경우
		else {	

			// d = (y2 - y1) / (x2 - x1) mod p
			d = (other.y - y) / (other.x - x);

			// newX = d^2 - x1 - x2 mod p
			newX = d * d - x - other.x;

			// newY = d(x1 - newX) - y1 mod p
			newY = d * (x - newX) - y;
		}

		return Point(newX, newY);
	}

	// 점 뺄셈
	Point Point::operator-(const Point& other) const noexcept {
		return *this + (-other);
	}

	// 점 스칼라 곱셈
	// P * k
	Point Point::operator*(const ULL other) const noexcept {
		ULL k = other;

		// 무한 원점과의 연산 처리
		if (this->_isInfinity) return *this;

		// 점이 y = 0인 경우 k가 짝수면 항등원, k가 홀수면 원점 반환
		if (y == 0) return (k & 1) ? *this : getInfinity();

		// k = 0인 경우 무한 원점 반환
		if (k == 0) return getInfinity();

		// 덧셈을 받을 점은 무한 원점으로 초기화
		Point result = getInfinity();

		Point x = *this;

		while (k > 0) {

			// k의 마지막 비트가 1인 경우
			if (k & 1) {

				// result + P
				result = result + x;
			}

			x = x + x;

			k >>= 1;
		}

		return result;
	}

	// k * P
	Point operator*(const ULL k, const Point& P) noexcept {
		return P * k;
	}

	// 출력
	std::ostream& operator<<(std::ostream& os, const Point& P) noexcept {

		// 무한 원점 출력
		if (P.isInfinity())
			os << "(Infinity)";

		// 일반 점 출력
		else 
			os << "(" << P.getX() << ", " << P.getY() << ")";

		return os;
	}
}