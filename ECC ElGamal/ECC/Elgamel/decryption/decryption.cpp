#pragma once
#include <string>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>

#include "../../ECC.h"
#include "../../group/ULL.h"
#include "../Elgamel.h"
#include "../../Point/Point.h"

// 암호문을 ull로 ECC Elgamel 복호화 후 평문 문자열로 변환
namespace ECC {

	namespace Elgamel {

		// ElGamal 키 생성
		void keyGeneration() {

			std::cout << "<ElGamal Key Generation>" << std::endl;

			// 비밀 키 d 입력
			std::cout << "Private key d: ";
			std::cin >> d;

			// 공개 키 e1 무작위 생성
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned long long> dis(1, p - 1);

			// e1이 곡선 위의 점이 될 때까지 반복
			while (true) {

				unsigned long long k = dis(gen);
				auto y = Point::findY(k);

				if (y.has_value()) {
					Point e1 = Point(k, k % 2 == 0 ? y->first : y->second);

					// 공개 키 e1, e2 등록
					publicKey.e1 = e1;
					publicKey.e2 = e1 * d;
					break;
				}
			}

			std::cout << "Public key e1: " << publicKey.e1 << std::endl;
			std::cout << "Public key e2: " << publicKey.e2 << std::endl;
		}

		// ElGamal 복호화: 암호문에서 점을 복원한 후 문자열로 변환
		std::string decrypt(const std::vector<Ciphertext>& cipher_vector) {

			std::string result;

			std::vector<ULL> ull_vector;

			for (auto& cipher : cipher_vector) {

				// P = C2 - d * C1
				Point P = cipher.C2 - (cipher.C1 * d);

				// 평문인 점 P의 x 좌표를 ull로 변환
				ULL m = P.getX();

				// ULL 벡터에 추가
				ull_vector.push_back(m);
			}

			// ULL 벡터를 문자열로 변환
			result = ULLToString(ull_vector);

			return result;
		}
	}
}