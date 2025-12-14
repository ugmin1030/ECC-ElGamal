#include <iostream>
#include "ULL.h"
#include "../Point/Point.h"

namespace ECC {

	// 문자열을 ULL 벡터로 인코딩 (암호화에 사용)
	std::vector<ULL> stringToULL(const std::string& str) {

		/*
		* 문자열을 7바이트씩 끊어서 ULL 벡터로 변환.
		* ECC 상에 존재해야하므로 7비트 패딩을 놓고 1씩 더하며 findY 검사.
		* 암호화 시엔 x = n * 128 + i 형태로 매핑, 복호화 시에 x/256은 n이 됨.
		*/

		std::vector<ULL> result;

		const size_t chunkSize = 7;
		size_t pos = 0;
		size_t strLen = str.size();

		while (pos < strLen) {

			// 현재 청크 길이 (마지막 청크는 7보다 작을 수 있음)
			size_t len = (strLen - pos) > chunkSize ? chunkSize : (strLen - pos);

			// 청크를 정수 n으로 변환
			unsigned long long n = 0;
			for (size_t k = 0; k < len; ++k) {

				// n을 1바이트씩 쉬프트
				n = (n << 8) | static_cast<unsigned long long>(static_cast<unsigned char>(str[pos + k]));
			}

			bool mapped = false;

			// 문자열 길이가 7바이트(chunkSize)보다 작으면, 
			// ULLToString 함수가 왼쪽부터 읽을 수 있도록 비트를 왼쪽으로 밈
			if (len < chunkSize) {
				n <<= (chunkSize - len) * 8;
			}

			// 7비트 패딩을 놓고 1씩 더하며 findY 검사
			for (unsigned int pad = 0; pad < 128; ++pad) {

				// x = n * 128 + p  (n << 7)
				unsigned long long xVal = (n << 7) + static_cast<unsigned long long>(pad);

				ULL x(xVal);

				// 이 x에 대해 ECC 상의 y가 존재하는지 확인
				auto possibleY = Point::findY(x);
				if (possibleY.has_value()) {
					result.push_back(x);
					mapped = true;
					break;
				}
			}

			// 매핑 실패
			if (!mapped) {

				// pos와 len을 이용해 실패한 청크 문자열 추출
				std::string_view error_str = std::string_view(str).substr(pos, len);

				std::cout << "error: 청크 문자열 \"" << error_str << "\"을 ULL로 매핑하는 데 실패했습니다." << std::endl;

				//실패 문자열 출력 후 throw
				throw std::invalid_argument("error: 문자열을 ULL로 매핑하는 데 실패했습니다.");
			}

			// 다음 청크로 이동
			pos += len;
		}

		return result;
	}

	// ULL 벡터를 문자열로 디코딩 (복호화에 사용)
	std::string ULLToString(std::vector<ULL> ull_vec){

		/*
		* ULL의 7바이트를 1바이트씩 문자열로 변환.
		* ECC 상에 존재해야하므로 패딩된 7비트는 제거하고 변환.
		* 암호화 시엔 x = n * 128 + i 형태로 매핑, 복호화 시에 x/256은 n이 됨.
		*/

		std::string result;

		// ULL 벡터를 하나씩 처리
		for(auto ull : ull_vec){

			// 값에서 7비트 패딩 제거
			unsigned long long n = ull.value >> 7;

			// n을 다시 문자열로 변환
			for(int i = 6; i >= 0; --i){

				// 마지막 바이트부터 추출
				unsigned char ch = static_cast<unsigned char>((n >> (i * 8)) & 0xFF);

				// 널 문자(0)가 나오면 중단
				if(ch == 0) break;

				result += static_cast<char>(ch);
			}
		}

		return result;
	}
}