// Question 2: Iris Classification
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    double sig = sigmoid(x);
    return sig * (1.0 - sig);
}

class NeuralNetwork {
private:
    vector<int> layers;
    vector<vector<vector<double>>> weights;
    vector<vector<double>> biases;
    double learning_rate;
    random_device rd;
    mt19937 gen;
    uniform_real_distribution<> dis;

public:
    NeuralNetwork(const vector<int>& layer_sizes, double lr = 0.0001) 
        : layers(layer_sizes), learning_rate(lr), gen(rd()), dis(-1.0, 1.0) {
        
        for (size_t i = 0; i < layers.size() - 1; ++i) {
            vector<vector<double>> layer_weights;
            for (int j = 0; j < layers[i + 1]; ++j) {
                vector<double> neuron_weights;
                for (int k = 0; k < layers[i]; ++k) {
                    neuron_weights.push_back(dis(gen));
                }
                layer_weights.push_back(neuron_weights);
            }
            weights.push_back(layer_weights);
            
            vector<double> layer_biases;
            for (int j = 0; j < layers[i + 1]; ++j) {
                layer_biases.push_back(dis(gen));
            }
            biases.push_back(layer_biases);
        }
    }

    vector<double> forward(const vector<double>& input) {
        vector<vector<double>> activations = {input};
        
        for (size_t i = 0; i < weights.size(); ++i) {
            vector<double> layer_output;
            for (size_t j = 0; j < weights[i].size(); ++j) {
                double sum = biases[i][j];
                for (size_t k = 0; k < weights[i][j].size(); ++k) {
                    sum += weights[i][j][k] * activations.back()[k];
                }
                layer_output.push_back(sigmoid(sum));
            }
            activations.push_back(layer_output);
        }
        
        return activations.back();
    }

    void backpropagate(const vector<double>& input, const vector<double>& target) {
        vector<vector<double>> activations = {input};
        vector<vector<double>> weighted_sums;
        
        for (size_t i = 0; i < weights.size(); ++i) {
            vector<double> layer_output;
            vector<double> layer_sums;
            for (size_t j = 0; j < weights[i].size(); ++j) {
                double sum = biases[i][j];
                for (size_t k = 0; k < weights[i][j].size(); ++k) {
                    sum += weights[i][j][k] * activations.back()[k];
                }
                layer_sums.push_back(sum);
                layer_output.push_back(sigmoid(sum));
            }
            weighted_sums.push_back(layer_sums);
            activations.push_back(layer_output);
        }

        vector<vector<double>> deltas;
        vector<double> output_delta;
        for (size_t i = 0; i < activations.back().size(); ++i) {
            double error = activations.back()[i] - target[i];
            output_delta.push_back(error * sigmoid_derivative(weighted_sums.back()[i]));
        }
        deltas.push_back(output_delta);
        
        for (int i = weights.size() - 2; i >= 0; --i) {
            vector<double> layer_delta;
            for (size_t j = 0; j < weights[i].size(); ++j) {
                double error = 0.0;
                for (size_t k = 0; k < weights[i + 1].size(); ++k) {
                    error += weights[i + 1][k][j] * deltas.front()[k];
                }
                layer_delta.push_back(error * sigmoid_derivative(weighted_sums[i][j]));
            }
            deltas.insert(deltas.begin(), layer_delta);
        }

        for (size_t i = 0; i < weights.size(); ++i) {
            for (size_t j = 0; j < weights[i].size(); ++j) {
                for (size_t k = 0; k < weights[i][j].size(); ++k) {
                    weights[i][j][k] -= learning_rate * deltas[i][j] * activations[i][k];
                }
                biases[i][j] -= learning_rate * deltas[i][j];
            }
        }
    }
};

vector<pair<vector<double>, vector<double>>> load_iris_data(const string& filename) {
    vector<pair<vector<double>, vector<double>>> data;
    ifstream file(filename);
    string line;
    
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<double> features;
        
        for (int i = 0; i < 4; ++i) {
            getline(ss, value, ',');
            features.push_back(stod(value));
        }
        
        getline(ss, value, ',');
        vector<double> target = {value == "setosa" ? 0.0 : 1.0};
        
        data.push_back({features, target});
    }
    
    return data;
}

int main() {
    cout << "Iris Classification Neural Network\n";
    auto data = load_iris_data("iris.csv");
    
    // Neural network with 4 input features, 10 hidden nodes, 1 output node
    NeuralNetwork nn({4, 10, 1}, 0.0001);
    
    cout << "Training for 35 epochs...\n";
    for (int epoch = 0; epoch < 35; ++epoch) {
        double total_error = 0.0;
        int correct_predictions = 0;
        
        for (const auto& sample : data) {
            auto output = nn.forward(sample.first);
            nn.backpropagate(sample.first, sample.second);
            total_error += pow(output[0] - sample.second[0], 2);
            
            // Calculate accuracy using 0.5 threshold
            bool predicted = output[0] > 0.5;
            bool actual = sample.second[0] > 0.5;
            if (predicted == actual) correct_predictions++;
        }
        
        double accuracy = (double)correct_predictions / data.size() * 100;
        cout << "Epoch " << epoch + 1 
             << ", Error: " << total_error / data.size()
             << ", Accuracy: " << accuracy << "%\n";
    }
    
    return 0;
}