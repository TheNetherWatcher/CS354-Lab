#include <iostream>
#include <vector>
#include <iomanip> // for setw and setprecision
using namespace std;

// Define the weight matrix globally
vector<vector<int>> w(4, vector<int>(4, 0));

// Function to construct the weight matrix
void construct_weights(const vector<int>& d) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i != j) {
                int val_i = 2 * d[i] - 1;
                int val_j = 2 * d[j] - 1;
                w[i][j] = val_i * val_j;
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
            int y_temp = input_vector[i];  // From original input each time

            for (int j = 0; j < 4; ++j) {
                if (i != j) {
                    y_temp += w[i][j] * y[j];
                }
            }

            if (y_temp > 0) y[i] = 1;
            else if (y_temp < 0) y[i] = 0;
            // If y_temp == 0, retain previous value
        }

        if (y == y_prev) break;
        y_prev = y;
    }

    return {y, count};
}

// Helper function to print vectors
void print_vector(const vector<int>& v) {
    for (int val : v) {
        cout << val << " ";
    }
    cout << endl;
}

// Main function
int main() {
    vector<int> d = {1, 1, 1, 0};
    construct_weights(d);

    // Print weight matrix nicely
    cout << "Weight matrix:\n";
    for (const auto& row : w) {
        for (int val : row) {
            cout << setw(4) << val << " ";
        }
        cout << endl;
    }

    // Test inputs
    vector<int> x;

    x = {1, 0, 0, 0};
    auto [result1, count1] = hopfield_network(x);
    cout << "Final output for input (1, 0, 0, 0): ";
    print_vector(result1);
    cout << "Iterations: " << count1 << "\n";

    x = {0, 0, 1, 0};
    auto [result2, count2] = hopfield_network(x);
    cout << "Final output for input (0, 0, 1, 0): ";
    print_vector(result2);
    cout << "Iterations: " << count2 << "\n";

    x = {1, 1, 1, 0};
    auto [result3, count3] = hopfield_network(x);
    cout << "Final output for input (1, 1, 1, 0): ";
    print_vector(result3);
    cout << "Iterations: " << count3 << "\n";

    return 0;
}
