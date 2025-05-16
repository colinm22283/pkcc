void myput(int x)
{
    putchar(65+x);
    putchar(10);
    return;
}

int main()
{
    int i;
    i = 3;
    for ( i=0 ; ; i = i + 1 ) {
        myput(i);
        if (i) {
            return 0;
        }
    }
    myput(33);
    return 1;
}