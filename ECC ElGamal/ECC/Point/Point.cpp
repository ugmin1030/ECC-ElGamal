#include <iostream>
#include <random>
#include "Point.h"
#include "../group/ULL.h"
#include "../ECC.h"

namespace ECC {

	// 유한 점 생성자
	Point::Point(const ULL x) noexcept {

		ULL x_value = x;

		// 가능한 y 값 찾기
		auto y_values = findY(x_value);

		while (!y_values) {
			std::cout << "주어진 x 좌표에 대한 유효한 y 값이 없습니다. 다른 x 값을 입력하세요: ";
			std::cin >> x_value;
			y_values = findY(x_value);
		}

		// y 값 무작위 선택
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 1);

		this->x = x_value;
		this->y = dis(gen) ? y_values->first : y_values->second;
	}

	Point::Point(const ULL x, const ULL y) noexcept {
		
		// 곡선 상에 존재하는지 확인
		if (isOnCurve(x, y)) {
			this->x = x;
			this->y = y;
		} 
		
		else {
			std::cout << "점이 곡선 상에 존재하지 않습니다. 무한 원점 반환." << std::endl;
			*this = getInfinity();
		}
	}

	// 무한 원점 싱글톤 반환 함수
	Point& Point::getInfinity() noexcept {
		static Point instance;
		return instance;
	}

}