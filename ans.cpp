#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
using boost::multiprecision::cpp_int;

cpp_int toDec(const string& s, int base) {
    cpp_int res = 0, p = 1;
    string t = s;
    reverse(t.begin(), t.end());

    for (char c : t) {
        int d;
        if (c >= '0' && c <= '9') d = c - '0';
        else if (c >= 'a' && c <= 'z') d = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') d = c - 'A' + 10;
        else throw runtime_error("bad char");

        if (d >= base) throw runtime_error("digit out of range");
        res += d * p;
        p *= base;
    }
    return res;
}

cpp_int lagrange(const vector<pair<cpp_int, cpp_int>>& pts, cpp_int x0) {
    cpp_int res = 0;
    int n = pts.size();

    for (int i = 0; i < n; i++) {
        cpp_int term = pts[i].second;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            if (pts[i].first == pts[j].first) 
                throw runtime_error("duplicate x");
            term *= (x0 - pts[j].first);
            term /= (pts[i].first - pts[j].first);
        }
        res += term;
    }
    return res;
}

int main() {
    ifstream in("testcase.json");
    if (!in.is_open()) {
        cerr << "Error: no testcase.json" << endl;
        return 1;
    }
    json j;
    in >> j;

    int k = j["keys"]["k"];
    vector<pair<cpp_int, cpp_int>> pts;

    for (auto& [key, val] : j.items()) {
        if (key == "keys") continue;
        cpp_int x = cpp_int(key);
        int b = stoi(val["base"].get<string>());
        string v = val["value"].get<string>();
        cpp_int y = toDec(v, b);
        pts.push_back({x, y});
        if ((int)pts.size() == k) break;
    }

    sort(pts.begin(), pts.end(), [](auto& a, auto& b) { return a.first < b.first; });

    cpp_int C = lagrange(pts, 0);
    cout << "Secret C: " << C << endl;
    return 0;
}
