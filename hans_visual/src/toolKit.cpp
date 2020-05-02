#include "toolKit.hpp"

namespace toolKit
{

void convert_HSVtoRGB(float (*colors)[3], int num_colors)
{
    for (int i = 0; i < num_colors; i++)
        HSVtoRGB(colors[i][0], colors[i][1], colors[i][2], &colors[i][0]);
}

void convert_RGB255toRGB(float (*colors)[3], int num_colors)
{
    for (int i = 0; i < num_colors; i++)
    {
        colors[i][0] /= 255;
        colors[i][1] /= 255;
        colors[i][2] /= 255;
    }
}

void transform_coordinates(float (*points_arr)[3], int number_points)
{
    float temp;

    for (int i = 0; i < number_points; i++)
    {
        temp = points_arr[i][0];

        points_arr[i][0] = - points_arr[i][1];
        points_arr[i][1] =   points_arr[i][2];
        points_arr[i][2] = - temp;
    }
}

void polynomial_points(float (*result)[3], float min_x, float max_x, unsigned int sample_size, float *coefficients, unsigned int number_of_coefficients)
{
    if(min_x > max_x || sample_size == 0 || number_of_coefficients == 0) { std::cout << "Wrong polynomial input data" << std::endl; return; }

    float step = (max_x - min_x) / sample_size;

    for(size_t i = 0; i < sample_size; i++)
        polynomial_x(&result[i][0], min_x + step * i, coefficients, number_of_coefficients);

    polynomial_x(&result[sample_size][0], max_x, coefficients, number_of_coefficients);
}

void polynomial_segments(float (*result)[2][3], float min_x, float max_x, unsigned int sample_size, float *coefficients, unsigned int number_of_coefficients)
{
    if(min_x > max_x || sample_size == 0 || number_of_coefficients == 0) { std::cout << "Wrong polynomial input data" << std::endl; return; }

    float step = (max_x - min_x) / sample_size;

    polynomial_x(&result[0][0][0], min_x, coefficients, number_of_coefficients);

    for(size_t i = 1; i < sample_size; i++)
    {
        polynomial_x(&result[i][0][0], min_x + step * i, coefficients, number_of_coefficients);

        result[i-1][1][0] = result[i][0][0];
        result[i-1][1][1] = result[i][0][1];
        result[i-1][1][2] = result[i][0][2];
    }

    polynomial_x(&result[sample_size - 1][1][0], max_x, coefficients, number_of_coefficients);
}

void icosahedron(float side_length, float(*points)[3])
{
    float S = side_length;
    const double pi = 3.14159265359;
    float t1 = 2 * pi / 5;
    float t2 = (pi / 2) - t1;
    float t4 = t1 / 2;
    float t3 = t4 - pi / 10;

    float R = (S / 2) / std::sin(t4);
    float H = std::cos(t4) * R;
    float Cx = R * std::sin(t2);
    float Cz = R * std::cos(t2);
    float H1 = std::sqrt(S * S - R * R);
    float H2 = std::sqrt((H + R) * (H + R) - H * H);
    float Y2 = (H2 - H1) / 2;
    float Y1 = Y2 + H1;

    points[0][0] = 0.;
    points[0][1] = Y1;
    points[0][2] = 0.;

    points[1][0] = R;
    points[1][1] = Y2;
    points[1][2] = 0.;

    points[2][0] = Cx;
    points[2][1] = Y2;
    points[2][2] = Cz;

    points[3][0] = -H;
    points[3][1] = Y2;
    points[3][2] = S / 2;

    points[4][0] = -H;
    points[4][1] = Y2;
    points[4][2] = -S / 2;

    points[5][0] = Cx;
    points[5][1] = Y2;
    points[5][2] = -Cz;

    points[6][0] = -R;
    points[6][1] = -Y2;
    points[6][2] = 0.;

    points[7][0] = -Cx;
    points[7][1] = -Y2;
    points[7][2] = -Cz;

    points[8][0] = H;
    points[8][1] = -Y2;
    points[8][2] = -S / 2;

    points[9][0] = H;
    points[9][1] = -Y2;
    points[9][2] = S / 2;

    points[10][0] = -Cx;
    points[10][1] = -Y2;
    points[10][2] = Cz;

    points[11][0] = 0.;
    points[11][1] = -Y1;
    points[11][2] = 0.;
}

void fill_rainbow(float (*modified_rainbow)[3], float min, float max, bool inverted, float original_lvl, float new_lvl)
{
    for (int i = 0; i < 256; i++)
    {
        if (i <= new_lvl)           // Color range [0, 60]
        {
            modified_rainbow[i][0] = i * original_lvl/new_lvl;
            modified_rainbow[i][1] = 1.;
            modified_rainbow[i][2] = 1.;
        }
        else
        if (i > new_lvl)            // Color range (60, 240]
        {
            modified_rainbow[i][0] = original_lvl + (i-new_lvl) * (max-original_lvl)/(255.-new_lvl);
            modified_rainbow[i][1] = 1.;
            modified_rainbow[i][2] = 1.;
        }
    }
    //for(int a = 0; a < 256; a++) std::cout << a << ": " << modified_rainbow[a][0] << std::endl;
    convert_HSVtoRGB(modified_rainbow, 256);

    if(inverted) invert_array(modified_rainbow, 256);
}

void fill_polyline(float (*points)[3], size_t num_points, float (*destination)[2][3])
{
    for(int i = 0; i < num_points-1; ++i)
    {
        destination[i][0][0] = points[i][0];
        destination[i][0][1] = points[i][1];
        destination[i][0][2] = points[i][2];

        destination[i][1][0] = points[i+1][0];
        destination[i][1][1] = points[i+1][1];
        destination[i][1][2] = points[i+1][2];
    }
}

// Auxiliary functions ---------------------------------------------------------------

void polynomial_x(float *results, float x, float *coefficients, float number_of_coefficients)
{
    // X
    results[0] = x;
    // Y
    results[1] = coefficients[0];
    for(int i = 1; i < number_of_coefficients; i++)
        results[1] += coefficients[i] * pow(x, i);
    // Z
    results[2] = 0;
}

void HSVtoRGB(double H, double S, double V, float output[3])
{
    // https://gist.github.com/kuathadianto/200148f53616cbd226d993b400214a7f
    double C = S * V;
    double X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs, Gs, Bs;

    if (H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    }
    else if (H >= 60 && H < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    }
    else if (H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    }
    else if (H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    }
    else if (H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    }
    else {
        Rs = C;
        Gs = 0;
        Bs = X;
    }

    output[0] = (Rs + m);
    output[1] = (Gs + m);
    output[2] = (Bs + m);
}

void invert_array(float (*arr)[3], size_t siz)
{
    float temp[3];

    for(size_t i = 0; i < siz/2; ++i)
    {
        temp[0] = arr[i][0];
        temp[1] = arr[i][1];
        temp[2] = arr[i][2];

        arr[i][0] = arr[siz-1 - i][0];
        arr[i][1] = arr[siz-1 - i][1];
        arr[i][2] = arr[siz-1 - i][2];

        arr[siz-1 - i][0] = temp[0];
        arr[siz-1 - i][1] = temp[1];
        arr[siz-1 - i][2] = temp[2];
    }
}

}   // End of namespace
