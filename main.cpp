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

    std::cout<<iData[0].moment.getPosixTime();

    std::cout<<123;
    return 0;
}
