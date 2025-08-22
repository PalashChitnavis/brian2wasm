PROGRAM = wasm_module.js

IGNORE := $(shell bash -c "/home/runner/work/brian2wasm/brian2wasm/.pixi/envs/default/lib/python3.13/site-packages/emsdk/emsdk activate latest && source /home/runner/work/brian2wasm/brian2wasm/.pixi/envs/default/lib/python3.13/site-packages/emsdk/emsdk_env.sh; env | sed 's/=/:=/' | sed 's/^/export /' > makeenv")
include makeenv

SRCS = code_objects/neurongroup_stateupdater_codeobject.cpp code_objects/statemonitor_codeobject.cpp main.cpp network.cpp objects.cpp run.cpp synapses_classes.cpp
H_SRCS = brianlib/clocks.h brianlib/common_math.h brianlib/dynamic_array.h code_objects/neurongroup_stateupdater_codeobject.h code_objects/statemonitor_codeobject.h network.h objects.h run.h synapses_classes.h
OBJS = ${SRCS:.cpp=.o}
OPTIMISATIONS = -w -I/home/runner/work/brian2wasm/brian2wasm/.pixi/envs/default/include -O3 -ffast-math -fno-finite-math-only -std=c++11
CXXFLAGS = -c -Wno-write-strings $(OPTIMISATIONS) -I.   -fwasm-exceptions 
LFLAGS =  $(OPTIMISATIONS) -L/home/runner/work/brian2wasm/brian2wasm/.pixi/envs/default/lib  -fwasm-exceptions -sALLOW_MEMORY_GROWTH
all: $(PROGRAM)

.PHONY: all clean

$(PROGRAM): $(OBJS) $(DEPS) makefile /home/runner/work/brian2wasm/brian2wasm/brian2wasm/templates/pre.js
	emcc $(OBJS) -o $(PROGRAM) $(LFLAGS)  --pre-js /home/runner/work/brian2wasm/brian2wasm/brian2wasm/templates/pre.js -sVERBOSE=1 -sMODULARIZE=1 -sENVIRONMENT=worker -sEXPORTED_FUNCTIONS=_main -sEXPORTED_RUNTIME_METHODS=callMain -sINVOKE_RUN=0

clean:
	rm $(OBJS) $(PROGRAM) $(DEPS)

make.deps: $(SRCS) $(H_SRCS)
	emcc $(CXXFLAGS) -MM $(SRCS) > make.deps
	
ifneq ($(wildcard $(DEPS)), )
include $(DEPS)
endif

%.o : %.cpp makefile
	emcc $(CXXFLAGS) $< -o $@