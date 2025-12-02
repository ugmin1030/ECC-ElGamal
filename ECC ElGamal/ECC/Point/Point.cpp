#include "Point.h"
#include "../Modulo/mod.h"
#include "../ECC.h"

namespace ECC {

	// 유한 점 생성자
	Point::Point(const long long x, const long long y) {
		
		// 곡선 상에 존재하는지 확인
		if (isOnCurve(x, y)) {
			this->x = x;
			this->y = y;

		} else {
			std::cout << "점이 곡선 상에 존재하지 않습니다. 무한 원점 반환." << std::endl;
			*this = getInfinity();
		}
	}

	// 무한 원점 생성자
	Point::Point() : _isInfinity(true) {}

	// 무한 원점 싱글톤 반환 함수
	Point& Point::getInfinity() {
		static Point instance;
		return instance;
	}

	// 점이 ECC 상에 존재하는지 확인
	bool Point::isOnCurve(const unsigned long long x, const unsigned long long y){

		// y^2
		unsigned long long ypow2 = modMul(y, y);

		// x^3
		unsigned long long xpow3 = modPow(x, 3ULL);

		// ax + b
		unsigned long long ax_plus_b = modAdd(modMul(a, x), b);

		// y^2 == x^3 + ax + b
		return ypow2 == modAdd(xpow3, ax_plus_b);
	}

	bool Point::isOnCurve() const {

		// 무한 원점은 항상 true 반환
		if (_isInfinity) return true;

		return isOnCurve(this->x, this->y);
	}

	// 점 비교
	bool Point::operator==(const Point& other) const {
		return (this->_isInfinity && other._isInfinity) || (this->x == other.x && this->y == other.y);
	}

	// 점 반전
	Point Point::operator-() const {
		return this->_isInfinity ? getInfinity() : Point(x, p - y);
	}

	// 점 덧셈
	Point Point::operator+(const Point& other) const {

		// 항등원인 무한 원점과의 연산 처리
		if (this->_isInfinity) return other;
		if (other._isInfinity) return *this;

		// 역원 관계 연산 시 항등원 반환
		if (*this == -other) return getInfinity();

		// 기울기
		unsigned long long d = 0;

		// 새로운 좌표
		unsigned long long newX = 0;
		unsigned long long newY = 0;

		if (*this == other) {	// 점이 같은 경우

			// 두 점이 모두 y = 0인 경우엔 항등원 반환
			if (y == 0) return getInfinity();
			
			// (3x^2 + a)
			unsigned long long numerator = modAdd(modMul(3ULL, modMul(x, x)), a);

			// (2y)
			unsigned long long denominator = modMul(2ULL, y);

			// d = (3x^2 + a) / (2y) mod p
			d = modDiv(numerator, denominator);

			// newX = d^2 - 2x mod p
			newX = modSub(modMul(d, d), modMul(2ULL, x));

			// newY = d(x - newX) - y mod p
			newY = modSub(modMul(d, modSub(x, newX)), y);

		} else {	// 점이 다른 경우
			
			// (y2 - y1)
			unsigned long long numerator = modSub(other.y, y);

			// (x2 - x1)
			unsigned long long denominator = modSub(other.x, x);

			// d = (y2 - y1) / (x2 - x1) mod p
			d = modDiv(numerator, denominator);

			// newX = d^2 - x1 - x2 mod p
			newX = modSub(modSub(modMul(d, d), x), other.x);

			// newY = d(x1 - newX) - y1 mod p
			newY = modSub(modMul(d, modSub(x, newX)), y);
		}

		return Point(newX, newY);
	}

	// 점 뺄셈
	Point Point::operator-(const Point& other) const {
		return *this + (-other);
	}

	// 점 스칼라 곱셈
	// P * k
	Point Point::operator*(unsigned long long k) const {

		// 무한 원점과의 연산 처리
		if (this->_isInfinity) return *this;

		// 점이 y = 0인 경우엔 항등원 반환
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
	Point operator*(const unsigned long long k, const Point& P) {
		return P * k;
	}

	// 출력
	std::ostream& operator<<(std::ostream& os, const Point& p) {

		// 무한 원점 출력
		if (p.isInfinity()) {
			os << "(Infinity)";
		}

		// 일반 점 출력
		else {
			os << "(" << p.getX() << ", " << p.getY() << ")";
		}

		return os;
	}
}