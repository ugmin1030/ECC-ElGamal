#include "Point.h"
#include "mod.h"
#include "ECC.h"

namespace ECC {

	// TODO: 곡선 상에 존재하는지 확인 필요
	// 유한 점 생성자
	Point::Point(const long long x, const long long y) : x(x), y(y) {}

	// 무한 원점 생성자
	Point::Point() : _isInfinity(true) {}

	// 무한 원점 싱글톤 반환 함수
	Point& Point::getInfinity() {
		static Point instance;
		return instance;
	}

	// 점 비교
	bool Point::operator==(const Point& other) const {
		return (this->_isInfinity && other._isInfinity) || (this->x == other.x && this->y == other.y);
	}

	// TODO: 점 연산 구현
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
		unsigned long long d;

		// 새로운 좌표
		unsigned long long newX = 0;
		unsigned long long newY = 0;

		if (*this == other) {
			// 점이 같은 경우
			unsigned long long numerator = (3 * x * x + a) % p;
			unsigned long long denominator = (2 * y) % p;
			d = (numerator * modInverse(denominator)) % p;
		} else {
			// 점이 다른 경우
			long long numerator = (other.y - y + p) % p;
			long long denominator = (other.x - x + p) % p;
			d = (numerator * modInverse(denominator)) % p;
		}
		return Point(newX, newY);
	}

	// 점 뺄셈
	Point Point::operator-(const Point& other) const {
		return *this + (-other);
	}

	// 점 스칼라 곱셈
	Point Point::operator*(const unsigned long long k) const {
		if (this->_isInfinity) return *this;
		if (k % p == 0) return Point();
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