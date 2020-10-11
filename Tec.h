#ifndef TEC_H
#define TEC_H

#include <array>
#include <cmath>
#include <vector>
#define _USE_MATH_DEFINES

#include "models.h"
#include "utils.h"
#include "dateTime.h"
#include "IFileParser.h"

const double H_ION = 450000.0;
const double RC = 6371000.0;
const double K_ION = 40.308;

struct TECval{
    double tecC = nan("empty value");
    double tecL = nan("empty value");
};

struct TECvalR{
    std::array<TECval, 24> sat;
    DateTime moment;
};

struct TECvalG{
    std::array<TECval, 32> sat;
    DateTime moment;
};

double TECmapSpher(std::vector<IFileData>& iFileData, DateTime time, std::array<double, 2>);

double TECmapDec(std::vector<IFileData>& iFileData, DateTime time, std::array<double, 3>);

double IPPTEC(std::vector<IFileData>&, DateTime, std::array<double, 3>&, std::array<double, 3>&);

std::vector<TECvalG> TEC(std::vector<GSatsParams>& satParamsData,std::array<double, 3>& pointPos);

std::vector<TECvalR> TEC(std::vector<RSatsParams>& satParamsData,std::array<double, 3>& pointPos);

template < class TEC_VAL>
std::vector<TEC_VAL>  resolvePhaseAmbiguity(std::vector<TEC_VAL> TECData){
    for(std::size_t index = 0; index < TECData[0].sat.size(); index++){
        double diff = 0;
        int count = 0, list = 0;
        std::size_t i1;
        for(std::size_t i = 0;i <TECData.size(); i++){
            if(!isnan(TECData[i].sat[index].tecC) && !isnan(TECData[i].sat[index].tecL))
             {
                if(count == 0)   {
                    i1=i;
                }
                  diff += (TECData[i].sat[index].tecL - TECData[i].sat[index].tecC);
                  count ++;
                  list = 0;
             }
            else
                list++;
            if(list > 10 && count > 1)
            {
                for(;i1<i;i1++){
                    if(!isnan(TECData[i1].sat[index].tecL))
                    {
                        TECData[i1].sat[index].tecL -= diff/count;
                    }
                }

                list = 0;
                diff = 0;
                count = 0;
            }
        }
    }
    return TECData;
}

template < class TEC_VAL, class SAT_PARAMS>
std::vector<TEC_VAL>  resolveBIAS(std::vector<TEC_VAL> TECData, std::vector<SAT_PARAMS>& satParamsData, std::array<double, 3>& point, std::vector<IFileData>& iFileData){
    for(std::size_t index = 0; index < TECData[0].sat.size(); index++){
        double diff = 0;
        int count = 0, list = 0;
        std::size_t i1;
        for(std::size_t i = 0;i <TECData.size(); i++){
            double ippTEC = IPPTEC(iFileData, TECData[i].moment, satParamsData[i].sat[index].coord, point);
            if(!isnan(TECData[i].sat[index].tecC) && ippTEC!=9999)
             {
                if(count == 0)   {
                    i1=i;
                }
                  diff += (TECData[i].sat[index].tecC - ippTEC);
                  count ++;
                  list = 0;
             }
            else
                list++;
            if(list > 10 && count > 1)
            {
                for(;i1<i;i1++){
                    if(!isnan(TECData[i1].sat[index].tecC))
                    {
                        TECData[i1].sat[index].tecC -= diff/count;
                    }
                }

                list = 0;
                diff = 0;
                count = 0;
            }
        }
    }
    return TECData;
}

template < class TEC_VAL, class SAT_PARAMS>
std::vector<TEC_VAL> _TEC(std::vector<SAT_PARAMS>& satParamsData,std::array<double, 3>& pointPos) {
    std::vector<TEC_VAL> TECData;
    double Rr = l2<double, 3>(pointPos);

    for(auto satParams: satParamsData){
        TEC_VAL TECDatum;
        std::size_t i =0;
        for(auto &sat: satParams.sats){
            if(sat.isVisible){
                std::array<double, 3> RsRr{sat.coord[0] -  pointPos[0], sat.coord[1] -  pointPos[1], sat.coord[2] -  pointPos[2]};
                double cosz = (RsRr[0]*pointPos[0] + RsRr[1] *pointPos[1] + RsRr[2] *pointPos[2]) / (Rr * l2<double, 3>(RsRr));
                if (std::asin(cosz) * 180 / M_PI < 10) continue;
                double cosz_ = pow((1 - (1 - cosz * cosz) * pow(Rr / (RC + H_ION), 2)), 0.5);
                TECval TECValues;

                if(!isnan(sat.P2) && !isnan(sat.P1))
                     TECValues.tecC = (sat.P2                                   - sat.P1                                  ) * std::pow(f1(sat) * f2(sat), 2) / (K_ION * (std::pow(f1(sat), 2) - std::pow(f2(sat), 2)))* cosz_ / pow(10, 16);
                if(!isnan(sat.L1) && !isnan(sat.L2))
                    TECValues.tecL = (sat.L1*SPEED_OF_LIGHT / f1(sat) - sat.L2*SPEED_OF_LIGHT / f2(sat)) * std::pow(f1(sat) * f2(sat), 2) / (K_ION * (std::pow(f1(sat), 2) - std::pow(f2(sat), 2)))* cosz_ / pow(10, 16);

                TECDatum.sat[i] = TECValues;
            }
            i++;
        }
        TECDatum.moment = satParams.moment;
        TECData.push_back(TECDatum);
    }
    return TECData;
}

#endif

