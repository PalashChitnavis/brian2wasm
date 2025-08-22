
#ifndef _BRIAN_OBJECTS_H
#define _BRIAN_OBJECTS_H

#include "synapses_classes.h"
#include "brianlib/clocks.h"
#include "brianlib/dynamic_array.h"
#include "brianlib/stdint_compat.h"
#include "network.h"
#include<random>
#include<vector>


namespace brian {

extern std::string results_dir;

class RandomGenerator {
    private:
        std::mt19937 gen;
        double stored_gauss;
        bool has_stored_gauss = false;
    public:
        RandomGenerator() {
            seed();
        }
        void seed() {
            std::random_device rd;
            gen.seed(rd());
            has_stored_gauss = false;
        }
        void seed(unsigned long seed) {
            gen.seed(seed);
            has_stored_gauss = false;
        }
        double rand() {
            /* shifts : 67108864 = 0x4000000, 9007199254740992 = 0x20000000000000 */
            const long a = gen() >> 5;
            const long b = gen() >> 6;
            return (a * 67108864.0 + b) / 9007199254740992.0;
        }

        double randn() {
            if (has_stored_gauss) {
                const double tmp = stored_gauss;
                has_stored_gauss = false;
                return tmp;
            }
            else {
                double f, x1, x2, r2;

                do {
                    x1 = 2.0*rand() - 1.0;
                    x2 = 2.0*rand() - 1.0;
                    r2 = x1*x1 + x2*x2;
                }
                while (r2 >= 1.0 || r2 == 0.0);

                /* Box-Muller transform */
                f = sqrt(-2.0*log(r2)/r2);
                /* Keep for next call */
                stored_gauss = f*x1;
                has_stored_gauss = true;
                return f*x2;
            }
        }
};


// In OpenMP we need one state per thread
extern std::vector< RandomGenerator > _random_generators;

//////////////// clocks ///////////////////
extern Clock defaultclock;

//////////////// networks /////////////////
extern Network magicnetwork;

void set_variable_by_name(std::string, std::string);

extern std::map<std::string, std::tuple<size_t, std::string, void*>> array_meta_data;

//////////////// dynamic arrays ///////////
extern std::vector<double> _dynamic_array_statemonitor_t;

//////////////// arrays ///////////////////
extern double *_array_defaultclock_dt;
extern const int _num__array_defaultclock_dt;
extern double *_array_defaultclock_t;
extern const int _num__array_defaultclock_t;
extern int64_t *_array_defaultclock_timestep;
extern const int _num__array_defaultclock_timestep;
extern int32_t *_array_neurongroup_i;
extern const int _num__array_neurongroup_i;
extern double *_array_neurongroup_sigma;
extern const int _num__array_neurongroup_sigma;
extern double *_array_neurongroup_tau;
extern const int _num__array_neurongroup_tau;
extern double *_array_neurongroup_y;
extern const int _num__array_neurongroup_y;
extern int32_t *_array_statemonitor__indices;
extern const int _num__array_statemonitor__indices;
extern int32_t *_array_statemonitor_N;
extern const int _num__array_statemonitor_N;
extern double *_array_statemonitor_y;
extern const int _num__array_statemonitor_y;

//////////////// dynamic arrays 2d /////////
extern DynamicArray2D<double> _dynamic_array_statemonitor_y;

/////////////// static arrays /////////////

//////////////// synapses /////////////////

// Profiling information for each code object
}

void _init_arrays();
void _load_arrays();
void _write_arrays();
void _dealloc_arrays();

#endif


