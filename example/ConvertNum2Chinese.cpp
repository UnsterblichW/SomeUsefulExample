// 2025/06/12 面试中遇到的机考题目

// 该程序可以将浮点型数字转换为中文大写形式，支持整数和小数部分的转换。
// 注意：此代码在 Windows 环境下使用，可能需要调整以适应其他平台。
// 整数部分最大到亿，小数部分精度为6位。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

// 数字到中文大写的映射
std::string const numToChinese[10] = {"零", "壹", "贰", "叁", "肆",
                                      "伍", "陆", "柒", "捌", "玖"};
// 数位单位
std::string const unit[4] = {"", "拾", "佰", "仟"};
// 大数位单位
std::string const bigUnit[4] = {"", "万", "亿", "兆"};

// 将单个数字转换为中文大写
std::string numToChineseStr(int num) {
    if (num >= 0 && num <= 9) {
        return numToChinese[num];
    }
    return "";
}

// 处理四位数以内的数字转换
std::string processFourDigits(int num) {
    if (num == 0) {
        return "";
    }

    std::string result = "";
    bool hasNonZero = false;
    bool lastWasZero = false;

    for (int i = 0; i < 4; i++) {
        int digit = (num / static_cast<int>(pow(10, 3 - i))) % 10;
        if (digit != 0) {
            if (lastWasZero) {
                result += numToChinese[0];
            }
            result += numToChinese[digit];
            result += unit[3 - i];
            hasNonZero = true;
            lastWasZero = false;
        } else {
            lastWasZero = hasNonZero;
        }
    }

    return result;
}

std::string removeChineseZero(std::string& str) {
    while (str.length() > 1 && 
           str.substr(str.length() - numToChinese[0].length()) == numToChinese[0]) {
        str = str.substr(0, str.length() - numToChinese[0].length());
    }

    return str;
}

// 处理整数部分转换
std::string convertIntegerPart(long long integerPart) {
    if (integerPart == 0) {
        return numToChinese[0];
    }

    std::string result = "";
    int unitIndex = 0;
    bool hasNonZeroInBigUnit = false;

    while (integerPart > 0) {
        int fourDigits = integerPart % 10000;
        std::string fourDigitsStr = processFourDigits(fourDigits);

        if (!fourDigitsStr.empty()) {
            result = fourDigitsStr + bigUnit[unitIndex] + result;
            hasNonZeroInBigUnit = true;
        } else if (hasNonZeroInBigUnit && integerPart > 9999) {
            // 当前四位全为零，且前面有非零数字，添加"零"
            result = numToChinese[0] + result;
        }

        integerPart /= 10000;
        unitIndex++;

        // 超过亿位不再处理
        if (unitIndex >= 3) {
            break;
        }
    }

    return removeChineseZero(result);
}

// 处理小数部分转换
std::string convertDecimalPart(double decimalPart, int precision = 6) {
    std::string result = "点";
    // 转换为整数处理
    long long decimalInt =
        static_cast<long long>(decimalPart * pow(10, precision) + 0.5);

    // 转换为字符串，去除前导零
    std::string decimalStr = std::to_string(decimalInt);
    // 补齐到指定精度位数
    while (decimalStr.length() < precision) {
        decimalStr = "0" + decimalStr;
    }

    // 处理小数部分每一位
    for (char digit: decimalStr) {
        result += numToChinese[digit - '0'];
    }

    return removeChineseZero(result);
}

// 主转换函数
std::string convertToChinese(std::string numStr) {
    std::string result = "";
    
    // 处理负数
    if (numStr[0] == '-') {
        result = "负";
        numStr = numStr.substr(1);  // 去掉负号
    }

    // 从字符串中分离整数和小数部分
    size_t dotPos = numStr.find('.');
    long long integerPart = std::stoll(numStr.substr(0, dotPos));
    double decimalPart = dotPos == std::string::npos
                             ? 0.0
                             : std::stod("0" + numStr.substr(dotPos));

    // 处理整数部分
    result += convertIntegerPart(integerPart);

    // 处理小数部分
    if (decimalPart > 0) {
        result += convertDecimalPart(decimalPart);
    }

    return result;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // 测试示例
    std::cout << "\n测试示例：" << std::endl;

    std::vector<double> testNumbers = {
        // 120.00457, 34567.89, 100000000.123, 0.123456, 10020.003, -567.89
        10020.003
    };

    for (double& num : testNumbers) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << num;
        std::string numStr = oss.str();
        std::cout << numStr << " -> " << convertToChinese(numStr) << std::endl;
    }

    // std::cout << "120.00457 -> " << convertToChinese(120.00457) << std::endl;
    // std::cout << "34567.89 -> " << convertToChinese(34567.89) << std::endl;
    // std::cout << "100000000.123 -> " << convertToChinese(100000000.123) <<
    // std::endl; std::cout << "0.123456 -> " << convertToChinese(0.123456) <<
    // std::endl; std::cout << "10020.003 -> " << convertToChinese(10020.003) <<
    // std::endl; std::cout << "-567.89 -> " << convertToChinese(-567.89) <<
    // std::endl;

    return 0;
}
