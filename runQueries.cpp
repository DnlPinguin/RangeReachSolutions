#include "main.h"

int main(){
    cout << "Run hybrid" << endl;
    system("Executables/runHybrid small points");
    system("Executables/runHybrid small mbr");

    cout << "Run socail first" << endl;
    system("Executables/runSocialFirst small strict points");
    system("Executables/runSocialFirst small strict mbr");
    system("Executables/runSocialFirst small sequential mbr");
    system("Executables/runSocialFirst small sequential points");

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