#include "main.h"

int main(int argc, char **argv){
    string filename = argv[1];
    string queryFile = argv[2];

    bool runHybrid = false;
    bool runSpatial = true;
    bool runSocial = false;
    bool runRangeReach = false;

    system(("Executables/runBreadthFirstQuery " + filename + " " + queryFile).c_str());

    if (runHybrid)
    {
        cout << "Run hybrid" << endl;
        

        system(("Executables/runHybrid " + filename + " straight points " + queryFile).c_str());
        system(("Executables/runHybrid " + filename + " straight mbr " + queryFile).c_str());
        system(("Executables/runHybrid " + filename + " reverse points " + queryFile).c_str());
        system(("Executables/runHybrid " + filename + " reverse mbr " + queryFile).c_str());
    }

    if (runSpatial)
    {
        cout << "Run spatial first" << endl;
        // system(("Executables/runSpatialFirst " + filename + " strict points noBfl " + queryFile).c_str());
        // system(("Executables/runSpatialFirst " + filename + " strict mbr noBfl " + queryFile).c_str());
        // system(("Executables/runSpatialFirst " + filename + " sequential mbr noBfl " + queryFile).c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential points noBfl " + queryFile).c_str());
        // system(("Executables/runSpatialFirst " + filename + " strict points bfl " + queryFile).c_str());
        // system(("Executables/runSpatialFirst " + filename + " strict mbr bfl " + queryFile).c_str());
        // system(("Executables/runSpatialFirst " + filename + " sequential mbr bfl " + queryFile).c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential points bfl " + queryFile).c_str());
    }

    if (runSocial)
    {
        cout << "Run social first" << endl;
        system(("Executables/runSocialFirst " + filename + " strict points " + queryFile).c_str());
        system(("Executables/runSocialFirst " + filename + " strict mbr " + queryFile).c_str());
        system(("Executables/runSocialFirst " + filename + " sequential mbr " + queryFile).c_str());
        system(("Executables/runSocialFirst " + filename + " sequential points " + queryFile).c_str());
    }

    if (runRangeReach)
    {
        cout << "Run RangeReach" << endl;
        system(("Executables/runRangeReach " + filename + " " + queryFile).c_str());
    }

   

    
}