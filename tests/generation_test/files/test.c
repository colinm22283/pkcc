
int main() {
    int i = 0 ? 5 : 15;

    for (; !(i < 0); i--) {
        if (i % 5 == 0) continue;

        putint(i);
        putchar('\n');
    }

    return 0;
}