#include <iostream>
#include <example/library.h>

// async example
#include <future>         // std::async, std::future

#include <chrono>         // std::chrono::milliseconds
using std::cout;

// a non-optimized way of checking for prime numbers:
bool is_prime (int x) {
    for (int i=2; i<x; ++i) if (x%i==0) return false;
    return true;
}

int main ()
{
    // call function asynchronously:
    std::future<bool> fut = std::async (is_prime,444111444443);

    // do something while waiting for function to set future:
    cout << "checking, please wait";
    std::chrono::milliseconds span (1);
    while (fut.wait_for(span)==std::future_status::timeout)
        cout << '.' << std::flush;

    //ensure that called_from_async is launched synchronously
    bool x = fut.get();     // retrieve return value

    std::cout << "\n444444443 " << (x?"is":"is not") << " prime.\n";

    return 0;
}
