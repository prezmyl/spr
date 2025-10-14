#include <cstdint>
#include <iostream>

//bezestavove, static metody

class ByteSwapper {
private:
public:
    static int32_t reverse_bytes(int32_t x);
};

// byte prevod pomoci masku a bitovych operasi and, or a posunu >> <<
// prectypovani pred na beznamkove a po posunu na znamenkovue, kvuli bitovemu posunu doprava
int32_t ByteSwapper::reverse_bytes(int32_t x) {
    uint32_t u = static_cast<uint32_t>(x); // to unsigned
    uint32_t y =
        //byteswapp pres masku
        ((u & 0x000000FFu) << 24) |
        ((u & 0x0000FF00u) <<  8) |
        ((u & 0x00FF0000u) >>  8) |
        ((u & 0xFF000000u) >> 24);
    return static_cast<int32_t>(y);
}


class EndianApp {
private:
    static bool read_input(std::istream& is, int32_t& out);

public:
    void run(std::istream& is, std::ostream& os);

};

bool EndianApp::read_input(std::istream &is, int32_t &out) {
    return static_cast<bool>(is >> out);
}

void EndianApp::run(std::istream &is, std::ostream &os) {
    int32_t n;
    while (read_input(is, n)) {
        const int32_t m = ByteSwapper::reverse_bytes(n);
        os << n << " converts to " << m <<'\n';
    }
}

int main() {

    EndianApp app;
    app.run(std::cin, std::cout);
    return 0;
}
