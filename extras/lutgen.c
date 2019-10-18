#include <limits.h>
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// http://www.ilikebigbits.com/2017_06_01_float_or_double.html

#ifndef M_PI
  #define M_PI  3.14159265358979323846f
#endif

void fsincos(const float lut[], size_t lut_size, float angle, float *sin, float *cos)
{
    const size_t lut_size_4th = lut_size / 4;
    const float lut_over_twice_pi = (float)lut_size / (float)(2.0f * M_PI);

    size_t index = (size_t)(angle * lut_over_twice_pi);
    *sin = lut[index % lut_size];
    *cos = lut[(index + lut_size_4th) % lut_size]; // cos(a) = sin(a + pi/4)
}

void generate_lut(float lut[], size_t lut_size)
{
    const size_t lut_size_4th = lut_size / 4;
    for (size_t i = 0; i < lut_size + lut_size_4th; ++i) {
        float angle = (float)(2.0f * M_PI) * (float)i / (float)lut_size;
        float s = sinf(angle);
        if (fabs(s - 0.0f) <= __FLT_EPSILON__)
        {
            lut[i] = 0.0f;
        }
        else if (fabs(s - 1.0f) <= __FLT_EPSILON__)
        {
            lut[i] = 1.0f;
        }
        else if (fabs(s + 1.0f) <= __FLT_EPSILON__)
        {
            lut[i] = -1.0f;
        }
        else
        {
            lut[i] = s;
        }
    }
}

void test_lut_sin(size_t iterations, const float lut[], size_t lut_size)
{
    const float lut_over_twice_pi = (float)lut_size / (float)(2.0f * M_PI);

    clock_t s = clock();
    for (int i = iterations; i; --i)
    {
        float angle = ((float)rand() / (float)RAND_MAX) * (float)(2.0f * M_PI);
        size_t index = (size_t)(angle * lut_over_twice_pi);
        float s = lut[index];
    }
    clock_t e = clock();
    printf("%f\n", (float)(e - s) / (float)CLOCKS_PER_SEC);
}

void test_lut_sincos(size_t iterations, const float lut[], size_t lut_size)
{
    clock_t s = clock();
    for (int i = iterations; i; --i) {
        float angle = ((float)rand() / (float)RAND_MAX) * (float)(2.0f * M_PI);
        float s, c;
        fsincos(lut, lut_size, angle, &s, &c);
    }
    clock_t e = clock();
    printf("%f\n", (float)(e - s) / (float)CLOCKS_PER_SEC);
}

void test_trig_sin(size_t iterations)
{
    clock_t s = clock();
    for (int i = iterations; i; --i)
    {
        float angle = ((float)rand() / (float)RAND_MAX) * (float)(2.0f * M_PI);
        float s = sinf(angle);
    }
    clock_t e = clock();
    printf("%f\n", (float)(e - s) / (float)CLOCKS_PER_SEC);
}

void test_trig_sincos(size_t iterations)
{
    clock_t s = clock();
    for (int i = iterations; i; --i)
    {
        float angle = ((float)rand() / (float)RAND_MAX) * (float)(2.0f * M_PI);
        float s = sinf(angle);
        float c = cosf(angle);
    }
    clock_t e = clock();
    printf("%f\n", (float)(e - s) / (float)CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    const size_t lut_size = strtoul(argv[1], NULL, 0);
    const size_t lut_size_4th = lut_size / 4;
    const float lut_over_twice_pi = (float)lut_size / (float)(2.0f * M_PI);

    float lut[lut_size + lut_size_4th];
    generate_lut(lut, lut_size);

    printf("#include <stddef.h>\n");
    printf("\n");
    printf("static const float _lut[%lu] = {\n", lut_size + lut_size_4th);
    for (int i = 0; i < lut_size + lut_size_4th; ++i) {
        printf("    %.9ff, /* [%d] */\n", lut[i], i);
    }
    printf("};\n");
    printf("\n");
    printf("void fsincos(int angle, float *sin, float *cos)\n");
    printf("{\n");
    printf("    const int index = angle & 0x%lx;\n", lut_size - 1);
    printf("    *sin = _lut[index];\n");
    printf("    *cos = _lut[index + 0x%lx];\n", lut_size_4th);
    printf("}\n");
    printf("\n");
    printf("int fatoi(float angle)\n");
    printf("{\n");
    printf("    return (int)(angle * 81.487327576f) & 0x1ff;\n");
    printf("}\n");


    srand(time(NULL));
    test_trig_sin(10000000);
    test_lut_sin(10000000, lut, lut_size);
    test_trig_sincos(10000000);
    test_lut_sincos(10000000, lut, lut_size);

    return 0;
}
