# MNXB11-project
# About
Describe project here

# Directory structure

The base directory here contains your "main" file, i.e. the C++ file that you
will be using as your starting point in your project. The code in this file should ideally be short and just make use of the functionality that you've
prepared in your other translation units. The majority of your code should be placed in  the two folders where you define your translation units, `src/` and `include/`. 


The main files that can be a starting point are:
- [rootlogon.C](rootlogon.C)
  - This file contains code that ROOT will execute automatically whenever you start it, a good place to place general style choices you want to make or anything else you always want to run!
  - Be careful to not include anything that depends on your particular machine here (e.g. absolute paths)
- [main.cxx](main.cxx)
  - This file contains a sample `main` function for your project. You should start with this file if you want to build the project as a C++ binary instead of using the ROOT interpreter.
- [Makefile](Makefile)
  - This file gives the `make` utility instructions on how to build different targets. For your convenience there is also a `clean` target that can be used to delete built files to start anew.
  - You should use this file if you plan to automate the C++ build of your project, but don't use it as a script. It is meant for C++ stuff only - use a BASH script for environment variables and task automation instead.

We have also included three special files in the base of the repository 
- [.gitignore](.gitignore)
  - This file contains regular expressions that git tells git that it shouldn't add certain file to your repository. 
  - Your git repository should generally not contain binary files like object files or executables nor should it contain build artefacts like external libraries. 
- [.clang-format](.clang-format)
  - This file holds the configuration for the clang-format tool that you can use to format your code consistently 
  - It is a good idea to keep your code formatted in a consistent manner, especially when working in groups but doing it manually is a waste of your time. Use a tool for it!
  ```
  # Show what the src/Example.cxx file would look like if formatted
  clang-format src/Example.cxx 
  # Carry out the formatting in the file directly 
  clang-format src/Example.cxx -i
  ```
  - The `.clang-format` file holds the configuration that clang-format will use to determine how to format your code. By default, it will be formatted according to Google's style but you can pick any that you like from https://clang.llvm.org/docs/ClangFormatStyleOptions.html

The [datasets](datasets) folder contains open data from SMHI and a README.md with further information about it.

The [scripts](scripts) folder contains BASH scripts examples that can be used to create a build script or a launcher/wrapper for your binaries or for your SLURM submission

The [slurm](slurm) folder contains an example SLURM SBATCH script that calls one of the project launcher scripts above.


# Building the project

## Before Running set up external library  
1. Go to your home directory  
cd ~/
2. Create external library directory  
mkdir -p external/include  
3. Go into directory  
cd external/include  
4. Clone date library  
git clone https://github.com/HowardHinnant/date.git date  


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

## CSV to ROOT
```cpp
#include "DataPoint.h"
#include "Csv2TTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TTree.h>

using namespace project;
  int main() {

    project::csv_to_root("datasets/testdata2.csv", "datasets/rootfiles/testroot2.root" );
    
    TFile file("/home/hannap/mnxb11/MNXB11-project/datasets/rootfiles/testroot2.root", "READ");
    file.ls();

    TTree* tree = nullptr;
    file.GetObject("temperature", tree);

    if (tree) {
        tree->Print();
    }

      return 0;
    }

```
## Running project as a SLURM job  
1. Go to slurm directory  
cd slurm/   
2. Create job on cluster  
sbatch project.sbatch   
3. Output is found in slurm-(jobid).output  
cat slurm-(jobid).output  



