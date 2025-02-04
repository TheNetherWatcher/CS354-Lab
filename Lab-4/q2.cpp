// #include <iostream>
// #include <vector>

// using namespace std;

// struct Perceptron {
//     vector<double> weights;
//     double eta;  
//     int theta;   
//     int bias_ip; 

//     Perceptron(double eta, int theta) {
//         this->eta = eta;
//         this->theta = theta;
//         this->bias_ip = 1;
//         weights = {0, 0, 0}; 
//     }

//     void train(const vector<pair<pair<int, int>, int>> &data) {
//         bool flag = true;
//         int epoch = 0;

//         while (flag) {
//             flag = false;
//             for (const auto &sample : data) {
//                 int x1 = sample.first.first, x2 = sample.first.second, target = sample.second;
//                 double net = weights[0] * bias_ip + weights[1] * x1 + weights[2] * x2;
//                 int output = (net >= theta) ? 1 : 0;

//                 int error = target - output;
//                 if (error != 0) {
//                     flag = true;
//                     weights[0] += eta * error * bias_ip;
//                     weights[1] += eta * error * x1;
//                     weights[2] += eta * error * x2;
//                 }
//             }
//             epoch++;
//         }
//     }

//     int predict(int x1, int x2) {
//         double net = weights[0] * bias_ip + weights[1] * x1 + weights[2] * x2;
//         return (net >= theta) ? 1 : 0;
//     }
// };

// int main() {
//     int choice;
//     cout << "Select the gate to train the perceptron model:\n";
//     cout << "1. AND\n";
//     cout << "2. NAND\n";
//     cout << "3. OR\n";
//     cout << "4. NOR\n";
//     cout << "Enter your choice: ";
//     cin >> choice;

//     vector<pair<pair<int, int>, int>> data;
//     switch (choice) {
//         case 1:
//             data = {{{0, 0}, 0}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 1}};
//             break;
//         case 2:
//             data = {{{0, 0}, 1}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 0}};
//             break;
//         case 3:
//             data = {{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 1}};
//             break;
//         case 4:
//             data = {{{0, 0}, 1}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 0}};
//             break;
//         default:
//             cout << "Invalid choice!" << endl;
//             return 1;
//     }

//     Perceptron perceptron(0.1, 1);
//     perceptron.train(data);

//     cout << "Trained weights: ";
//     for (double weight : perceptron.weights) {
//         cout << weight << " ";
//     }
//     cout << endl;

//     cout << "Predictions:" << endl;
//     cout << "0 and 0: " << perceptron.predict(0, 0) << endl;
//     cout << "0 and 1: " << perceptron.predict(0, 1) << endl;
//     cout << "1 and 0: " << perceptron.predict(1, 0) << endl;
//     cout << "1 and 1: " << perceptron.predict(1, 1) << endl;

//     return 0;
// }

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