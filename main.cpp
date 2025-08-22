#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

long long baseToDecimal(const string &val, int base) {
    long long result = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = 10 + (tolower(c) - 'a');
        result = result * base + digit;
    }
    return result;
}

vector<long double> gaussianSolve(vector<vector<long double>> A) {
    int n = A.size();
    int m = A[0].size()-1;

    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i+1; j < n; j++) {
            if (fabsl(A[j][i]) > fabsl(A[pivot][i])) pivot = j;
        }
        swap(A[i], A[pivot]);

        long double div = A[i][i];
        if (fabsl(div) < 1e-15) continue;
        for (int j = i; j <= m; j++) A[i][j] /= div;

        for (int k = 0; k < n; k++) {
            if (k == i) continue;
            long double factor = A[k][i];
            for (int j = i; j <= m; j++) {
                A[k][j] -= factor * A[i][j];
            }
        }
    }

    vector<long double> sol(m);
    for (int i = 0; i < m; i++) sol[i] = A[i][m];
    return sol;
}

int main() {
    ifstream file("input.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open input.json\n";
        return 1;
    }

    json data;
    file >> data;

    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    vector<pair<long double,long double>> points;

    int count = 0;
    for (auto& el : data.items()) {
        if (el.key() == "keys") continue;

        int x = stoi(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string val = el.value()["value"].get<string>();
        long long y = baseToDecimal(val, base);

        points.push_back({x, (long double)y});
        count++;
        if (count == k) break;
    }

    int degree = k - 1;

    vector<vector<long double>> A(k, vector<long double>(k+1));
    for (int i = 0; i < k; i++) {
        long double xi = points[i].first;
        long double yi = points[i].second;

        long double power = 1;
        for (int j = 0; j <= degree; j++) {
            A[i][j] = power;
            power *= xi;
        }
        A[i][degree+1] = yi;
    }

    vector<long double> coeffs = gaussianSolve(A);

    cout << fixed << setprecision(0);
    cout << coeffs[0] << endl;

    return 0;
}
