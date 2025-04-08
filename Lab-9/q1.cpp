#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Define the weight matrix globally for simplicity
vector<vector<double>> w(4, vector<double>(4, 0.0));

// Function to construct the weight matrix
void construct_weights(const vector<int>& d1, const vector<int>& d2) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i != j) {
                int val1 = (2 * d1[i] - 1) * (2 * d1[j] - 1);
                int val2 = (2 * d2[i] - 1) * (2 * d2[j] - 1);
                w[i][j] = 0.5 * (val1 + val2);
            }
        }
    }
}

// Hopfield network simulation
pair<vector<int>, int> hopfield_network(const vector<int>& input_vector) {
    vector<int> y = input_vector;
    vector<int> y_prev = input_vector;
    int count = 0;

    for (int iter = 0; iter < 10; ++iter) {
        count++;

        for (int i = 0; i < 4; ++i) {
            double y_temp = input_vector[i];
            for (int j = 0; j < 4; ++j) {
                if (i != j) {
                    y_temp += w[i][j] * y[j];
                }
            }

            if (y_temp > 0) y[i] = 1;
            else if (y_temp < 0) y[i] = 0;
            // if y_temp == 0, keep previous y[i]
        }

        if (y == y_prev) break;
        y_prev = y;
    }

    return {y, count};
}

// Helper function to print vector
void print_vector(const vector<int>& v) {
    for (int val : v) {
        cout << val << " ";
    }
}

int main() {
    vector<int> d1 = {1, 1, 1, 1};
    vector<int> d2 = {1, 1, 0, 0};

    construct_weights(d1, d2);

    cout << "Weight matrix:" << endl;
    for (const auto& row : w) {
        for (double val : row) {
            cout << fixed << setprecision(2) << setw(6) << val << " ";
        }
        cout << endl;
    }

    vector<int> x;

    x = {1, 1, 1, 1};
    auto [result1, count1] = hopfield_network(x);
    cout << "Final output for input (1, 1, 1, 1): ";
    print_vector(result1);
    cout << "Iterations: " << count1 << endl;

    x = {1, 1, 0, 0};
    auto [result2, count2] = hopfield_network(x);
    cout << "Final output for input (1, 1, 0, 0): ";
    print_vector(result2);
    cout << "Iterations: " << count2 << endl;

    x = {1, 1, 1, 0};
    auto [result3, count3] = hopfield_network(x);
    cout << "Final output for input (1, 1, 1, 0): ";
    print_vector(result3);
    cout << "Iterations: " << count3 << endl;

    return 0;
}