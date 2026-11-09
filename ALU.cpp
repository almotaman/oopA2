#include "ALU.h"
#include <sstream>
#include <iomanip>
#include <bitset>
#include <cmath>
#include <cstdint>

static int hexToDecimal(const std::string& hexStr) {
    int val;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> val;
    return val;
}

static std::string decimalToHex(int val) {
    val &= 0xFF;
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << val;
    return ss.str();
}

std::string ALU::addTwosComplement(std::string val1, std::string val2) {
    int num1 = hexToDecimal(val1);
    int num2 = hexToDecimal(val2);

    if (num1 & 0x80) num1 -= 256;
    if (num2 & 0x80) num2 -= 256;

    int sum = num1 + num2;
    return decimalToHex(sum);
}

std::string ALU::addFloatingPoint(std::string val1, std::string val2) {
    auto decodeFloat = [](uint8_t hexVal) {
        int sign = (hexVal >> 7) & 0x01;
        int exponent = (hexVal >> 4) & 0x07;
        int mantissa = hexVal & 0x0F;
        
        double m = (double)mantissa / 16.0;
        if (sign) m = -m;
        
        return m * std::pow(2.0, exponent - 4);
    };

    uint8_t b1 = hexToDecimal(val1);
    uint8_t b2 = hexToDecimal(val2);

    double d1 = decodeFloat(b1);
    double d2 = decodeFloat(b2);
    double result = d1 + d2;

    int sign = 0;
    if (result < 0) {
        sign = 1;
        result = -result;
    }

    int exponent = 0;
    if (result > 0) {
        while (result < 0.5 && exponent > 0) {
            result *= 2.0;
            exponent--;
        }
        while (result >= 1.0 && exponent < 7) {
            result /= 2.0;
            exponent++;
        }
    }

    int mantissa = (int)(result * 16.0 + 0.5);
    if (mantissa > 15) mantissa = 15;

    uint8_t finalByte = (sign << 7) | ((exponent & 0x07) << 4) | (mantissa & 0x0F);
    return decimalToHex(finalByte);
}

std::string ALU::bitwiseOR(std::string val1, std::string val2) {
    int v1 = hexToDecimal(val1);
    int v2 = hexToDecimal(val2);
    return decimalToHex(v1 | v2);
}

std::string ALU::bitwiseAND(std::string val1, std::string val2) {
    int v1 = hexToDecimal(val1);
    int v2 = hexToDecimal(val2);
    return decimalToHex(v1 & v2);
}

std::string ALU::bitwiseXOR(std::string val1, std::string val2) {
    int v1 = hexToDecimal(val1);
    int v2 = hexToDecimal(val2);
    return decimalToHex(v1 ^ v2);
}

std::string ALU::rotateRight(std::string val, int steps) {
    int v = hexToDecimal(val);
    steps %= 8;
    for (int i = 0; i < steps; ++i) {
        int lsb = v & 1;
        v >>= 1;
        if (lsb) {
            v |= 0x80;
        }
    }
    return decimalToHex(v);
}

bool ALU::checkEqual(std::string val1, std::string val2) {
    return hexToDecimal(val1) == hexToDecimal(val2);
}