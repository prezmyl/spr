#include <iostream>
#include <string>

using namespace std;

// standard LCD segments ->
// indices as: 0=top,1=ul,2=ur,3=mid,4=ll,5=lr,6=bot
const int SEG[10] = {
    (1<<0)|(1<<1)|(1<<2)|(1<<4)|(1<<5)|(1<<6),             // 0
    (1<<2)|(1<<5),                                         // 1
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<6),                    // 2
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                    // 3
    (1<<1)|(1<<2)|(1<<3)|(1<<5),                           // 4
    (1<<0)|(1<<1)|(1<<3)|(1<<5)|(1<<6),                    // 5
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<5)|(1<<6),             // 6
    (1<<0)|(1<<2)|(1<<5),                                  // 7
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),      // 8
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)              // 9
};

bool on(int d, int seg) { return (SEG[d] & (1<<seg)) != 0; }
/*      1<<seg
        1 << 0 = ...00000001 (1)
        1 << 1 = ...00000010 (2)
        1 << 2 = ...00000100 (4)
        1 << 3 = ...00001000 (8)
        1 << 4 = ...00010000 (16)
        1 << 5 = ...00100000 (32)
        1 << 6 = ...01000000 (64)
        */

void render_row_sep(const string& num, int s, int seg) {
    for (size_t i = 0; i < num.size(); ++i) {
        int d = num[i] - '0'; //str to int pres ascii kod
        cout << ' ' << string(s, on(d, seg) ? '-' : ' ') << ' ';
        if (i + 1 != num.size()) cout << ' ';
    }
    cout << "\n";
}

void render_row_vert(const string& num, int s, int leftSeg, int rightSeg) {
    for (size_t i = 0; i < num.size(); ++i) {
        int d = num[i] - '0';
        cout << (on(d, leftSeg) ? '|' : ' ')
             << string(s, ' ')
             << (on(d, rightSeg) ? '|' : ' ');
        if (i + 1 != num.size()) cout << ' ';
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s;
    string num;
    while (cin >> s >> num) {
        if (s == 0 && num == "0") break;
//segmenty: 0=top,1=ul,2=ur,3=mid,4=ll,5=lr,6=bot
        render_row_sep(num, s, 0);   // top
        for (int i = 0; i < s; ++i) render_row_vert(num, s, 1, 2); // upper l/r
        render_row_sep(num, s, 3);   // middle
        for (int i = 0; i < s; ++i) render_row_vert(num, s, 4, 5); // lower l/r
        render_row_sep(num, s, 6);   // bottom

        cout << "\n";
    }
    return 0;
}
