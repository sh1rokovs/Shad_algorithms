#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <iomanip>
#include <cmath>

constexpr double eps = 1e-6;

struct TPoint {
    int x_, y_;
};

double arg_gap(double radius, TPoint point) {
    return std::sqrt(std::fabs(radius * radius - point.y_ * point.y_));
}

std::pair<double, double> including_gap(double radius, TPoint point) {
    double gap = arg_gap(radius, point);
    return std::make_pair(point.x_ - gap, point.x_ + gap);
}

bool contains_k(double radius, const std::vector<TPoint> &points, ssize_t k_) {
    std::vector<std::pair<double, bool>> extreme_points;
    extreme_points.reserve(2 * points.size());
    for (TPoint point : points) {
        if (std::abs(point.y_) < radius + eps) {
            auto [lep, rep] = including_gap(radius, point);
            extreme_points.emplace_back(lep, false);
            extreme_points.emplace_back(rep, true);
        }
    }
    std::sort(extreme_points.begin(), extreme_points.end());

    ssize_t counter = 0;
    for (const auto &[arg, out] : extreme_points) {
        counter += out ? -1 : 1;
        if (counter == k_) {
            return true;
        }
    }
    return false;
}

double smallest_disc_rad(const std::vector<TPoint> &points, size_t k_) {
    constexpr double lb = 0., rb = std::sqrt(2 * 2000. * 2000.);
    double median = (lb + rb) / 2.;
    for (double lhs = lb, rhs = rb; lhs + eps < rhs; median = (lhs + rhs) / 2.) {
        if (contains_k(median, points, k_)) {
            rhs = median;
        } else {
            lhs = median;
        }
    }
    return median;
}

int main() {
    size_t n_, k_;
    std::cin >> n_ >> k_;
    std::vector<TPoint> points(n_);
    for (TPoint &item : points) {
        std::cin >> item.x_ >> item.y_;
    }

    std::cout << std::setprecision(6) << std::fixed << smallest_disc_rad(points, k_) << std::endl;
    return 0;
}