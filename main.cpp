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

    std::vector<std::string> gFileNames{"vost_0617a.16g"};

    std::vector<GFileData> gFileData;

    for (auto name: gFileNames){
        file->open(name);

        auto _gFileData = parseGFile(file);

        std::sort(_gFileData.begin(), _gFileData.end(), [](auto a, auto b){
            return a.moment < b.moment;
        });

        if(gFileData.size() > 0){
            if(gFileData.back().moment < _gFileData[0].moment){
                gFileData.insert(gFileData.end(), _gFileData.begin(), _gFileData.end());
            }else{
                auto gFileDatum = gFileData.back();
                gFileData.pop_back();
                for(std::size_t i = 0; i < _gFileData[0].sats.size(); i++){
                    if(_gFileData[0].sats[i].isVisible){
                        gFileDatum.sats[i] = _gFileData[0].sats[i];
                    }
                }
                gFileData.push_back(gFileDatum);
                gFileData.insert(gFileData.end(), _gFileData.begin() + 1, _gFileData.end());
            }
        }else{
            gFileData.insert(gFileData.end(), _gFileData.begin(), _gFileData.end());
        }
        file->close();
    }

    std::vector<std::string> nFileNames{"vost_0617a.16n"};

    std::vector<NFileData> nFileData;

    for (auto name: nFileNames){
        file->open(name);

        auto _nFileData = parseNFile(file);

        std::sort(_nFileData.begin(), _nFileData.end(), [](auto a, auto b){
            return a.moment < b.moment;
        });

        if(nFileData.size() > 0){
            if(nFileData.back().moment < _nFileData[0].moment){
                nFileData.insert(nFileData.end(), _nFileData.begin(), _nFileData.end());
            }else{
                auto nFileDatum = nFileData.back();
                nFileData.pop_back();
                for(std::size_t i = 0; i < _nFileData[0].sats.size(); i++){
                    if(_nFileData[0].sats[i].isVisible){
                        nFileDatum.sats[i] = _nFileData[0].sats[i];
                    }
                }
                nFileData.push_back(nFileDatum);
                nFileData.insert(nFileData.end(), _nFileData.begin() + 1, _nFileData.end());
            }
        }else{
            nFileData.insert(nFileData.end(), _nFileData.begin(), _nFileData.end());
        }
        file->close();
    }

     std::vector<std::string> oFileNames{"vost_0617a.16o"};

     std::vector<OFileData> oFileData;

     for (auto name: oFileNames){
         file->open(name);

         auto _oFileData = parseOFile(file);

         oFileData.insert(oFileData.end(), _oFileData.begin(), _oFileData.end());

         file->close();
     }

    auto rSatPositions = RSatPositions(oFileData, gFileData);
    auto gSatPositions = GSatPositions(oFileData, nFileData);

    std::array<double, 3> point{-370700.2945,  1225384.5150, -6230930.2438};

    auto TECR = TEC<TECvalR>(rSatPositions, point);

    return 0;
}
