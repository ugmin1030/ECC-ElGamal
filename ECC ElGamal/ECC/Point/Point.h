#pragma once
#include <iostream>

namespace ECC{

	// 타원 곡선 상의 점을 나타내는 클래스
	class Point {

	private:

		// x, y 좌표
		unsigned long long x = 0;
		unsigned long long y = 0;

		// 무한 원점 여부
		bool _isInfinity = false;
		
		// 무한 원점 생성자
		Point();
	
	public:

		// x로 가능한 y 값 찾기
		// static unsigned long long* findY(const long long x);

		// 유한 점 생성자
		// Point(const long long x);
		Point(const long long x, const long long y);

		// 무한 원점 싱글톤 반환 함수
		static Point& getInfinity();

		// 무한 원점 여부 반환
		inline bool isInfinity() const { return _isInfinity; }

		// 점이 ECC 상에 존재하는지 확인
		static bool isOnCurve(const unsigned long long x, const unsigned long long y);
		bool isOnCurve() const;

		// 좌표 반환
		inline unsigned long long getX() const { return x; }
		inline unsigned long long getY() const { return y; }

		// 점 비교
		bool operator==(const Point& other) const;

		// 점 반전
		Point operator-() const;

		// 점 덧셈
		Point operator+(const Point& other) const;

		// 점 뺄셈
		Point operator-(const Point& other) const;

		// 점 스칼라 곱셈
		Point operator*(const unsigned long long k) const;

		// 출력
		friend std::ostream& operator<<(std::ostream& os, const Point& p);
	};

	// k * P
	Point operator*(const unsigned long long k, const Point& P);
}