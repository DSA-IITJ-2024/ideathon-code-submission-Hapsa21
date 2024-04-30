#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>

// Function to calculate (a^b) % c
long long power(long long a, long long b, long long c) {
    long long result = 1;
    a %= c;
    while (b) {
        if (b & 1)
            result = (result * a) % c;
        a = (a * a) % c;
        b >>= 1;
    }
    return result;
}

// Miller-Rabin primality test
bool isPrime(long long n, int k = 5) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    long long r = 0, d = n - 1;
    while (d % 2 == 0) {
        r++;
        d /= 2;
    }

    for (int i = 0; i < k; ++i) {
        long long a = 2 + rand() % (n - 4);
        long long x = power(a, d, n);
        if (x == 1 || x == n - 1) continue;

        for (long long j = 0; j < r - 1; ++j) {
            x = (x * x) % n;
            if (x == 1) return false;
            if (x == n - 1) break;
        }

        if (x != n - 1) return false;
    }
    return true;
}

// Function to find a random prime number of given bit length
long long generatePrime(int bits) {
    long long candidate;
    do {
        candidate = (1LL << (bits - 1)) + rand() % (1LL << (bits - 2));
    } while (!isPrime(candidate));
    return candidate;
}

// Function to calculate gcd(a, b)
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate modular inverse of a mod m
long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

// RSA Encryption
std::vector<long long> encrypt(const std::string& message, long long e, long long n) {
    std::vector<long long> encryptedMessage;
    for (char c : message) {
        encryptedMessage.push_back(power(c, e, n));
    }
    return encryptedMessage;
}

// RSA Decryption using Chinese Remainder Theorem (CRT)
long long decryptCRT(long long cipher, long long d, long long p, long long q) {
    long long dp = d % (p - 1);
    long long dq = d % (q - 1);
    long long qInv = modInverse(q, p);

    long long c1 = power(cipher % p, dp, p);
    long long c2 = power(cipher % q, dq, q);

    long long h = (qInv * (c1 - c2)) % p;
    if (h < 0) h += p;

    return (c2 + h * q) % (p * q);
}

int main() {
    srand(time(NULL));

    int bits = 8; // Change this to desired bit length for prime numbers
    long long p = generatePrime(bits);
    long long q = generatePrime(bits);

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e = 65537; // Common choice for e
    long long d = modInverse(e, phi);

    std::cout << "Public key (n, e): (" << n << ", " << e << ")" << std::endl;
    std::cout << "Private key (n, d): (" << n << ", " << d << ")" << std::endl;

    std::string message;
    std::cout << "Enter the message to encrypt: ";
    std::getline(std::cin >> std::ws, message);

    // Encrypt the message
    std::vector<long long> encryptedMessage = encrypt(message, e, n);

    std::cout << "Encrypted message: ";
    for (long long cipher : encryptedMessage) {
        std::cout << cipher << " ";
    }
    std::cout << std::endl;

    // Decrypt the message using CRT
    std::ostringstream decryptedMessage;
    for (long long cipher : encryptedMessage) {
        long long decryptedChar = decryptCRT(cipher, d, p, q);
        decryptedMessage << (char)decryptedChar;
    }

    std::cout << "Decrypted message: " << decryptedMessage.str() << std::endl;

    return 0;
}
