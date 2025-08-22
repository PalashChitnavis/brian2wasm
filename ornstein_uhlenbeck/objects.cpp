

#include "objects.h"
#include "synapses_classes.h"
#include "brianlib/clocks.h"
#include "brianlib/dynamic_array.h"
#include "brianlib/stdint_compat.h"
#include "network.h"
#include<random>
#include<vector>
#include<iostream>
#include<fstream>
#include <emscripten.h>

namespace brian {

std::string results_dir = "results/";  // can be overwritten by --results_dir command line arg
// For multhreading, we need one generator for each thread. We also create a distribution for
// each thread, even though this is not strictly necessary for the uniform distribution, as
// the distribution is stateless.
std::vector< RandomGenerator > _random_generators;

//////////////// networks /////////////////
Network magicnetwork;


template<class T> void set_variable_from_value(std::string varname, T* var_pointer, size_t size, T value) {
    #ifdef DEBUG
    std::cout << "Setting '" << varname << "' to " << value << std::endl;
    #endif
    std::fill(var_pointer, var_pointer+size, value);
}

template<class T> void set_variable_from_file(std::string varname, T* var_pointer, size_t data_size, std::string filename) {
    ifstream f;
    streampos size;
    #ifdef DEBUG
    std::cout << "Setting '" << varname << "' from file '" << filename << "'" << std::endl;
    #endif
    f.open(filename, ios::in | ios::binary | ios::ate);
    size = f.tellg();
    if (size != data_size) {
        std::cerr << "Error reading '" << filename << "': file size " << size << " does not match expected size " << data_size << std::endl;
        return;
    }
    f.seekg(0, ios::beg);
    if (f.is_open())
        f.read(reinterpret_cast<char *>(var_pointer), data_size);
    else
        std::cerr << "Could not read '" << filename << "'" << std::endl;
    if (f.fail())
        std::cerr << "Error reading '" << filename << "'" << std::endl;
}

//////////////// set arrays by name ///////
void set_variable_by_name(std::string name, std::string s_value) {
	size_t var_size;
	size_t data_size;
	std::for_each(s_value.begin(), s_value.end(), [](char& c) // modify in-place
    {
        c = std::tolower(static_cast<unsigned char>(c));
    });
    if (s_value == "true")
        s_value = "1";
    else if (s_value == "false")
        s_value = "0";
	// non-dynamic arrays
    if (name == "neurongroup.sigma") {
        var_size = 1;
        data_size = 1*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_sigma, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_sigma, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.tau") {
        var_size = 1;
        data_size = 1*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_tau, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_tau, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.y") {
        var_size = 1;
        data_size = 1*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_y, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_y, data_size, s_value);
        }
        return;
    }
    // dynamic arrays (1d)
    std::cerr << "Cannot set unknown variable '" << name << "'." << std::endl;
    exit(1);
}

//////////////// arrays ///////////////////
double * _array_defaultclock_dt;
const int _num__array_defaultclock_dt = 1;
double * _array_defaultclock_t;
const int _num__array_defaultclock_t = 1;
int64_t * _array_defaultclock_timestep;
const int _num__array_defaultclock_timestep = 1;
int32_t * _array_neurongroup_i;
const int _num__array_neurongroup_i = 1;
double * _array_neurongroup_sigma;
const int _num__array_neurongroup_sigma = 1;
double * _array_neurongroup_tau;
const int _num__array_neurongroup_tau = 1;
double * _array_neurongroup_y;
const int _num__array_neurongroup_y = 1;
int32_t * _array_statemonitor__indices;
const int _num__array_statemonitor__indices = 1;
int32_t * _array_statemonitor_N;
const int _num__array_statemonitor_N = 1;
double * _array_statemonitor_y;
const int _num__array_statemonitor_y = (0, 1);

//////////////// dynamic arrays 1d /////////
std::vector<double> _dynamic_array_statemonitor_t;

//////////////// dynamic arrays 2d /////////
DynamicArray2D<double> _dynamic_array_statemonitor_y;

/////////////// static arrays /////////////

//////////////// synapses /////////////////

//////////////// clocks ///////////////////
Clock defaultclock;  // attributes will be set in run.cpp

// Profiling information for each code object
}

