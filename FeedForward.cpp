#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h> 
#include <ctime>


using namespace std;
vector<vector<float>> weights;


vector<float> initialize_input(int ip_size)
{
    vector<float> rand_ip;
    srand((unsigned int)time(NULL));
    float a = 3;
    cout<<"\n";
    for (int i=0; i<ip_size; i++)
    {
        rand_ip.push_back(((float)rand()/(float)(RAND_MAX)) * a); //will generate a value between 0.0 and a
    }
    cout<<"\n";

    return rand_ip;
}

void initialize_weights(int input_size, int output_size)
{
    //generate random weights
    for(int out = 0; out < output_size; out++)
    {
        weights.push_back(vector<float>());
        for(int input = 0; input < input_size + 1; input++)
        { //we create an extra weight (one more than input_size) for our bias
            weights.back().push_back((float)rand() / RAND_MAX); //random value between 0 and 1
        }
    }
}


class LinearLayer{
private:
    
    vector<float> output_vals;
    vector<float> input_vals;
    int output_dim;
    int input_dim;
    
public:
    LinearLayer(){}
    LinearLayer(int input_size, int output_size);
    vector<float> feedForward(const vector<float> &input);
};
LinearLayer::LinearLayer(int input_size, int output_size)
{
    assert(input_size > 0);
    assert(output_size > 0);
    output_dim = output_size;
    input_dim = input_size;
   


}
vector<float> LinearLayer::feedForward(const vector<float> &input){
    assert(input.size() == input_dim);
    output_vals = vector<float>();
    input_vals = input; //store the input vector

    //perform matrix multiplication
    for(int out = 0; out < output_dim; out++){
        float sum = 0.0;
        for(int w = 0; w < input_dim; w++){
            sum += weights[out][w] * input[w];
        }
        sum += weights[out][input_dim]; //account for the bias
        output_vals.push_back(sum);
    }
    return output_vals;
}

vector<int> activation(vector<float> feedfw_vec)
{
    vector<int> act_vec;
    for (float i: feedfw_vec)
    {
        act_vec.push_back(tanh(i));
    }
    return act_vec;
}

int main ()
{
    initialize_weights(16,4);
    vector<float> fw_input = initialize_input(16);
    LinearLayer model(16,4);
    vector<float> feedFw_vector = model.feedForward(fw_input);
    vector<int> activation_output = activation(feedFw_vector);
    std::cout <<"Feedforward Output [ ";
    for (float i: feedFw_vector)
    std::cout << i << ' ';
    std::cout <<"] \n";

    std::cout <<"Activation Output [ ";
    for (int k: activation_output)
    std::cout << k << ' ';
    std::cout <<"] \n";
}