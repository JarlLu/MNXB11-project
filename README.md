# MNXB11-project
# About
This is the final project repository for team 2 consisting of Jarl, Hanna and Yoline. With the code in this repository we try to make observations regarding 3 questions about the Swedish climate. The first being how the temperature of Uppsala has evolved over time using the long time dataset and seeing if we can see signs of climate change. The second being whether there is any difference between the average temperatures of the week days which we expect there not to be, but interesting none the less. The final being investigating if the temperature depends on the day of the month in a few locations, which we also expect to not have any clear trend.




# Running the project
By following these instruction you will be able to reproduce the results of our project including all of the plots used in the report
## Before Running set up external library  
1. Go to your home directory
```bash  
cd ~/
```
2. Create external library directory
```bash
mkdir -p external/include
```
3. Go into directory
```bash
cd external/include 
```
4. Clone date library
```bash
git clone https://github.com/HowardHinnant/date.git date  
```

# How to run

1. In MNXB11-project directory

2. To build project:
```bash
cd scripts/
./buildproject.sh
```

3. To run project:
```bash
cd scripts/
./runproject.sh
```
**Note:** `runproject.sh` runs the executable `main` so any code you want to run should be written in `main.cxx`. Or change which executable it runs. That works too :) Also make sure that paths are correct! Keep in mind that you cannot open a file that doesn't exist so generate ROOT file first and then open that ROOT file, must be in correct order.


## Running project as a SLURM job  
1. Go to slurm directory  
```bash
cd slurm/ 
```
2. Create job on cluster  
```bash
sbatch project.sbatch
```
3. Output is found in slurm-(jobid).output  
```bash
cat slurm-(jobid).output  
```
# Examples
## DataPoint class

```cpp 
#include "DataPoint.h"
#include <iostream>

using namespace project;
int main() {
    project::DataPoint datapoint = project::DataPoint("1880-01-01", 20, 20.3, true);
    std::cout << "Date: " << datapoint.get_date() << "\n";
    std::cout << "Temp: " << datapoint.get_temperature() << "\n";
}
```

