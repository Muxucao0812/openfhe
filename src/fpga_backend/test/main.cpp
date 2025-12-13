#include <iostream>


void arithmetic_test();
void interleave_test();
void auto_test();

int main() {
    std::cout << "Running Arithmetic Module Tests..." << std::endl;
    arithmetic_test(); // 编译器现在知道这个函数长什么样了
    std::cout << "Finished Arithmetic Module Tests." << std::endl;

    std::cout << "Running InterLeave Tests..." << std::endl;
    interleave_test();
    std::cout << "Finished InterLeave Tests." << std::endl;

    // std::cout << "Running Auto Tests..." << std::endl;
    // auto_test();
    // std::cout << "Finished Auto Tests." << std::endl;

    return 0;
}