void _init_arrays()
{
	using namespace brian;

    // Arrays initialized to 0
	_array_defaultclock_dt = new double[1];
    
	for(int i=0; i<1; i++) _array_defaultclock_dt[i] = 0;

	_array_defaultclock_t = new double[1];
    
	for(int i=0; i<1; i++) _array_defaultclock_t[i] = 0;

	_array_defaultclock_timestep = new int64_t[1];
    
	for(int i=0; i<1; i++) _array_defaultclock_timestep[i] = 0;

	_array_neurongroup_i = new int32_t[1];
    
	for(int i=0; i<1; i++) _array_neurongroup_i[i] = 0;

	_array_neurongroup_sigma = new double[1];
    
	for(int i=0; i<1; i++) _array_neurongroup_sigma[i] = 0;

	_array_neurongroup_tau = new double[1];
    
	for(int i=0; i<1; i++) _array_neurongroup_tau[i] = 0;

	_array_neurongroup_y = new double[1];
    
	for(int i=0; i<1; i++) _array_neurongroup_y[i] = 0;

	_array_statemonitor__indices = new int32_t[1];
    
	for(int i=0; i<1; i++) _array_statemonitor__indices[i] = 0;

	_array_statemonitor_N = new int32_t[1];
    
	for(int i=0; i<1; i++) _array_statemonitor_N[i] = 0;


	// Arrays initialized to an "arange"
	_array_neurongroup_i = new int32_t[1];
    
	for(int i=0; i<1; i++) _array_neurongroup_i[i] = 0 + i;


	// static arrays

    // Random number generator states
    std::random_device rd;
    for (int i=0; i<1; i++)
        _random_generators.push_back(RandomGenerator());
}

void _load_arrays()
{
	using namespace brian;

}

