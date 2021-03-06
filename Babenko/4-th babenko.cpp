
#include "floatfann.h"
#include "fann_cpp.h"
#include <ios>
#include <iostream>
#include <iomanip>
using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::left;
using std::right;
using std::showpos;
using std::noshowpos;



int print_callback(FANN::neural_net &net, FANN::training_data &train,
    unsigned int max_epochs, unsigned int epochs_between_reports,
    float desired_error, unsigned int epochs, void *user_data)
{
    cout << "Epochs     " << setw(8) << epochs << ". "
         << "Current Error: " << left << net.get_MSE() << right << endl;
    return 0;
}


void xor_test()
{
    cout << endl << "XOR test started." << endl;

    const float learning_rate = 0.7f;
    const unsigned int num_layers = 3;
    const unsigned int num_input = 2;
    const unsigned int num_hidden = 3;
    const unsigned int num_output = 1;
    const float desired_error = 0.001f;
    const unsigned int max_iterations = 300000;
    const unsigned int iterations_between_reports = 1000;

    cout << endl << "Creating network." << endl;

    FANN::neural_net net;
    net.create_standard(num_layers, num_input, num_hidden, num_output);

    net.set_learning_rate(learning_rate);

    net.set_activation_steepness_hidden(1.0);
    net.set_activation_steepness_output(1.0);
    
    net.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC_STEPWISE);
    net.set_activation_function_output(FANN::SIGMOID_SYMMETRIC_STEPWISE);


    cout << endl << "Network Type                         :  ";
    switch (net.get_network_type())
    {
    case FANN::LAYER:
        cout << "LAYER" << endl;
        break;
    case FANN::SHORTCUT:
        cout << "SHORTCUT" << endl;
        break;
    default:
        cout << "UNKNOWN" << endl;
        break;
    }
    net.print_parameters();

    cout << endl << "Training network." << endl;

    FANN::training_data data;
    if (data.read_train_from_file("xor.data"))
    {
        
        net.init_weights(data);

        cout << "Max Epochs " << setw(8) << max_iterations << ". "
            << "Desired Error: " << left << desired_error << right << endl;
        net.set_callback(print_callback, NULL);
        net.train_on_data(data, max_iterations,
            iterations_between_reports, desired_error);

        cout << endl << "Testing network." << endl;

        for (unsigned int i = 0; i < data.length_train_data(); ++i)
        {
            
            fann_type *calc_out = net.run(data.get_input()[i]);

            cout << "XOR test (" << showpos << data.get_input()[i][0] << ", " 
                 << data.get_input()[i][1] << ") -> " << *calc_out
                 << ", should be " << data.get_output()[i][0] << ", "
                 << "difference = " << noshowpos
                 << fann_abs(*calc_out - data.get_output()[i][0]) << endl;
        }
        
        cout << endl << "Saving network." << endl;

        
        net.save("xor_float.net");
        unsigned int decimal_point = net.save_to_fixed("xor_fixed.net");
        data.save_train_to_fixed("xor_fixed.data", decimal_point);

        cout << endl << "XOR test completed." << endl;
    }
}


int main(int argc, char **argv)
{
    try
    {
        std::ios::sync_with_stdio(); 
        xor_test();
    }
    catch (...)
    {
        cerr << endl << "Abnormal exception." << endl;
    }
    return 0;
}
