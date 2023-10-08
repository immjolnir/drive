int* f() {
    int i = 42;
    int* p = &i;
    return p;
}

int g(int* p) { return *p; }

int main() { return g(f()); }

