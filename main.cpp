#include <fstream>
#include "NFileParser.h"
#include "GFileParser.h"
#include "OFileParser.h"
#include "IFileParser.h"
#include "models.h"
#include <algorithm>
#include "Tec.h"

int main(){
    std::ifstream *file = new std::ifstream;

    std::vector<std::string> iFileNames{"codg1680.16i"};

    file->open(iFileNames[0]);

    auto iData = parseIFile(file);

    file->close();

    std::array<double, 2> point1 {-177.5 * M_PI / 180, -77.5 * M_PI / 180};
    std::cout<<TECmapSpher(iData, iData[0].moment, point1);

//    std::cout<<DecToSpher(point)[1]*180/M_PI;

   // std::cout<<123;
    return 0;
}
