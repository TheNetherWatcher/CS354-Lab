#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Perceptron {
    vector<double> weights;
    double eta;  // Learning rate
    int theta;   // Threshold
    int bias_ip; // Bias input

    Perceptron(double eta, int theta, int input_size) {
        this->eta = eta;
        this->theta = theta;
        this->bias_ip = 1;
        weights = vector<double>(input_size + 1, 0); // Initializing weights including bias weight
    }

    void train(const vector<pair<vector<int>, int>> &data) {
        bool flag = true;
        int epoch = 0;

        while (flag) {
            flag = false;
            for (const auto &sample : data) {
                const vector<int> &inputs = sample.first;
                int target = sample.second;
                double net = weights[0] * bias_ip;
                for (size_t i = 0; i < inputs.size(); ++i) {
                    net += weights[i + 1] * inputs[i];
                }
                int output = (net >= theta) ? 1 : 0;

                int error = target - output;
                if (error != 0) {
                    flag = true;
                    weights[0] += eta * error * bias_ip;
                    for (size_t i = 0; i < inputs.size(); ++i) {
                        weights[i + 1] += eta * error * inputs[i];
                    }
                }
            }
            epoch++;
        }
    }

    int predict(const vector<int> &inputs) {
        double net = weights[0] * bias_ip;
        for (size_t i = 0; i < inputs.size(); ++i) {
            net += weights[i + 1] * inputs[i];
        }
        return (net >= theta) ? 1 : 0;
    }
};

vector<pair<vector<int>, int>> load_data(const string &filename) {
    vector<pair<vector<int>, int>> data;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> inputs;
        int input, target;

        while (ss >> input) {
            inputs.push_back(input);
        }
        target = inputs.back();
        inputs.pop_back();
        data.push_back({inputs, target});
    }

    return data;
}

int main() {
    int choice;
    cout << "Select the gate to train the perceptron model:\n";
    cout << "1. AND\n";
    cout << "2. NAND\n";
    cout << "3. OR\n";
    cout << "4. NOR\n";
    cout << "Enter your choice: ";
    cin >> choice;

    string filename;
    switch (choice) {
        case 1:
            filename = "example_and.txt";
            break;
        case 2:
            filename = "example_nand.txt";
            break;
        case 3:
            filename = "example_or.txt";
            break;
        case 4:
            filename = "example_nor.txt";
            break;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    vector<pair<vector<int>, int>> data = load_data(filename);
    if (data.empty()) {
        cout << "Failed to load data from " << filename << endl;
        return 1;
    }

    int input_size = data[0].first.size();
    Perceptron perceptron(0.1, 1, input_size);
    perceptron.train(data);

    cout << "Trained weights: ";
    for (double weight : perceptron.weights) {
        cout << weight << " ";
    }
    cout << endl;

    cout << "Predictions:" << endl;
    for (const auto &sample : data) {
        const vector<int> &inputs = sample.first;
        cout << "Inputs: ";
        for (int input : inputs) {
            cout << input << " ";
        }
        cout << "Output: " << perceptron.predict(inputs) << endl;
    }

    return 0;
}