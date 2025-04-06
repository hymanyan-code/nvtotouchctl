
#include <stdint.h>




void Delayms(uint32_t ms) {
    for (; ms > 0; ms--) {
        for (volatile uint32_t j = 12500; j > 0; j--); // 12500 * 4 = 50,000周期
    }
}

void Delayus(uint32_t us) {
    for (; us > 0; us--) {
        for (volatile uint32_t j = 3; j > 0; j--);    // 12 * 4 = 48周期（近似50周期）
    }

}

