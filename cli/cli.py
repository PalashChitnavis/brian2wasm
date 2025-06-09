import argparse
import sys
import os
import re # For regular expressions to parse the .sh file
import shutil # For shutil.which to find executables

def set_emsdk_environment():
    """
    Locates the emsdk environment script within the Pixi environment
    and sets the necessary environment variables in the current process.
    This ensures that tools like 'emcc' are found by Brian2WASM.
    """
    try:
        # Get the root path of the current Python environment managed by Pixi.
        # sys.prefix typically points to the environment's base directory
        # e.g., /Users/palashchitnavis/Desktop/brian2wasm/.pixi/envs/default
        env_prefix = sys.prefix

        # Construct the full, expected path to emsdk_env.sh
        # This path is specific to how the 'emsdk' conda-forge package is laid out.
        # It dynamically uses the Python major and minor version for robustness.
        emsdk_env_sh_path = os.path.join(
            env_prefix,
            'lib',
            f'python{sys.version_info.major}.{sys.version_info.minor}',
            'site-packages',
            'emsdk',
            'emsdk_env.sh'
        )

        if not os.path.isfile(emsdk_env_sh_path):
            print(f"Warning: emsdk_env.sh not found at expected path: '{emsdk_env_sh_path}'", file=sys.stderr)
            print("Please ensure 'pixi run setup-emsdk' has been executed successfully to install the SDK.", file=sys.stderr)
            return

        # Read the emsdk_env.sh script content
        with open(emsdk_env_sh_path, 'r') as f:
            lines = f.readlines()

        # Regular expression to parse 'export VAR="VALUE"' or 'export VAR=VALUE' lines
        # Group 1: variable name
        # Group 2: value (if quoted)
        # Group 3: value (if unquoted)
        export_pattern = re.compile(r'^\s*export\s+([A-Za-z_][A-Za-z0-9_]*)=(?:"(.*?)"|(.*))$')

        # Collect new PATH components to prepend later
        new_path_components = []
        path_var_name = "PATH"
        current_path_env = os.environ.get(path_var_name, "")

        # Process each line to set environment variables
        for line in lines:
            match = export_pattern.match(line)
            if match:
                var_name = match.group(1)
                value = match.group(2) if match.group(2) is not None else match.group(3)

                # Basic expansion for variables commonly used by emsdk_env.sh itself
                # This handles cases like: export PATH="$EMSDK_ACTIVE/upstream/bin:$PATH"
                # It's a simplification; a full shell script parser is very complex.
                value = value.replace('$EMSDK_ACTIVE', os.environ.get('EMSDK_ACTIVE', ''))
                value = value.replace('$EMSDK', os.environ.get('EMSDK', ''))
                value = value.replace('${EMSDK_ACTIVE}', os.environ.get('EMSDK_ACTIVE', ''))
                value = value.replace('${EMSDK}', os.environ.get('EMSDK', ''))

                if var_name == path_var_name:
                    # emsdk_env.sh typically prepends paths.
                    # We collect these prepended parts and will join them with the existing PATH.
                    path_parts_from_script = value.split(':')
                    for part in path_parts_from_script:
                        if part and part != '$PATH' and part != '${PATH}': # Avoid adding '$PATH' literal
                            new_path_components.append(part)
                else:
                    # Set other environment variables directly
                    os.environ[var_name] = value

        # Prepend the collected new path components to the existing PATH
        if new_path_components:
            # os.pathsep is ':' on Unix-like systems and ';' on Windows
            final_path_list = new_path_components + [current_path_env]
            os.environ[path_var_name] = os.pathsep.join(filter(None, final_path_list)) # filter(None) removes empty strings

        # Verify if 'emcc' is now found on the PATH after setting variables
        if shutil.which("emcc"):
            print("EMSDK environment variables successfully set internally by brian2wasm CLI.")
        else:
            print("Warning: 'emcc' still not found after attempting to set EMSDK environment variables.", file=sys.stderr)
            print("This may indicate an issue with the emsdk installation or path resolution.", file=sys.stderr)

    except Exception as e:
        print(f"Error encountered while setting EMSDK environment internally: {e}", file=sys.stderr)


# Original brian2wasm CLI tool code, with the new function call
def main():
    # Call the new function to set up EMSDK environment variables
    set_emsdk_environment()

    parser = argparse.ArgumentParser(
        description="Brian2WASM CLI"
    )
    parser.add_argument(
        "script",
        help="Path to the Python script to run"
    )
    args = parser.parse_args()

    script_path = args.script

    # Check if the script exists and is a Python file
    if not os.path.isfile(script_path):
        print(f"Error: File '{script_path}' does not exist.", file=sys.stderr)
        sys.exit(1)
    if not script_path.endswith(".py"):
        print(f"Error: File '{script_path}' is not a Python script (.py).", file=sys.stderr)
        sys.exit(1)

    # Read the original script
    with open(script_path, 'r') as f:
        script_content = f.read()

    # Get the script's directory and name
    script_dir = os.path.dirname(script_path) or '.'
    script_name = os.path.splitext(os.path.basename(script_path))[0]

    # Check if an HTML file with the same name exists
    html_file = f"{script_name}.html"
    html_file_path = os.path.join(script_dir, html_file)
    has_html_file = os.path.isfile(html_file_path)

    # Inject the required lines at the top
    if has_html_file:
        print(f"html file found: '{html_file_path}'")
        injection = (
            "from brian2 import *\n"
            "import brian2wasm\n"
            f"set_device('wasm_standalone', directory='{script_name}', html_file='{html_file}')\n"
        )
    else:
        print(f"html file not found: using default html template")
        injection = (
            "from brian2 import *\n"
            "import brian2wasm\n"
            f"set_device('wasm_standalone', directory='{script_name}')\n"
        )
    modified_script = injection + script_content

    # Set the working directory to the script's directory
    original_cwd = os.getcwd()
    os.chdir(script_dir)

    try:
        # Execute the modified script in memory with __file__ set
        print(f"Script path: {os.path.abspath(script_path)}")
        print(f"Directory: {script_dir}")
        exec_globals = {'__name__': '__main__', '__file__': os.path.abspath(script_path)}
        exec(modified_script, exec_globals)
    except Exception as e:
        print(f"Error running script: {e}", file=sys.stderr)
        sys.exit(1)
    finally:
        # Restore the original working directory
        os.chdir(original_cwd)

if __name__ == "__main__":
    main()