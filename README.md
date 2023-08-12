
# ChargeEMD: Earth Mover's Distance for Discrete Charge Distributions

Calculate the Earth Mover's Distance (EMD) between ground state and excited state electronic charge distributions.

## Authors 
- Zhe Wang `<wang_j@berkeley.edu>`
- Jiashu Liang `<jsliang@berkeley.edu>`

## Description
ChargeEMD facilitates the comparison of ground and excited state charge distributions, enriching the understanding of electronic properties related to excitations. It utilizes the Transport Simplex algorithm. The core optimization code (`src/emd.h`) is sourced from [Darren T. MacDonald's repository](https://github.com/engine99/transport-simplex).

## Usage

1. **Prepare Input Data:** Prepare your input in the following three input files
   - **Fitting Grid File:** Contains coordinates for the smaller set of grid for fitting.
   - **Ground State Density File:** Contains discrete charge distribution on the original grid, with columns: coordinate (x, y, z) and charge amount.
   - **Excited State Density File:** Same format as the Ground State Density File.
   
   Refer to example files in the `example/` directory.

2. **Run the Code:** Execute in the following manner
   ```shell
   ./emd FITTING_GRID_FILE GS_DENSITY_FILE EX_DENSITY_FILE EMD_OUTPUT_FILE
   ```
   This writes the result to EMD_OUTPUT_FILE.

   Alternatively:
      ```shell
      ./emd FITTING_GRID_FILE GS_DENSITY_FILE EX_DENSITY_FILE
      ```
   This displays the result on the console.


      Usage Example:
      ```shell
      ./emd example/fitting_grid.txt example/charge_gs.txt example/charge_ex.txt emd_out.txt
      ```

3. **(Optional) Validate Output:** Compare your results with our reference output
      ```shell
      diff emd_out.txt example/emd_ref.out
      ```
      If the calculation matches, nothing will be displayed.