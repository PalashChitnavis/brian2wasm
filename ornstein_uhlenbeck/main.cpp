#include <stdlib.h>
#include "objects.h"
#include <csignal>
#include <ctime>
#include <time.h>

#include "run.h"
#include "brianlib/common_math.h"

#include "code_objects/neurongroup_stateupdater_codeobject.h"
#include "code_objects/statemonitor_codeobject.h"

#include <emscripten.h>

#include <iostream>
#include <fstream>
#include <string>




void set_from_command_line(const std::vector<std::string> args)
{
    for (const auto& arg : args) {
		// Split into two parts
		size_t equal_sign = arg.find("=");
		auto name = arg.substr(0, equal_sign);
		auto value = arg.substr(equal_sign + 1, arg.length());
		brian::set_variable_by_name(name, value);
	}
}

void _int_handler(int signal_num) {
	if (Network::_globally_running && !Network::_globally_stopped) {
		Network::_globally_stopped = true;
	} else {
		std::signal(signal_num, SIG_DFL);
		std::raise(signal_num);
	}
}

int main(int argc, char **argv)
{
	std::signal(SIGINT, _int_handler);
	std::random_device _rd;
	std::vector<std::string> args(argv + 1, argv + argc);
	if (args.size() >=2 && args[0] == "--results_dir")
	{
		brian::results_dir = args[1];
		#ifdef DEBUG
		std::cout << "Setting results dir to '" << brian::results_dir << "'" << std::endl;
		#endif
		args.erase(args.begin(), args.begin()+2);
	}
        

	brian_start();
        

	{
		using namespace brian;

		
                
        _array_defaultclock_dt[0] = 0.0001;
        _array_defaultclock_dt[0] = 0.0001;
        _array_defaultclock_dt[0] = 0.0001;
        _array_defaultclock_dt[0] = 0.0001;
        _array_neurongroup_tau[0] = 0.5;
        _array_neurongroup_sigma[0] = 0.1;
        _array_neurongroup_y[0] = 1;
        _array_statemonitor__indices[0] = 0;
        _array_defaultclock_timestep[0] = 0;
        _array_defaultclock_t[0] = 0.0;
        magicnetwork.clear();
        magicnetwork.add(&defaultclock, _run_statemonitor_codeobject);
        magicnetwork.add(&defaultclock, _run_neurongroup_stateupdater_codeobject);
        set_from_command_line(args);
        magicnetwork.run(10.0, NULL, 10.0);

	}
        

	brian_end();
        

	return 0;
}