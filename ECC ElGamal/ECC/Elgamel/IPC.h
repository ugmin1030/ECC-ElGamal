#pragma once
#include <string>

namespace ECC {
    namespace IPC {
        // 메시지를 암호화하여 파이프로 전송
        void sender(const std::string& message);

        // 파이프를 생성하고 접속을 대기, 수신 후 복호화
        void receiver();
    }
}