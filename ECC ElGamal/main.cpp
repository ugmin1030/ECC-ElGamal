#include "mod.h"
#include "ECC.h"
//#include "Point.h"
#include <iostream>

int main() {

	// long long 최대 소수
	unsigned long long p = 18446744073709551557ULL;

	ECC::p = p;
	ECC::a = 2;
	ECC::b = 3;

	std::cout << "ECC ElGamal Test" << std::endl;
	std::cout << "Curve: y^2 = x^3 + " << ECC::a << "x + " << ECC::b << std::endl;

	// mod.cpp 테스트
	std::cout << "Modular Arithmetic Tests:" << std::endl;

	std::cout << "modAdd(18446744073709551556ULL, 18446744073709551556ULL) = " 
		<< ECC::modAdd(18446744073709551556ULL, 18446744073709551556ULL) << std::endl;

	std::cout << "modSub(18446744073709551557ULL, 18446744073709551555ULL) = "
		<< ECC::modSub(18446744073709551557ULL, 18446744073709551555ULL) << std::endl;

	//std::cout << "modMul(12345678901234567, 76543210987654321) = " 
	//	<< ECC::modMul(12345678901234567LL, 76543210987654321LL) << std::endl;

	//std::cout << "modPow(12345678901234567, 32) = " 
	//	<< ECC::modPow(12345678901234567LL, 32ULL) << std::endl;

	//std::cout<< "modInverse(12345678901234567) = " 
	//	<< ECC::modInverse(12345678901234567LL) << std::endl;

	return 0;
}