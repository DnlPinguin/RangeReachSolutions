#include "main.h"

int main(){
    bool runHybrid = true;
    bool runSpatial = true;
    bool runSocial = true;
    bool runRangeReach = true;

    if (runHybrid)
    {
        cout << "Run hybrid" << endl;
        system("Executables/runHybrid small straight points");
        system("Executables/runHybrid small straight mbr");
        system("Executables/runHybrid small reverse points");
        system("Executables/runHybrid small reverse mbr");
    }

    if (runSpatial)
    {
        cout << "Run spatial first" << endl;
        system("Executables/runSpatialFirst small strict points noBfl");
        system("Executables/runSpatialFirst small strict mbr noBfl");
        system("Executables/runSpatialFirst small sequential mbr noBfl");
        system("Executables/runSpatialFirst small sequential points noBfl");
        system("Executables/runSpatialFirst small strict points bfl");
        system("Executables/runSpatialFirst small strict mbr bfl");
        system("Executables/runSpatialFirst small sequential mbr bfl");
        system("Executables/runSpatialFirst small sequential points bfl");
    }

    if (runSocial)
    {
        cout << "Run social first" << endl;
        system("Executables/runSocialFirst small strict points");
        system("Executables/runSocialFirst small strict mbr");
        system("Executables/runSocialFirst small sequential mbr");
        system("Executables/runSocialFirst small sequential points");
    }

    if (runRangeReach)
    {
        cout << "Run RangeReach" << endl;
        system("Executables/runRangeReach small reverse mbr");
    }

   

    
}