<h1 align="center">Contiguous</h1>

<p align="center"><em>It's my first allocator ever • December 29, 2025</em></p>

<p align="center">
Probably my last project of 2025, wishing you a great end of year!<br>
It's not very impressive, but we all start somewhere, right?<br>
I plan to learn and understand all the allocators in order to create a small allocator library using TMP and CRTP Mixin ;)
</p>


```console
Max Point objects in memory: 2305843009213693951
(0,10)
(1,11)
(2,12)
(3,13)
(4,14)
(5,15)
(6,16)
(7,17)
(8,18)
(9,19)
```

```cpp
// Copyright (c) December 2025 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#include <iostream>

struct Point {
    int x, y;
    Point(int a, int b) : x(a), y(b) {}
    void print() { std::cout << "(" << x << "," << y << ")\n"; }
};

template<typename T>
struct Allocator {
    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(sizeof(T) * n));
    }

    void deallocate(T* ptr, std::size_t n) {
        ::operator delete(ptr);
    }

    template<typename... Args>
    void construct(T* ptr, Args&&... args) {
        new (ptr) T(std::forward<Args>(args)...);
    }

    void destroy(T* ptr) {
        ptr->~T();
    }

    std::size_t max_size() const {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }
};
```

```cpp
int main() {
    std::size_t n = 10;

    Allocator<Point> alloc;

    Point* base = alloc.allocate(n);

    std::cout << "Max Point objects in memory: "
              << alloc.max_size() << "\n";

    for (std::size_t i = 0; i < n; i++)
        alloc.construct(base + i, i, i + 10);

    for (std::size_t i = 0; i < n; i++)
        base[i].print();

    for (std::size_t i = 0; i < n; i++)
        alloc.destroy(base);

    alloc.deallocate(base, n);
}
```
