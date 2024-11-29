#include <sycl/sycl.hpp>
#include <cstring>

const std::string secret = "Ifmmp-!xpsme \"\012J(n!tpssz-!Ebwf/!";
const size_t sz = secret.size();

int main() {
    sycl::queue q;
    char* result = sycl::malloc_shared<char>(sz, q);
    std::memcpy(result, secret.data(), sz);
    printf("%s\n", result);
    sycl::free(result, q);
    return 0;
}