void _write_arrays()
{
	using namespace brian;

    EM_ASM({
        // Make the results directory if it doesn't exist
		if (!FS.analyzePath(UTF8ToString($0)).exists) {
        	FS.mkdir(UTF8ToString($0));
		}
    }, results_dir.c_str());

	ofstream outfile__array_defaultclock_dt;
	outfile__array_defaultclock_dt.open(results_dir + "_array_defaultclock_dt_1978099143", ios::binary | ios::out);
	if(outfile__array_defaultclock_dt.is_open())
	{
		outfile__array_defaultclock_dt.write(reinterpret_cast<char*>(_array_defaultclock_dt), 1*sizeof(_array_defaultclock_dt[0]));
		outfile__array_defaultclock_dt.close();
		EM_ASM({
			add_results('defaultclock', 'dt', 'double', UTF8ToString($0) + '_array_defaultclock_dt_1978099143', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_defaultclock_dt." << endl;
	}
	ofstream outfile__array_defaultclock_t;
	outfile__array_defaultclock_t.open(results_dir + "_array_defaultclock_t_2669362164", ios::binary | ios::out);
	if(outfile__array_defaultclock_t.is_open())
	{
		outfile__array_defaultclock_t.write(reinterpret_cast<char*>(_array_defaultclock_t), 1*sizeof(_array_defaultclock_t[0]));
		outfile__array_defaultclock_t.close();
		EM_ASM({
			add_results('defaultclock', 't', 'double', UTF8ToString($0) + '_array_defaultclock_t_2669362164', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_defaultclock_t." << endl;
	}
	ofstream outfile__array_defaultclock_timestep;
	outfile__array_defaultclock_timestep.open(results_dir + "_array_defaultclock_timestep_144223508", ios::binary | ios::out);
	if(outfile__array_defaultclock_timestep.is_open())
	{
		outfile__array_defaultclock_timestep.write(reinterpret_cast<char*>(_array_defaultclock_timestep), 1*sizeof(_array_defaultclock_timestep[0]));
		outfile__array_defaultclock_timestep.close();
		EM_ASM({
			add_results('defaultclock', 'timestep', 'int64_t', UTF8ToString($0) + '_array_defaultclock_timestep_144223508', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_defaultclock_timestep." << endl;
	}
	ofstream outfile__array_neurongroup_i;
	outfile__array_neurongroup_i.open(results_dir + "_array_neurongroup_i_2649026944", ios::binary | ios::out);
	if(outfile__array_neurongroup_i.is_open())
	{
		outfile__array_neurongroup_i.write(reinterpret_cast<char*>(_array_neurongroup_i), 1*sizeof(_array_neurongroup_i[0]));
		outfile__array_neurongroup_i.close();
		EM_ASM({
			add_results('neurongroup', 'i', 'int32_t', UTF8ToString($0) + '_array_neurongroup_i_2649026944', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_i." << endl;
	}
	ofstream outfile__array_neurongroup_sigma;
	outfile__array_neurongroup_sigma.open(results_dir + "_array_neurongroup_sigma_129435642", ios::binary | ios::out);
	if(outfile__array_neurongroup_sigma.is_open())
	{
		outfile__array_neurongroup_sigma.write(reinterpret_cast<char*>(_array_neurongroup_sigma), 1*sizeof(_array_neurongroup_sigma[0]));
		outfile__array_neurongroup_sigma.close();
		EM_ASM({
			add_results('neurongroup', 'sigma', 'double', UTF8ToString($0) + '_array_neurongroup_sigma_129435642', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_sigma." << endl;
	}
	ofstream outfile__array_neurongroup_tau;
	outfile__array_neurongroup_tau.open(results_dir + "_array_neurongroup_tau_4170660524", ios::binary | ios::out);
	if(outfile__array_neurongroup_tau.is_open())
	{
		outfile__array_neurongroup_tau.write(reinterpret_cast<char*>(_array_neurongroup_tau), 1*sizeof(_array_neurongroup_tau[0]));
		outfile__array_neurongroup_tau.close();
		EM_ASM({
			add_results('neurongroup', 'tau', 'double', UTF8ToString($0) + '_array_neurongroup_tau_4170660524', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_tau." << endl;
	}
	ofstream outfile__array_neurongroup_y;
	outfile__array_neurongroup_y.open(results_dir + "_array_neurongroup_y_2152980964", ios::binary | ios::out);
	if(outfile__array_neurongroup_y.is_open())
	{
		outfile__array_neurongroup_y.write(reinterpret_cast<char*>(_array_neurongroup_y), 1*sizeof(_array_neurongroup_y[0]));
		outfile__array_neurongroup_y.close();
		EM_ASM({
			add_results('neurongroup', 'y', 'double', UTF8ToString($0) + '_array_neurongroup_y_2152980964', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_y." << endl;
	}
	ofstream outfile__array_statemonitor__indices;
	outfile__array_statemonitor__indices.open(results_dir + "_array_statemonitor__indices_2854283999", ios::binary | ios::out);
	if(outfile__array_statemonitor__indices.is_open())
	{
		outfile__array_statemonitor__indices.write(reinterpret_cast<char*>(_array_statemonitor__indices), 1*sizeof(_array_statemonitor__indices[0]));
		outfile__array_statemonitor__indices.close();
		EM_ASM({
			add_results('statemonitor', '_indices', 'int32_t', UTF8ToString($0) + '_array_statemonitor__indices_2854283999', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_statemonitor__indices." << endl;
	}
	ofstream outfile__array_statemonitor_N;
	outfile__array_statemonitor_N.open(results_dir + "_array_statemonitor_N_4140778434", ios::binary | ios::out);
	if(outfile__array_statemonitor_N.is_open())
	{
		outfile__array_statemonitor_N.write(reinterpret_cast<char*>(_array_statemonitor_N), 1*sizeof(_array_statemonitor_N[0]));
		outfile__array_statemonitor_N.close();
		EM_ASM({
			add_results('statemonitor', 'N', 'int32_t', UTF8ToString($0) + '_array_statemonitor_N_4140778434', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_statemonitor_N." << endl;
	}

	ofstream outfile__dynamic_array_statemonitor_t;
	outfile__dynamic_array_statemonitor_t.open(results_dir + "_dynamic_array_statemonitor_t_3983503110", ios::binary | ios::out);
	if(outfile__dynamic_array_statemonitor_t.is_open())
	{
			outfile__dynamic_array_statemonitor_t.write(reinterpret_cast<char*>(&_dynamic_array_statemonitor_t[0]), _dynamic_array_statemonitor_t.size()*sizeof(_dynamic_array_statemonitor_t[0]));
		    outfile__dynamic_array_statemonitor_t.close();
			EM_ASM({
				add_results('statemonitor', 't', 'double', UTF8ToString($0) + '_dynamic_array_statemonitor_t_3983503110', $1);
			}, results_dir.c_str(), _dynamic_array_statemonitor_t.size());
	} else
	{
		std::cout << "Error writing output file for _dynamic_array_statemonitor_t." << endl;
	}

	ofstream outfile__dynamic_array_statemonitor_y;
	outfile__dynamic_array_statemonitor_y.open(results_dir + "_dynamic_array_statemonitor_y_2480804795", ios::binary | ios::out);
	if(outfile__dynamic_array_statemonitor_y.is_open())
	{
        for (int n=0; n<_dynamic_array_statemonitor_y.n; n++)
        {
            if (! _dynamic_array_statemonitor_y(n).empty())
            {
                outfile__dynamic_array_statemonitor_y.write(reinterpret_cast<char*>(&_dynamic_array_statemonitor_y(n, 0)), _dynamic_array_statemonitor_y.m*sizeof(_dynamic_array_statemonitor_y(0, 0)));
            }
        }
        outfile__dynamic_array_statemonitor_y.close();

		EM_ASM({
				add_results('statemonitor', 'y', 'double', UTF8ToString($0) + '_dynamic_array_statemonitor_y_2480804795', $1, $2);
		}, results_dir.c_str(), _dynamic_array_statemonitor_y.n, _dynamic_array_statemonitor_y.m);
	} else
	{
		std::cout << "Error writing output file for _dynamic_array_statemonitor_y." << endl;
	}
	// Write last run info to disk
	ofstream outfile_last_run_info;
	outfile_last_run_info.open(results_dir + "last_run_info.txt", ios::out);
	if(outfile_last_run_info.is_open())
	{
		outfile_last_run_info << (Network::_last_run_time) << " " << (Network::_last_run_completed_fraction) << std::endl;
		outfile_last_run_info.close();
	} else
	{
	    std::cout << "Error writing last run info to file." << std::endl;
	}
}

void _dealloc_arrays()
{
	using namespace brian;


	// static arrays
}

