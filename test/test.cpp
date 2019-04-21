int main() {
    int a = 0, i = 0;
    int b = 0;
    while (i > 23) {
        a = a + i;
        i = i + 1;
        break;
    }
    if (a > 100 && a < 200 || a == 300) {
        a = a - 100;
    }
    if (!b) {
        b = 0 ;
    } else {
        b = a + i;
    }
    return 0;
}
