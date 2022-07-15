#include "main.h"


string getFileName(){
    string input;
    cout << "Enter filename:" << endl;
    cin >> input;
    return input;
}

string getQueryFileName(){
    string input;
    cout << "Enter query filename:" << endl;
    cin >> input;
    return input;
}

int main(int argc, char **argv){
    string filename, query_filename;
    if (argc == 3){
        filename = argv[1];
        query_filename = argv[2];
    } else {
        filename = getFileName();
        query_filename = getQueryFileName();
    }
    
    bool runHybrid = true;
    bool runSpatial = true;
    bool runSocial = true;
    bool runRangeReach = true;
    bool runBfl = true;

    system(("Executables/runBreadthFirstQuery " + filename + " " + query_filename).c_str());

    if (runHybrid)
    {
        cout << "Run hybrid" << endl;
        system(("Executables/runHybrid " + filename + " straight points " + query_filename).c_str());
        system(("Executables/runHybrid " + filename + " straight mbr " + query_filename).c_str());
        system(("Executables/runHybrid " + filename + " reverse points " + query_filename).c_str());
        system(("Executables/runHybrid " + filename + " reverse mbr " + query_filename).c_str());
    }
    if (runBfl){
        cout << "Run Spatial first with Bfl" << endl;
        system(("Executables/runSpatialFirst " + filename + " strict points bfl " + query_filename).c_str());
        system(("Executables/runSpatialFirst " + filename + " strict mbr bfl " + query_filename).c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential mbr bfl " + query_filename).c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential points bfl " + query_filename).c_str());
    }
    if (runSpatial)
    {
        cout << "Run Spatial First" << endl;
        system(("Executables/runSpatialFirst " + filename + " strict points noBfl " + query_filename).c_str());
        system(("Executables/runSpatialFirst " + filename + " strict mbr noBfl " + query_filename).c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential mbr noBfl " + query_filename).c_str());
        system(("Executables/runSpatialFirst " + filename + " sequential points noBfl " + query_filename).c_str());
    }

    if (runSocial)
    {
        cout << "Run social first" << endl;
        system(("Executables/runSocialFirst " + filename + " strict points " + query_filename).c_str());
        system(("Executables/runSocialFirst " + filename + " strict mbr " + query_filename).c_str());
        system(("Executables/runSocialFirst " + filename + " sequential mbr " + query_filename).c_str());
        system(("Executables/runSocialFirst " + filename + " sequential points " + query_filename).c_str());
    }

    if (runRangeReach)
    {
        cout << "Run RangeReach" << endl;
        system(("Executables/runRangeReach " + filename + " " + query_filename).c_str());
    }
}