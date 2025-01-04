int test_function(int x, int y) {
    int z = 0;
    if (x > 0) {
        z = x + y;
    } else {
        z = x - y;
    }

    while (z < 100) {
        z += 10;
    }

    return z;
}