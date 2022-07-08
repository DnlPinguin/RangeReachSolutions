#!/bin/bash

#SBATCH -J createIntervalSchemeOne         # Job name
#SBATCH -A m2_jgu-evgeosreachq          # Account name
#SBATCH -C broadwell                    # MachineType
#SBATCH -p parallel                     # Queue name
#SBATCH -N 1                # Reserving whole mache
#SBATCH -n 1                # Number of tasks
#SBATCH -c 8                # Number of CPUs
#SBATCH --mem=32G           # Memory per node
#SBATCH -t 1200               # Time in minutes


# Next we load all modules we need. Loading these in the script ensures a consistent environment.
module purge
module load devel/Boost/1.77.0-GCC-11.2.0

# replace the name with the program name
datasource="yelp_wcc"
number_of_threads=4
machine_id=2
total_number_of_machines=5

# Build the executable
make clean
make createIntervalScheme


# Launch executable
srun -J createIntervalSchemeOne -A m2_jgu-evgeosreachq -C broadwell -p parallel -N 1 -n 1 -c 8 --mem=32G -t 1200 ./Executables/createIntervalScheme $datasource parallel $number_of_threads $machine_id $total_number_of_machines
