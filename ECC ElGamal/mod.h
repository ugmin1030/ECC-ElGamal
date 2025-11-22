#pragma once
namespace ECC {

	// 모듈로 연산 함수

	// 모듈로 상 덧셈 함수
	unsigned long long modAdd(unsigned long long x, unsigned long long y);

	// 모듈로 상 뺄셈 함수
	unsigned long long modSub(unsigned long long x, unsigned long long y);

	// 모듈로 상 곱셈을 위한 128비트 곱셈 함수
	unsigned long long modMul(unsigned long long x, unsigned long long y);
	
	// 고속 지수 연산
	unsigned long long modPow(unsigned long long x, unsigned long long n);

	// 곱셈에 대한 역원
	unsigned long long modInverse(unsigned long long x);
}