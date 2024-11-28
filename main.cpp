#include <sycl/sycl.hpp>
#include <iostream>
#include <array> // For std::size

using namespace sycl;

int main() {
    // Hard-coded array
    int host_array[] = {2, 7, 1, 5, 0, 9, 3, 6, 1};

    // Dynamically compute the size of the array
    const int n = std::size(host_array);

    queue Q;

    std::cout << "\nSelected device: " << Q.get_device().get_info<info::device::name>() << "\n";

    // Buffers
    buffer<int> A(host_array, range<1>(n)); // Initialize buffer with the hard-coded array
    buffer<int> sum_buf{1};                // Buffer for the sum of squares (single element)

    // Initialize sum_buf to 0
    {
        host_accessor sum_init(sum_buf, write_only);
        sum_init[0] = 0;
    }

    // Submit kernel to compute squares and sum them up
    Q.submit([&](handler& h) {
        accessor A_acc(A, h, read_only);       // Read-only access to the array buffer
        accessor sum_acc(sum_buf, h, read_write); // Read/write access for sum buffer

        h.parallel_for(range<1>(n), [=](id<1> idx) {
            int i = idx[0];
            int square = A_acc[i] * A_acc[i]; // Compute square of the element
            // Atomically add to sum buffer
            atomic_ref<int, memory_order::relaxed, memory_scope::device,
           	    access::address_space::global_space> atomic_sum(sum_acc[0]);
            atomic_sum.fetch_add(square);
        });
    });

    // Read back the sum result and print
    {
        host_accessor sum_host(sum_buf, read_only);
        std::cout << "Sum of squares: " << sum_host[0] << "\n";
    }

    // Read back the array and print squares
    std::cout << "Squares of elements:\n";
    {
        host_accessor result(A, read_only);
        for (int i = 0; i < n; i++) {
            std::cout << "Element: " << result[i] << ", Square: " << result[i] * result[i] << "\n";
        }
    }

    return 0;
}

