const float pi = 3.1415926535897932384626433; /* approximately */

struct point {
    int x, y;
};

struct rectangle {
    struct point upperleft;
    struct point lowerright;
};

struct window {
    struct rectangle area;
    char text[1024];
};

void display(const struct window W[], int n);

struct point strange(int z)
{
/*
Syntactically correct.
Will fail type checking later.
*/
    int y;
    struct mything {
        float a, b, c;
    };
    struct rectangle A;

    return A.lowerright;
}