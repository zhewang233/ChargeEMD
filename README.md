# ChargeEMD: Calculate the Earth Mover's Distance for Discrete Charge Distributions between Ground State and Excited State Electronic Structures

## Authors 
- Zhe Wang `<wang_j@berkeley.edu>`
- Jiashu Liang `<jsliang@berkeley.edu>`

## Description and Usage
The script is designed to calculate the Earth Mover's Distance for comparing ground state and excited state charge distributions, contributing to the study of electronic properties related to excitations. It employs the Transport Simplex algorithm, with the core optimization code (`emd.h`) sourced from [this repository](https://github.com/engine99/transport-simplex) by Darren T. MacDonald.

### Usage
To use the code:
1. Prepare input data, including the ground state discrete charge distribution (x, y, z, and weight for each column), excited state discrete charge distribution (x, y, z, and weight), and the smaller grid for fitting (x, y, and z). Refer to the example input files in the `example/input` directory.
2. Place `emd.h`, `main.cpp`, and input files in the same directory.
3. Compile the code using the following commands:
   ```shell
   g++ main.cpp -o emd
   ./emd

The calculated Earth Mover's Distance (EMD) results will be saved in `emd.txt`.
