#!/bin/bash

echo "--- Attempting to activate EMSDK environment ---"

# Get the root of the current Python environment (assuming it's a Pixi env)
# This finds the 'bin' directory where python is, then goes up two levels.
PIXI_ENV_ROOT=$(dirname $(dirname $(which python)))

# Dynamically determine the Python major and minor version
PYTHON_MAJOR=$(python -c "import sys; print(sys.version_info.major)")
PYTHON_MINOR=$(python -c "import sys; print(sys.version_info.minor)")

# Reconstruct the Python version directory string
PYTHON_VERSION_DIR="python${PYTHON_MAJOR}.${PYTHON_MINOR}"

# Construct the full path to emsdk_env.sh
EMSDK_ENV_SH_PATH="$PIXI_ENV_ROOT/lib/$PYTHON_VERSION_DIR/site-packages/emsdk/emsdk_env.sh"

echo "Looking for emsdk_env.sh at: $EMSDK_ENV_SH_PATH"

# Check if the script exists and source it
if [ -f "$EMSDK_ENV_SH_PATH" ]; then
    source "$EMSDK_ENV_SH_PATH"
    echo "EMSDK environment sourced successfully."
    echo "You should now have emcc and other EMSDK tools available."
    which emcc 2>/dev/null || echo "Warning: emcc not found on PATH after sourcing."
else
    echo "Error: emsdk_env.sh not found at $EMSDK_ENV_SH_PATH." >&2
    echo "Please ensure Pixi environment is active and 'pixi run setup' has been completed." >&2
    exit 1
fi
