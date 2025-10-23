void myput(int x)
{
    putchar(65+x);
    putchar(10);
    return;
}

int main()
{
    float a = 40;

    int i;
    i = 3;
    for ( i=0 ; ; i = i + 1 ) {
        myput(i);
        if (i == 10) {
            break;
        }
    }
    myput(33);

    putint(10);

    return 1;
}
