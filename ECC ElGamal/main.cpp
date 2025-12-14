#include <iostream>
#include <random>
#include "ECC/ECC.h"
#include "ECC/Point/Point.h"
#include "ECC/group/ULL.h"
#include "ECC/Elgamel/Elgamel.h"
#include "ECC/Elgamel/IPC.h"

static void testULL() {

	// ULL.h 테스트
	ECC::ULL x = (ECC::ULL)1557;
	ECC::ULL y = (ECC::ULL)1844674407370955155ULL;

	std::cout << "x = " << x << std::endl;
	std::cout << "y = " << y << std::endl;

	// 대수 연산
	std::cout << "x + y = " << (x + y) << std::endl;
	std::cout << "x - y = " << (x - y) << std::endl;
	std::cout << "x * y = " << (x * y) << std::endl;
	std::cout << "x ^ y = " << (x ^ y) << std::endl;
	std::cout << "y ^ -1 = " << (y ^ -1) << std::endl;
	std::cout << "x / y = " << (x / y) << std::endl;

	// 비교 연산
	std::cout << "x == y: " << (x == y) << std::endl;
	std::cout << "x != y: " << (x != y) << std::endl;
	std::cout << "x < y: " << (x < y) << std::endl;
	std::cout << "x <= y: " << (x <= y) << std::endl;
	std::cout << "x > y: " << (x > y) << std::endl;
	std::cout << "x >= y: " << (x >= y) << std::endl;
}

static void testPointH() {

	// Point.h 테스트
	// Point(x)
	std::cout << "Enter x coordinate for Point P: ";
	ECC::ULL x_input;
	std::cin >> x_input;

	ECC::Point A(x_input);

	std::cout << "Point P: " << A << std::endl;

	// findY 테스트
	auto pp = ECC::Point::findY(x_input).value();
	std::cout << ECC::Point::findY(x_input).value().first << ", " << ECC::Point::findY(x_input).value().second << std::endl;

	// Point + Point
	ECC::Point P = ECC::Point(x_input, pp.first);
	ECC::Point Q = ECC::Point(x_input, pp.second);
	std::cout << "Point P: " << P << std::endl;
	std::cout << "Point Q: " << Q << std::endl;
	std::cout << "P + Q = " << (P + Q) << std::endl;

	// -Point
	std::cout << "-P = " << -P << std::endl;

	// Point - Point
	std::cout << "P - Q = " << (P - Q) << std::endl;

	// Point * ULL
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis(0, ECC::p - 1);

	ECC::ULL k = dis(gen);

	std::cout << "k = " << k << std::endl;
	std::cout << "k * P = " << k * P << std::endl;

	// isOnCurve 테스트
	std::cout << "Is P on curve? " << P.isOnCurve() << std::endl;
	std::cout << "Is P on curve? " << ECC::Point::isOnCurve(P.getX(), P.getY() + 1) << std::endl;

	// Point == Point
	std::cout << "P == Q: " << (P == Q) << std::endl;
	std::cout << "P == P: " << (P == P) << std::endl;

	// Point != Point
	std::cout << "P != Q: " << (P != Q) << std::endl;
	std::cout << "P != P: " << (P != P) << std::endl;
}

static void testStringWithULL() {

	// stringWithULL 테스트
	std::string message = "Hello, ECC!";

	std::vector<ECC::ULL> ull_vec;

	try{
		std::cout << "Original message: " << message << std::endl;
		ull_vec = ECC::stringToULL(message);
	}

	catch(const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return;
	}

	std::cout << "Converted to ULL vector: ";
	for (const auto& ull : ull_vec) {
		std::cout << ull << " ";
	}
	std::cout << std::endl;

	std::string recovered_message = ECC::ULLToString(ull_vec);
	std::cout << "Recovered message: " << recovered_message << std::endl;
}

static void testElgamel() {

	// 키 생성
	ECC::Elgamel::keyGeneration();

	// 평문
	std::string plane = "Hello, ECC Elgamel!";
	std::cout << "Plaintext: " << plane << std::endl;

	// 암호화
	std::vector<ECC::Elgamel::Ciphertext> cipher_vector = ECC::Elgamel::encrypt(plane);

	// 암호문 출력
	std::cout << "Ciphertext:" << std::endl;
	for (const auto& cipher : cipher_vector) {
		std::cout << "C1: " << cipher.C1 << ", C2: " << cipher.C2 << std::endl;
	}

	// 복호화
	std::string decrypted_message = ECC::Elgamel::decrypt(cipher_vector);

	// 복호문 출력
	std::cout << "Decrypted message: " << decrypted_message << std::endl;
}

int main() {

	// ECC.a, b, p 출력
	std::cout << "ECC parameters:" << std::endl;
	std::cout << "y^2 = x^3 + " << ECC::a << "x + " << ECC::b << " over GF(" << ECC::p << ")" << std::endl;

	// 메인 로직 시작
	int mode;
	std::cout << "모드를 선택하세요 (1: Receiver(받기), 2: Sender(보내기)): ";
	std::cin >> mode;
	std::cin.ignore(); // 버퍼 비우기

	if (mode == 1) {
		// Receiver 실행 (먼저 실행)
		ECC::IPC::receiver();
	}
	else if (mode == 2) {
		// Sender 실행 (Receiver가 켜진 상태에서 새 창으로 실행)
		std::string msg;
		std::cout << "전송할 메시지 입력: ";
		std::getline(std::cin, msg);

		ECC::IPC::sender(msg);
	}
	else {
		std::cout << "잘못된 입력입니다." << std::endl;
	}

	// 창이 바로 꺼지지 않게 대기
	system("pause");
	
	return 0;
}