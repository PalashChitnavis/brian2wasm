

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
    if (name == "neurongroup._spikespace") {
        var_size = 25;
        data_size = 25*sizeof(int32_t);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<int32_t>(name, _array_neurongroup__spikespace, var_size, (int32_t)atoi(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup__spikespace, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.lastspike") {
        var_size = 24;
        data_size = 24*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_lastspike, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_lastspike, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.not_refractory") {
        var_size = 24;
        data_size = 24*sizeof(char);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<char>(name, _array_neurongroup_not_refractory, var_size, (char)atoi(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_not_refractory, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.x") {
        var_size = 24;
        data_size = 24*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_x, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_x, data_size, s_value);
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
int32_t * _array_neurongroup__spikespace;
const int _num__array_neurongroup__spikespace = 25;
int32_t * _array_neurongroup_i;
const int _num__array_neurongroup_i = 24;
double * _array_neurongroup_lastspike;
const int _num__array_neurongroup_lastspike = 24;
char * _array_neurongroup_not_refractory;
const int _num__array_neurongroup_not_refractory = 24;
double * _array_neurongroup_x;
const int _num__array_neurongroup_x = 24;
int32_t * _array_spikemonitor__source_idx;
const int _num__array_spikemonitor__source_idx = 24;
int32_t * _array_spikemonitor_count;
const int _num__array_spikemonitor_count = 24;
int32_t * _array_spikemonitor_N;
const int _num__array_spikemonitor_N = 1;

//////////////// dynamic arrays 1d /////////
std::vector<int32_t> _dynamic_array_spikemonitor_i;
std::vector<double> _dynamic_array_spikemonitor_t;

//////////////// dynamic arrays 2d /////////

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

	_array_neurongroup__spikespace = new int32_t[25];
    
	for(int i=0; i<25; i++) _array_neurongroup__spikespace[i] = 0;

	_array_neurongroup_i = new int32_t[24];
    
	for(int i=0; i<24; i++) _array_neurongroup_i[i] = 0;

	_array_neurongroup_lastspike = new double[24];
    
	for(int i=0; i<24; i++) _array_neurongroup_lastspike[i] = 0;

	_array_neurongroup_not_refractory = new char[24];
    
	for(int i=0; i<24; i++) _array_neurongroup_not_refractory[i] = 0;

	_array_neurongroup_x = new double[24];
    
	for(int i=0; i<24; i++) _array_neurongroup_x[i] = 0;

	_array_spikemonitor__source_idx = new int32_t[24];
    
	for(int i=0; i<24; i++) _array_spikemonitor__source_idx[i] = 0;

	_array_spikemonitor_count = new int32_t[24];
    
	for(int i=0; i<24; i++) _array_spikemonitor_count[i] = 0;

	_array_spikemonitor_N = new int32_t[1];
    
	for(int i=0; i<1; i++) _array_spikemonitor_N[i] = 0;


	// Arrays initialized to an "arange"
	_array_neurongroup_i = new int32_t[24];
    
	for(int i=0; i<24; i++) _array_neurongroup_i[i] = 0 + i;

	_array_spikemonitor__source_idx = new int32_t[24];
    
	for(int i=0; i<24; i++) _array_spikemonitor__source_idx[i] = 0 + i;


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
	ofstream outfile__array_neurongroup__spikespace;
	outfile__array_neurongroup__spikespace.open(results_dir + "_array_neurongroup__spikespace_3522821529", ios::binary | ios::out);
	if(outfile__array_neurongroup__spikespace.is_open())
	{
		outfile__array_neurongroup__spikespace.write(reinterpret_cast<char*>(_array_neurongroup__spikespace), 25*sizeof(_array_neurongroup__spikespace[0]));
		outfile__array_neurongroup__spikespace.close();
		EM_ASM({
			add_results('neurongroup', '_spikespace', 'int32_t', UTF8ToString($0) + '_array_neurongroup__spikespace_3522821529', 25);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup__spikespace." << endl;
	}
	ofstream outfile__array_neurongroup_i;
	outfile__array_neurongroup_i.open(results_dir + "_array_neurongroup_i_2649026944", ios::binary | ios::out);
	if(outfile__array_neurongroup_i.is_open())
	{
		outfile__array_neurongroup_i.write(reinterpret_cast<char*>(_array_neurongroup_i), 24*sizeof(_array_neurongroup_i[0]));
		outfile__array_neurongroup_i.close();
		EM_ASM({
			add_results('neurongroup', 'i', 'int32_t', UTF8ToString($0) + '_array_neurongroup_i_2649026944', 24);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_i." << endl;
	}
	ofstream outfile__array_neurongroup_lastspike;
	outfile__array_neurongroup_lastspike.open(results_dir + "_array_neurongroup_lastspike_1647074423", ios::binary | ios::out);
	if(outfile__array_neurongroup_lastspike.is_open())
	{
		outfile__array_neurongroup_lastspike.write(reinterpret_cast<char*>(_array_neurongroup_lastspike), 24*sizeof(_array_neurongroup_lastspike[0]));
		outfile__array_neurongroup_lastspike.close();
		EM_ASM({
			add_results('neurongroup', 'lastspike', 'double', UTF8ToString($0) + '_array_neurongroup_lastspike_1647074423', 24);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_lastspike." << endl;
	}
	ofstream outfile__array_neurongroup_not_refractory;
	outfile__array_neurongroup_not_refractory.open(results_dir + "_array_neurongroup_not_refractory_1422681464", ios::binary | ios::out);
	if(outfile__array_neurongroup_not_refractory.is_open())
	{
		outfile__array_neurongroup_not_refractory.write(reinterpret_cast<char*>(_array_neurongroup_not_refractory), 24*sizeof(_array_neurongroup_not_refractory[0]));
		outfile__array_neurongroup_not_refractory.close();
		EM_ASM({
			add_results('neurongroup', 'not_refractory', 'char', UTF8ToString($0) + '_array_neurongroup_not_refractory_1422681464', 24);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_not_refractory." << endl;
	}
	ofstream outfile__array_neurongroup_x;
	outfile__array_neurongroup_x.open(results_dir + "_array_neurongroup_x_4149530994", ios::binary | ios::out);
	if(outfile__array_neurongroup_x.is_open())
	{
		outfile__array_neurongroup_x.write(reinterpret_cast<char*>(_array_neurongroup_x), 24*sizeof(_array_neurongroup_x[0]));
		outfile__array_neurongroup_x.close();
		EM_ASM({
			add_results('neurongroup', 'x', 'double', UTF8ToString($0) + '_array_neurongroup_x_4149530994', 24);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_neurongroup_x." << endl;
	}
	ofstream outfile__array_spikemonitor__source_idx;
	outfile__array_spikemonitor__source_idx.open(results_dir + "_array_spikemonitor__source_idx_1477951789", ios::binary | ios::out);
	if(outfile__array_spikemonitor__source_idx.is_open())
	{
		outfile__array_spikemonitor__source_idx.write(reinterpret_cast<char*>(_array_spikemonitor__source_idx), 24*sizeof(_array_spikemonitor__source_idx[0]));
		outfile__array_spikemonitor__source_idx.close();
		EM_ASM({
			add_results('spikemonitor', '_source_idx', 'int32_t', UTF8ToString($0) + '_array_spikemonitor__source_idx_1477951789', 24);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_spikemonitor__source_idx." << endl;
	}
	ofstream outfile__array_spikemonitor_count;
	outfile__array_spikemonitor_count.open(results_dir + "_array_spikemonitor_count_598337445", ios::binary | ios::out);
	if(outfile__array_spikemonitor_count.is_open())
	{
		outfile__array_spikemonitor_count.write(reinterpret_cast<char*>(_array_spikemonitor_count), 24*sizeof(_array_spikemonitor_count[0]));
		outfile__array_spikemonitor_count.close();
		EM_ASM({
			add_results('spikemonitor', 'count', 'int32_t', UTF8ToString($0) + '_array_spikemonitor_count_598337445', 24);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_spikemonitor_count." << endl;
	}
	ofstream outfile__array_spikemonitor_N;
	outfile__array_spikemonitor_N.open(results_dir + "_array_spikemonitor_N_225734567", ios::binary | ios::out);
	if(outfile__array_spikemonitor_N.is_open())
	{
		outfile__array_spikemonitor_N.write(reinterpret_cast<char*>(_array_spikemonitor_N), 1*sizeof(_array_spikemonitor_N[0]));
		outfile__array_spikemonitor_N.close();
		EM_ASM({
			add_results('spikemonitor', 'N', 'int32_t', UTF8ToString($0) + '_array_spikemonitor_N_225734567', 1);
		}, results_dir.c_str());

	} else
	{
		std::cout << "Error writing output file for _array_spikemonitor_N." << endl;
	}

	ofstream outfile__dynamic_array_spikemonitor_i;
	outfile__dynamic_array_spikemonitor_i.open(results_dir + "_dynamic_array_spikemonitor_i_1976709050", ios::binary | ios::out);
	if(outfile__dynamic_array_spikemonitor_i.is_open())
	{
			outfile__dynamic_array_spikemonitor_i.write(reinterpret_cast<char*>(&_dynamic_array_spikemonitor_i[0]), _dynamic_array_spikemonitor_i.size()*sizeof(_dynamic_array_spikemonitor_i[0]));
		    outfile__dynamic_array_spikemonitor_i.close();
			EM_ASM({
				add_results('spikemonitor', 'i', 'int32_t', UTF8ToString($0) + '_dynamic_array_spikemonitor_i_1976709050', $1);
			}, results_dir.c_str(), _dynamic_array_spikemonitor_i.size());
	} else
	{
		std::cout << "Error writing output file for _dynamic_array_spikemonitor_i." << endl;
	}
	ofstream outfile__dynamic_array_spikemonitor_t;
	outfile__dynamic_array_spikemonitor_t.open(results_dir + "_dynamic_array_spikemonitor_t_383009635", ios::binary | ios::out);
	if(outfile__dynamic_array_spikemonitor_t.is_open())
	{
			outfile__dynamic_array_spikemonitor_t.write(reinterpret_cast<char*>(&_dynamic_array_spikemonitor_t[0]), _dynamic_array_spikemonitor_t.size()*sizeof(_dynamic_array_spikemonitor_t[0]));
		    outfile__dynamic_array_spikemonitor_t.close();
			EM_ASM({
				add_results('spikemonitor', 't', 'double', UTF8ToString($0) + '_dynamic_array_spikemonitor_t_383009635', $1);
			}, results_dir.c_str(), _dynamic_array_spikemonitor_t.size());
	} else
	{
		std::cout << "Error writing output file for _dynamic_array_spikemonitor_t." << endl;
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

