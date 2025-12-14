#include "IPC.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "../Elgamel/Elgamel.h"
#include "../group/ULL.h"
#include "../Point/Point.h"

// 통신할 파이프 이름
#define PIPE_NAME L"\\\\.\\pipe\\ECC_Elgamal_Pipe"

// 버퍼 크기
#define BUFFER_SIZE 1024

namespace ECC {
    namespace IPC {

        // 네트워크 전송을 위한 직렬화된 점 데이터 (내부용 구조체)
        struct RawPoint {
            unsigned long long x;
            unsigned long long y;
        };

        // 직렬화된 암호문 쌍 (내부용 구조체)
        struct RawCipher {
            RawPoint c1;
            RawPoint c2;
        };

        // Receiver: 서버 역할 (키 생성 -> 파이프 생성 -> 키 전송 -> 암호문 수신 -> 복호화)
        void receiver() {
            std::cout << "[Receiver] 파이프 서버를 시작합니다..." << std::endl;

            // 1. 키 생성 (Receiver가 개인키를 가짐)
            ECC::Elgamel::keyGeneration();

            // 2. 파이프 생성
            HANDLE hPipe = CreateNamedPipeW(
                PIPE_NAME,                                              // 파이프 이름 (주소)
                PIPE_ACCESS_DUPLEX,                                     // 개방 모드 (양방향)
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,  // 파이프 속성 (메시지 단위 저장 및 읽기 + 메시지 올 때까지 대기)
                1,                                                      // 최대 인스턴스 수 (일대일 통신)
                BUFFER_SIZE,                                            // 출력 버퍼 크기
                BUFFER_SIZE,                                            // 입력 버퍼 크기
                0,                                                      // 기본 타임아웃
                NULL                                                    // 보안 속성
            );

            // 파이프 생성 실패 시
            if (hPipe == INVALID_HANDLE_VALUE) {
                std::cout << "[Receiver] 파이프 생성 실패! (Error: " << GetLastError() << ")" << std::endl;
                return;
            }

            // 3. 클라이언트 접속 대기
            std::cout << "[Receiver] Sender의 접속을 대기 중입니다..." << std::endl;

            // ConnectNamedPipe 함수로 송신자의 파이프 연결까지 대기
            // 연결에 성공 시 공개 키 전송부터 시작
            if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
                std::cout << "[Receiver] Sender가 접속했습니다." << std::endl;

				// 공개 키 쓰기 및 메시지 읽기 완료된 바이트 수
                DWORD bytesWritten, bytesRead;

                // 4. 공개 키 전송 (Sender가 암호화할 수 있도록)
                // e1 전송
                RawPoint pubE1 = { ECC::Elgamel::publicKey.e1.getX().getValue(), ECC::Elgamel::publicKey.e1.getY().getValue() };
                WriteFile(hPipe, &pubE1, sizeof(RawPoint), &bytesWritten, NULL);

                // e2 전송
                RawPoint pubE2 = { ECC::Elgamel::publicKey.e2.getX().getValue(), ECC::Elgamel::publicKey.e2.getY().getValue() };
                WriteFile(hPipe, &pubE2, sizeof(RawPoint), &bytesWritten, NULL);

                std::cout << "[Receiver] 공개 키를 전송했습니다." << std::endl;

                // 5. 암호문 개수 수신
                size_t cipherCount = 0;
                ReadFile(hPipe, &cipherCount, sizeof(size_t), &bytesRead, NULL);
                std::cout << "[Receiver] 수신할 암호문 블록 수: " << cipherCount << std::endl;

                // 6. 암호문 데이터 수신 및 구조체 복원
                std::vector<ECC::Elgamel::Ciphertext> receivedCiphers;
                for (size_t i = 0; i < cipherCount; ++i) {
                    RawCipher raw;
                    ReadFile(hPipe, &raw, sizeof(RawCipher), &bytesRead, NULL);

                    // Raw Data -> ECC Point -> Ciphertext 변환
                    ECC::Point C1(ECC::ULL(raw.c1.x), ECC::ULL(raw.c1.y));
                    ECC::Point C2(ECC::ULL(raw.c2.x), ECC::ULL(raw.c2.y));

                    receivedCiphers.push_back({ C1, C2 });
                }

                // 7. 복호화 수행
                std::cout << "[Receiver] 복호화를 시작합니다..." << std::endl;
                std::string decrypted = ECC::Elgamel::decrypt(receivedCiphers);

                std::cout << "\n========================================" << std::endl;
                std::cout << "수신된 메시지: " << decrypted << std::endl;
                std::cout << "========================================\n" << std::endl;

            }
            else {
                std::cout << "[Receiver] 접속 대기 실패." << std::endl;
            }

            // 정리
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
        }

        // Sender: 클라이언트 역할 (접속 -> 키 수신 -> 암호화 -> 전송)
        void sender(const std::string& message) {
            std::cout << "[Sender] Receiver에게 접속을 시도합니다..." << std::endl;

            // 1. 파이프 접속
            HANDLE hPipe = CreateFile(
                PIPE_NAME,                     // 접속할 이름
                GENERIC_READ | GENERIC_WRITE,  // 권한 (읽기/쓰기)
                0,                             // 공유 모드 (독점)
                NULL,                          // 보안 속성
                OPEN_EXISTING,                 // 생성/열기 옵션
                0,                             // 파일 속성
                NULL                           // 템플릿 파일
            );

			// 파이프 접속 실패 시
            if (hPipe == INVALID_HANDLE_VALUE) {
                std::cout << "[Sender] 접속 실패! Receiver가 실행 중인지 확인하세요." << std::endl;
                return;
            }

			// 메시지 쓰기 및 공개 키 읽기 완료된 바이트 수
            DWORD bytesRead, bytesWritten;

            // 2. 공개 키 수신 (Receiver로부터)
            RawPoint rawE1, rawE2;
            ReadFile(hPipe, &rawE1, sizeof(RawPoint), &bytesRead, NULL);
            ReadFile(hPipe, &rawE2, sizeof(RawPoint), &bytesRead, NULL);

            // 공개 키 설정
            ECC::Elgamel::publicKey.e1 = ECC::Point(ECC::ULL(rawE1.x), ECC::ULL(rawE1.y));
            ECC::Elgamel::publicKey.e2 = ECC::Point(ECC::ULL(rawE2.x), ECC::ULL(rawE2.y));

            std::cout << "[Sender] 공개 키를 수신했습니다. 암호화를 진행합니다." << std::endl;

            // 3. 메시지 암호화
            std::vector<ECC::Elgamel::Ciphertext> ciphers = ECC::Elgamel::encrypt(message);

            // 4. 암호문 개수 전송
            size_t count = ciphers.size();
            WriteFile(hPipe, &count, sizeof(size_t), &bytesWritten, NULL);

            // 5. 암호문 데이터 직렬화 및 전송
            for (const auto& c : ciphers) {
                RawCipher raw;
                raw.c1 = { c.C1.getX().getValue(), c.C1.getY().getValue() };
                raw.c2 = { c.C2.getX().getValue(), c.C2.getY().getValue() };

                WriteFile(hPipe, &raw, sizeof(RawCipher), &bytesWritten, NULL);
            }

            std::cout << "[Sender] 메시지 전송 완료 (" << count << " 블록)" << std::endl;

            CloseHandle(hPipe);
        }
    }
}