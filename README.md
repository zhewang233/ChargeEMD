# ChargeEMD: Calculate the Earth Mover's Distance for discrete charge distributions between ground state and excited state electronic structures

## Authors 
* Zhe Wang `<wang_j@berkeley.edu>`
* Jiashu Liang `<jsliang@berkeley.edu>`

## Description and Usage
The script is to calculate the Earth Mover's Distance for ground state and excited state charge distributions to study electronic properties related to the excitation. It takes Transport Simplex algorithm and the main code for optimization (emd.h) is taken from https://github.com/engine99/transport-simplex (from Darren T. MacDonald).

To use the code, please prepare the input including the ground state discrete charge distribution (x, y, z, and weight for each column), excited state discrete charge distribution (x, y, z, and weight), and the smaller grid for fitting (x, y, and z) according to the files under `example/input`. Then put the `emd.h`, `main.cpp`, and the inputs under the same directory. Then run the following commands:

    g++ main.cpp -o emd
    ./emd

The output will be stored in `emd.txt`.
