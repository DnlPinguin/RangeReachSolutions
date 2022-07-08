#!/bin/bash

#SBATCH -J createIntervalScheme         # Job name
#SBATCH -A m2_jgu-evgeosreachq          # Account name
#SBATCH -p bigmem                       # Queue name
#SBATCH -n 1                # Number of tasks
#SBATCH -c 8                # Number of CPUs
#SBATCH --gres=gpu:1        # Total number of GPUs
#SBATCH --mem=10G           # Memory per node
#SBATCH -t 600               # Time in minutes


# Next we load all modules we need. Loading these in the script ensures a consistent environment.
module purge
module load devel/Boost/1.77.0-GCC-11.2.0

# replace the name with the program name
datasource="yelp_wcc"
number_of_threads=4
machine_id=0
total_number_of_machines=5

# Build the executable
make clean
make createIntervalScheme


# Launch executable
./Executables/createIntervalScheme $datasource parallel $number_of_threads $machine_id $total_number_of_machines
