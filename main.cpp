#include <iostream>

// new (adresse) Type(constructor_args…)

// Une idée comme ça:
//     faire un système d'allocator avec Mixins
//     et CRTP pour pouvoir swap et composer ;)

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

int main() {
    {
        auto a = 1.34534554645645643535;

        void* mem = ::operator new(100);

        ::operator delete(mem);

        //std::cout << sizeof(a) << "\n";
    }

    {
        void* p_mem = ::operator new(sizeof(Point));

        Point* p1 = new (p_mem) Point(10, 10);

        //p1->print();

        p1->~Point();
        ::operator delete(p_mem);
    }

    {
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
}
