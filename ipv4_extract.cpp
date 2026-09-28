#include <iostream>
#include <string>

// Returns true if c is a character that can be part of a candidate token.
static bool isTokenChar(char c) {
    return (c >= '0' && c <= '9') || c == '.' || c == ':';
}

// This function 
// Enforces: at least one digit, no more than maxDigits, no leading zero unless the number is exactly "0", and value <= maxValue.
// when pass constraint, move pos past the digits and stores the value.
static bool readNumber(const std::string& str, size_t& pos, size_t end,
                       int maxDigits, long maxValue, long& value) {
    size_t start = pos;
    long result = 0;
    int digits = 0;
    // my own comment: this loop goes through the array contains the char inputed. make sure that it does not go out
// of range
    while (pos < end && str[pos] >= '0' && str[pos] <= '9') {
        digits++;
        if (digits > maxDigits) return false;      // prevents overflow
        result = result * 10 + (str[pos] - '0');
        pos++;
    }

    if (digits == 0) return false;                  // empty octet / empty port
    if (digits > 1 && str[start] == '0') return false; // leading zero not allowed
    if (result > maxValue) return false;            // out of range

    value = result;
    return true;
}

// Validates the run str[begin, end) against the FULL grammar:
//   octet.octet.octet.octet[:port]
// The entire run must be consumed, otherwise it is rejected.
static bool validateRun(const std::string& str, size_t begin, size_t end,
                        unsigned long& address, int& port) {
    size_t pos = begin;
    unsigned long addr = 0;
	// my comment: this loop check if input's is with in bound of the grammar
    for (int i = 0; i < 4; i++) {
        long octet = 0;
        if (!readNumber(str, pos, end, 3, 255, octet)) return false;
        addr = (addr << 8) | static_cast<unsigned long>(octet);

        if (i < 3) {// this check the input end with .
            if (pos >= end || str[pos] != '.') return false;
            pos++;
        }
    }

    int foundPort = -1;
    if (pos < end) {
        // Anything after the 4th octet must be ':' followed by a valid port.
        if (str[pos] != ':') return false;          // stray '.' or other trailing char
        pos++;
        long p = 0;
        if (!readNumber(str, pos, end, 5, 65535, p)) return false;
        foundPort = static_cast<int>(p);
    }

    if (pos != end) return false;                   // leftover chars (second colon, etc.)

    address = addr;
    port = foundPort;
    return true;
}

// this functioni returns true when valid address was found, false otherwise.
// Success: outAddress holds the 32-bit value, outPort holds the port or -1 if no port was present. 
// Fail: outAddress = 0, outPort = -1.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    size_t i = 0;
	//my comment: this loop go through the string, 1st make sure to ignore garbage sign
    while (i < str.size()) {
        if (!isTokenChar(str[i])) {
            i++;
            continue;
        }

        //my comment: after ignore invalid char, it will "save" the location of the first valid char, and go to the 
	//next char untill reach max valid char
        size_t j = i;
        while (j < str.size() && isTokenChar(str[j])) j++;

        unsigned long addr = 0;
        int port = -1;
        if (validateRun(str, i, j, addr, port)) {
            outAddress = addr;
            outPort = port;
            return true;
        }

        i = j; // resume scanning AFTER the failed run (never restart from the top)
    }

    return false;
}

int main() {
    std::string line;

    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        if (!std::getline(std::cin, line)) break;   // handle EOF gracefully

        if (line == "END") {
            std::cout << "Program terminated." << std::endl;
            break;
        }

        unsigned long address = 0;
        int port = -1;

        if (extractIPv4(line, address, port)) {
            std::cout << "Extracted IPv4 address: "
                      << ((address >> 24) & 0xFF) << "."
                      << ((address >> 16) & 0xFF) << "."
                      << ((address >> 8) & 0xFF) << "."
                      << (address & 0xFF)
                      << " (decimal value: " << address << ", port: ";
            if (port == -1) std::cout << "none";
            else std::cout << port;
            std::cout << ")" << std::endl;
        } else {
            std::cout << "Invalid input: no valid IPv4 address found" << std::endl;
        }
    }

    return 0;
}
