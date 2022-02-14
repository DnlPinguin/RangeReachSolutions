#include "main.h"

int main(int argc, char **argv){
    string filename = argv[1];
    bool runHybrid = true;
    bool runSpatial = true;
    bool runSocial = true;
    bool runRangeReach = false;

    system(("Executables/runBreadthFirstQuery " + filename).c_str());


    if (runHybrid)
    {
        cout << "Run hybrid" << endl;
        

        system(("Executables/runHybrid " + filename + " straight points").c_str());
        system(("Executables/runHybrid " + filename + " straight mbr").c_str());
        system(("Executables/runHybrid " + filename + " reverse points").c_str());
        system(("Executables/runHybrid " + filename + " reverse mbr").c_str());
    }

    if (runSpatial)
    {
        cout << "Run spatial first" << endl;
        system(("Executables/runSpatialFirst " + filename + " strict points noBfl").c_str());
        system(("Executables/runSpatialFirst " + filename + " strict mbr noBfl").c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential mbr noBfl").c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential points noBfl").c_str());
        // system(("Executables/runSpatialFirst " + filename + " strict points bfl").c_str());
        // system(("Executables/runSpatialFirst " + filename + " strict mbr bfl").c_str());
        // system(("Executables/runSpatialFirst " + filename + " sequential mbr bfl").c_str());
        // system(("Executables/runSpatialFirst " + filename + " sequential points bfl").c_str());
    }

    if (runSocial)
    {
        cout << "Run social first" << endl;
        system(("Executables/runSocialFirst " + filename + " strict points").c_str());
        system(("Executables/runSocialFirst " + filename + " strict mbr").c_str());
        system(("Executables/runSocialFirst " + filename + " sequential mbr").c_str());
        system(("Executables/runSocialFirst " + filename + " sequential points").c_str());
    }

    if (runRangeReach)
    {
        cout << "Run RangeReach" << endl;
        system(("Executables/runRangeReach " + filename + " reverse mbr").c_str());
    }

   

    
}