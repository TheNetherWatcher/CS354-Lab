#include <bits/stdc++.h>
using namespace std;

#define NUM_EPOCHS 35
#define LEARNING_RATE 0.0001
#define NUM_HIDDEN_NEURONS 15

// Activation function
inline double activate(double value) {
    return 1.0 / (1.0 + exp(-value));
}

// Derivative of activation function
inline double activation_derivative(double value) {
    return value * (1 - value);
}

class MLPClassifier {
public:
    vector<vector<double>> input_hidden_weights;
    vector<double> hidden_output_weights;
    int num_inputs, num_hidden, num_outputs;

    MLPClassifier(int num_inputs, int num_hidden, int num_outputs) {
        this->num_inputs = num_inputs;
        this->num_hidden = num_hidden;
        this->num_outputs = num_outputs;

        input_hidden_weights.resize(num_inputs, vector<double>(num_hidden));
        hidden_output_weights.resize(num_hidden);

        random_device rd;
        mt19937 generator(rd());
        uniform_real_distribution<double> weight_dist(-1, 1);

        for (auto &layer : input_hidden_weights)
            for (auto &weight : layer)
                weight = weight_dist(generator);

        for (auto &weight : hidden_output_weights)
            weight = weight_dist(generator);
    }

    void train(vector<vector<double>> &train_data, vector<int> &train_labels) {
        for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
            double total_loss = 0.0;
            for (size_t sample = 0; sample < train_data.size(); ++sample) {
                vector<double> hidden_layer(num_hidden, 0.0);
                double output_layer = 0.0;

                for (int j = 0; j < num_hidden; ++j) {
                    for (int k = 0; k < num_inputs; ++k)
                        hidden_layer[j] += train_data[sample][k] * input_hidden_weights[k][j];
                    hidden_layer[j] = activate(hidden_layer[j]);
                }

                for (int j = 0; j < num_hidden; ++j)
                    output_layer += hidden_layer[j] * hidden_output_weights[j];
                output_layer = activate(output_layer);

                double error = train_labels[sample] - output_layer;
                total_loss += 0.5 * error * error;

                double output_delta = error * activation_derivative(output_layer);
                vector<double> hidden_deltas(num_hidden);

                for (int j = 0; j < num_hidden; ++j)
                    hidden_deltas[j] = output_delta * hidden_output_weights[j] * activation_derivative(hidden_layer[j]);

                for (int j = 0; j < num_hidden; ++j)
                    hidden_output_weights[j] += LEARNING_RATE * output_delta * hidden_layer[j];

                for (int j = 0; j < num_hidden; ++j)
                    for (int k = 0; k < num_inputs; ++k)
                        input_hidden_weights[k][j] += LEARNING_RATE * hidden_deltas[j] * train_data[sample][k];
            }
            cout << "Epoch " << epoch + 1 << ": Loss = " << total_loss / train_data.size() << endl;
        }
    }

    int classify(vector<double> &sample) {
        vector<double> hidden_layer(num_hidden, 0.0);
        double output_layer = 0.0;

        for (int j = 0; j < num_hidden; ++j) {
            for (int k = 0; k < num_inputs; ++k)
                hidden_layer[j] += sample[k] * input_hidden_weights[k][j];
            hidden_layer[j] = activate(hidden_layer[j]);
        }

        for (int j = 0; j < num_hidden; ++j)
            output_layer += hidden_layer[j] * hidden_output_weights[j];
        output_layer = activate(output_layer);

        return output_layer > 0.5 ? 1 : 0;
    }
};

vector<vector<double>> read_dataset(const string &file_path, vector<int> &labels) {
    ifstream file(file_path);
    vector<vector<double>> dataset;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<double> features(4);
        for (int i = 0; i < 4; ++i)
            ss >> features[i], ss.ignore();
        
        string label;
        ss >> label;
        labels.push_back(label == "setosa" ? 0 : 1);
        dataset.push_back(features);
    }

    for (int i = 0; i < 4; ++i) {
        double max_value = 0.0;
        for (auto &sample : dataset)
            max_value = max(max_value, sample[i]);
        for (auto &sample : dataset)
            sample[i] /= max_value;
    }
    return dataset;
}

int main() {
    vector<int> train_labels, test_labels;
    vector<vector<double>> train_data = read_dataset("iris_train.csv", train_labels);
    vector<vector<double>> test_data = read_dataset("iris_test.csv", test_labels);

    MLPClassifier model(4, NUM_HIDDEN_NEURONS, 1);
    model.train(train_data, train_labels);

    int correct_predictions = 0;
    for (size_t i = 0; i < test_data.size(); ++i)
        if (model.classify(test_data[i]) == test_labels[i])
            ++correct_predictions;

    cout << "Test Accuracy: " << (double)correct_predictions / test_data.size() * 100 << "%" << endl;
    return 0;
}