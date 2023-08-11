# ChargeEMD: Calculate the Earth Mover's Distance for Discrete Charge Distributions between Ground State and Excited State Electronic Structures

## Authors 
- Zhe Wang `<wang_j@berkeley.edu>`
- Jiashu Liang `<jsliang@berkeley.edu>`

## Description and Usage
The script is designed to calculate the Earth Mover's Distance for comparing ground state and excited state charge distributions, contributing to the study of electronic properties related to excitations. It employs the Transport Simplex algorithm, with the core optimization code (`src/emd.h`) sourced from [this repository](https://github.com/engine99/transport-simplex) by Darren T. MacDonald.

### Usage
To use the code:
1. Prepare input data, including the ground state discrete charge distribution (x, y, z, and weight for each column), excited state discrete charge distribution (x, y, z, and weight), and the smaller grid for fitting (x, y, and z). Refer to the example input files in the `example/input` directory.
2. Run the code in the following format:
   ```shell
   ./emd FITTING_GRID_FILE GS_DENSITY_FILE EX_DENSITY_FILE EMD_OUTPUT_FILE
   ```

   For example:
   ```shell
   ./emd ./example/input/fitting_grid.txt ./example/input/charge_gs.txt ./example/input/charge_ex.txt ./example/output/emd.txt
   ```