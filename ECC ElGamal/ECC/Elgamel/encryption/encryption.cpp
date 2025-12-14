#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <random>

#include "../../Point/Point.h"
#include "../../group/ULL.h"
#include "../Elgamel.h"

namespace ECC {
	namespace Elgamel {

		// ElGamal 암호화: 평문을 ECC 상 점에 매핑한 후 암호화 수행
		std::vector<Ciphertext> encrypt(std::string plane) {

			std::vector<Ciphertext> result;

			// 문자열을 ULL 벡터로 변환
			std::vector<ULL> ull_vector = ECC::stringToULL(plane);

			// 무작위 수 생성기
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned long long> dis(1, p - 1);

			// 각 ULL에 대해 Point로 매핑 후 암호화 수행
			for (auto& m : ull_vector) {

				// 무작위 k 선택
				ULL r = dis(gen);

				// m은 항상	ECC 상의 점이므로 Point로 매핑
				Point P(m);

				// 암호문 계산
				Point C1 = publicKey.e1 * r;
				Point C2 = P + (publicKey.e2 * r);

				// 암호문 저장
				result.push_back({ C1, C2 });
			}

			return result;
		}
	}
}