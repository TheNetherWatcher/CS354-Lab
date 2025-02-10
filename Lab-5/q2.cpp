#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

struct Perceptron {
    vector<double> weights;
    double eta;  // Learning rate
    int bias_ip; // Bias input

    Perceptron(double eta, int input_size) {
        this->eta = eta;
        this->bias_ip = 1;
        weights = vector<double>(input_size + 1, 0); // Including bias weight
    }

    void train(const vector<pair<vector<double>, int>> &data, int max_epochs) {
        bool flag = true;
        int epoch = 0;

        while (flag && epoch < max_epochs) {
            flag = false;
            cout << "Epoch " << epoch + 1 << ":\n";
            for (const auto &sample : data) {
                const vector<double> &inputs = sample.first;
                int target = sample.second;

                double net = weights[0] * bias_ip;
                for (size_t i = 0; i < inputs.size(); ++i) {
                    net += weights[i + 1] * inputs[i];
                }
                int output = (net >= 0) ? 1 : 0;

                int error = target - output;
                if (error != 0) {
                    flag = true;
                    weights[0] += eta * error * bias_ip;
                    for (size_t i = 0; i < inputs.size(); ++i) {
                        weights[i + 1] += eta * error * inputs[i];
                    }
                    cout << "Updated weights: ";
                    for (double weight : weights) {
                        cout << weight << " ";
                    }
                    cout << endl;
                }
            }
            epoch++;
            cout << "-----------------------------\n";
        }

        cout << "Training completed in " << epoch << " epochs." << endl;
    }

    int predict(const vector<double> &inputs) {
        double net = weights[0] * bias_ip;
        for (size_t i = 0; i < inputs.size(); ++i) {
            net += weights[i + 1] * inputs[i];
        }
        return (net >= 0) ? 1 : 0;
    }
};

int main() {
    vector<pair<vector<double>, int>> data = {
        {{1.0, 2.0}, 0},  // Rabbit
        {{1.5, 2.5}, 0},  // Rabbit
        {{2.0, 3.0}, 0},  // Rabbit
        {{4.0, 1.0}, 1},  // Bear
        {{4.5, 1.5}, 1},  // Bear
        {{5.0, 2.0}, 1}   // Bear
    };

    Perceptron perceptron(0.1, 2); // Learning rate = 0.1, 2 features (weight, ear length)
    perceptron.train(data, 100);   // Train for up to 100 epochs

    cout << "Trained weights: ";
    for (double weight : perceptron.weights) {
        cout << weight << " ";
    }
    cout << endl;

    cout << "Predictions:\n";
    for (const auto &sample : data) {
        const vector<double> &inputs = sample.first;
        int target = sample.second;
        int output = perceptron.predict(inputs);
        cout << "Inputs (Weight, Ear Length): (" << inputs[0] << ", " << inputs[1] << ") ";
        cout << "Predicted: " << (output == 0 ? "Rabbit" : "Bear") << ", Target: " << (target == 0 ? "Rabbit" : "Bear") << endl;
    }

    return 0;
}
