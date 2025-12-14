#pragma once
#include <string>
#include <utility>
#include <vector>
#include "../Point/Point.h"
#include "../group/ULL.h"

namespace ECC {

	namespace Elgamel{

		// 공개 키: e1, e2 점의 쌍
		struct PublicKey {
			Point e1;
			Point e2;
			PublicKey() = default;
			PublicKey(const Point& e1, const Point& e2) : e1(e1), e2(e2) {}
		};

		inline PublicKey publicKey;

		// 비밀 키: d 스칼라
		inline ULL d;

		// 암호문: 점의 쌍
		struct Ciphertext {
			Point C1;
			Point C2;
		};

		// ElGamal 키 생성
		void keyGeneration();

		// ElGamal 암호화: 평문을 ECC 상 점에 매핑한 후 암호화 수행
		std::vector<Ciphertext> encrypt(std::string plane);

		// ElGamal 복호화: 암호문에서 점을 복원한 후 문자열로 변환
		std::string decrypt(const std::vector<Ciphertext>& cipher_vector);
	}
}