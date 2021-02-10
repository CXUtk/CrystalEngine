#include "Crystal.h"


int main(int argc, char** argv) {
    Crystal& crystal = Crystal::GetInstance();
    crystal.Run();
    return 0;
}
