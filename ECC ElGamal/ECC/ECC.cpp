#include "ECC.h"

namespace ECC {	
	//// a, b, p 초기화 함수
	//void initECC() noexcept {
	//	std::cout << "타원 곡선의 계수 a, b: ";
	//	std::cin >> a >> b;

	//	std::cout << "유한체의 소수 p: ";
	//	std::cin >> p;

	//	isNotInitialized = false;
	//}

	// <디버그용으로 값을 고정>
	// 타원 곡선의 계수 a, b
	unsigned long long a = 2ULL;
	unsigned long long b = 3ULL;

	// 유한체의 소수 p (unsigned long long 기준 최대 소수)
	unsigned long long p = 18446744073709551557ULL;

	// ECC 초기화 여부
	bool isNotInitialized = false;
}