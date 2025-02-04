#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

float randomFloat() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

int main() {
    srand(static_cast<unsigned int>(time(0))); 

    float w1, w2, theta;
    w1 = randomFloat();
    w2 = randomFloat();
    theta = 0.5; 

    cout << "The weights are " << w1 << " and " << w2 << "\n";
    cout << "Theta value is " << theta << "\n\n";

    float input1, input2;
    cout << "Enter the 2 inputs: ";
    cin >> input1 >> input2;
    cout << "\n\n";

    float res = w1 * input1 + w2 * input2;
    int ans = (res >= theta) ? 1 : 0;
    
    cout << "The output is: " << ans << "\n";

    return 0;
}