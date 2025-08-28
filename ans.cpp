#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "nlohmann/json.hpp"

long long decodeYValue(const std::string& base_str, const std::string& value_str) {
    return std::stoll(value_str, nullptr, std::stoi(base_str));
}

double lagrangeInterpolation(const std::vector<std::pair<int, long long>>& data_points, double x_target) {
    double result = 0.0;
    int n = data_points.size();
    for (int i = 0; i < n; ++i) {
        double term = data_points[i].second;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term *= (x_target - data_points[j].first) / (double)(data_points[i].first - data_points[j].first);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    // using namespace std; // Removed this and explicitly used std:: for clarity

    // Read the root.json file
    std::ifstream i("root.json");
    nlohmann::json j; // nlohmann::json is correct
    i >> j;

    std::vector<std::pair<int, long long>> data_points;

    // Extract and decode data points
    for (auto it = j.begin(); it != j.end(); ++it) {
        if (it.key() != "keys") {
            int x = std::stoi(it.key());
            std::string base_str = it.value()["base"];
            std::string value_str = it.value()["value"];
            long long y = decodeYValue(base_str, value_str);
            data_points.push_back({x, y});
        }
    }

    // Sort data points by x-value to ensure consistent order for interpolation
    std::sort(data_points.begin(), data_points.end());

    // Get k value (degree of polynomial + 1)
    int k = j["keys"]["k"];

    // Select the first k data points for interpolation
    std::vector<std::pair<int, long long>> selected_data_points;
    for (int i = 0; i < k; ++i) {
        selected_data_points.push_back(data_points[i]);
    }

    // Calculate the constant term 'c' by evaluating the polynomial at x = 0
    double c_value = lagrangeInterpolation(selected_data_points, 0.0);

    std::cout << "The constant term c is: " << c_value << std::endl;

    return 0;
}