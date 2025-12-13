#include "../include/arithmetic.h"

// Write a test file for arithmetic.h functions
#include <iostream>
#include <cassert>

void test_AddMod() {
    for (int i = 0; i < LIMB; ++i) {
        for (int j = 0; j < RING_DIM; ++j) {
            uint64_t a = rand() % MODULUS[i];
            uint64_t b = rand() % MODULUS[i];
            uint64_t mod = MODULUS[i];

            // Test addition
            uint64_t res_add = a;
            AddMod(res_add, b, mod, true);
            uint64_t expected_add = (a + b) % mod;
            assert(res_add == expected_add);

            // Test subtraction
            uint64_t res_sub = a;
            AddMod(res_sub, b, mod, false);
            uint64_t expected_sub = (a >= b) ? (a - b) % mod : (mod + a - b) % mod;
            assert(res_sub == expected_sub);
        }
    }
    std::cout << "========================" << std::endl;
    std::cout << "AddMod tests passed!" << std::endl;
    std::cout << "========================" << std::endl;
}


void test_Karatsuba() {
    for (int i = 0; i < LIMB; ++i) {
        for (int j = 0; j < RING_DIM; ++j) {
            uint64_t a = rand() % MODULUS[i];
            uint64_t b = rand() % MODULUS[i];

            uint128_t res;
            Karatsuba(a,b,res);
            uint128_t expected = (uint128_t)a * (uint128_t)b;

            assert(res == expected);
        }
    }

    std::cout << "========================" << std::endl;
    std::cout << "Karatsuba tests passed!" << std::endl;
    std::cout << "========================" << std::endl;

}

void test_MultMod() {
    for (int i = 0; i < LIMB; ++i) {
        for (int j = 0; j < RING_DIM; ++j) {
            uint64_t a = rand() % MODULUS[i];
            uint64_t b = rand() % MODULUS[i];
            uint64_t mod = MODULUS[i];
    

            uint64_t res;
            MultMod(a,b,res,i);
            uint64_t expected = (uint64_t)(((uint128_t)a * (uint128_t)b) % mod);

            assert(res == expected);
        }
    }

    std::cout << "========================" << std::endl;
    std::cout << "MultMod tests passed!" << std::endl;
    std::cout << "========================" << std::endl;

}

void arithmetic_test() {
    std::cout << "Starting arithmetic tests..." << std::endl;
    test_AddMod();
    test_Karatsuba();
    test_MultMod();
    std::cout << "All arithmetic tests passed!" << std::endl;
}