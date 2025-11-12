#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <numeric>
#include <algorithm>

using namespace std;

struct Circle {
    double x, y, r;

    [[nodiscard]] bool contains(double px, double py) const {
        double dx = px - x, dy = py - y;
        return dx * dx + dy * dy <= r * r;
    }
};

double exact_area() {
    return 0.25 * M_PI + 1.25 * asin(0.8) - 1.0;
}

double monte_carlo_area(const vector<Circle> &circles,
                        double x_min, double x_max,
                        double y_min, double y_max,
                        int64_t N) {
    mt19937_64 rng(32142141);
    uniform_real_distribution dist_x(x_min, x_max);
    uniform_real_distribution dist_y(y_min, y_max);

    int64_t M = 0;
    for (int64_t i = 0; i < N; ++i) {
        double x = dist_x(rng);
        double y = dist_y(rng);

        bool inside_all = true;
        for (const auto &circle: circles) {
            if (!circle.contains(x, y)) {
                inside_all = false;
                break;
            }
        }

        if (inside_all) {
            ++M;
        }
    }

    double res_c = (x_max - x_min) * (y_max - y_min);
    return static_cast<double>(M) / N * res_c;
}

struct Region {
    string name;
    double x_min, x_max, y_min, y_max;
};

int main() {
    vector<Circle> circles = {
        {1.0, 1.0, 1.0},
        {1.5, 2.0, sqrt(5.0) / 2},
        {2.0, 1.5, sqrt(5.0) / 2}
    };

    vector<Region> regions = {
        {"wide", 0.0, 3.0, 0.0, 3.0},
        {"narrow", 0.8, 2.1, 0.8, 2.1}
    };

    double exact = exact_area();

    const int64_t N_min = 100;
    const int64_t N_max = 100000;
    const int64_t step = 500;

    ofstream data_file("results.csv");
    data_file << "region,N,exact_area,estimated_area,relative_error,absolute_error\n";

    for (const auto &region: regions) {
        for (int64_t N = N_min; N <= N_max; N += step) {
            double estimate = monte_carlo_area(circles,
                                               region.x_min, region.x_max,
                                               region.y_min, region.y_max,
                                               N);

            double absolute_error = abs(estimate - exact);
            double relative_error = absolute_error / exact;

            data_file << region.name << ","
                    << N << ","
                    << exact << ","
                    << estimate << ","
                    << relative_error << ","
                    << absolute_error << "\n";
        }
    }

    data_file.close();
    return 0;
}
