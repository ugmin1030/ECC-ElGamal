#pragma once

namespace ECC {

	// 타원 곡선의 계수 a, b
	extern unsigned long long a;
	extern unsigned long long b;

	// 유한체의 소수 p
	extern unsigned long long p;

	// ECC 초기화 여부
	extern bool isNotInitialized;

	//// a, b, p 초기화 함수
	//void initECC() noexcept;
}