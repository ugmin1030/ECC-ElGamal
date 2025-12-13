#pragma once
#include <iostream>
#include <utility>
#include <optional>
#include "../ECC.h"
#include "../group/ULL.h"

namespace ECC{

	// 타원 곡선 상의 점을 나타내는 클래스
	class Point {

	private:

		// x, y 좌표
		ULL x = 0;
		ULL y = 0;

		// 무한 원점 여부
		bool _isInfinity = false;
		
		// 무한 원점 생성자
		Point() noexcept : _isInfinity(true) {}
	
	public:

		// x로 가능한 y 값 찾기
		static std::optional<std::pair<ULL, ULL>> findY(const ULL x);

		// 유한 점 생성자
		Point(const ULL x) noexcept;
		Point(const ULL x, const ULL y) noexcept;

		// 무한 원점 싱글톤 반환 함수
		static Point& getInfinity() noexcept;

		// 무한 원점 여부 반환
		inline bool isInfinity() const noexcept { return _isInfinity; }

		// 좌표 반환
		inline ULL getX() const noexcept { return x; }
		inline ULL getY() const noexcept { return y; }

		// ECC 좌항, 우항 반환
		// y^2
		static inline ULL getLHS(ULL y) noexcept { return y * y; }
		inline ULL getLHS() const noexcept { return getLHS(y); }

		// x^3 + ax + b
		static inline ULL getRHS(ULL x) noexcept { return (x ^ (ULL)3) + ((ULL)a * x) + (ULL)b; }
		inline ULL getRHS() const noexcept { return getRHS(x); }

		// 점 반전
		Point operator-() const noexcept {
			return this->_isInfinity ? getInfinity() : Point(x, (ULL)p - y);
		}

		// 점이 ECC 상에 존재하는지 확인
		static inline bool isOnCurve(const ULL x, const ULL y) noexcept {

			// y^2 == x^3 + ax + b
			return getLHS(y) == getRHS(x);
		}

		inline bool isOnCurve() const noexcept {

			// 무한 원점은 항상 true 반환
			return _isInfinity ? true : isOnCurve(x, y);
		}

		// 점 덧셈
		Point operator+(const Point& other) const noexcept;

		inline Point& operator+=(const Point& other) {
			*this = *this + other;
			return *this;
		}

		// 점 뺄셈
		Point operator-(const Point& other) const noexcept;

		inline Point& operator-=(const Point& other) {
			*this = *this - other;
			return *this;
		}

		// 점 스칼라 곱셈
		Point operator*(const ULL other) const noexcept;

		inline Point& operator*=(const ULL k) {
			*this = *this * k;
			return *this;
		}

		// 점 비교
		inline bool operator==(const Point& other) const noexcept {
			return (this->_isInfinity && other._isInfinity) || (this->x == other.x && this->y == other.y);
		}

		inline bool operator!=(const Point& other) const noexcept {
			return !(*this == other);
		}

		// 출력
		friend std::ostream& operator<<(std::ostream& os, const Point& p) noexcept;
	};

	// k * P
	Point operator*(const ULL k, const Point& P) noexcept;
}