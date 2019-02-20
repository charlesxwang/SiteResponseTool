#include "SiteResponse.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>




#include "StandardStream.h"
#include "FileStream.h"
#include "OPS_Stream.h"

#include <QDebug>

// these must be defined here!!
StandardStream sserr;
FileStream ferr("log");
OPS_Stream *opserrPtr = &ferr;
OPS_Stream *opsoutPtr = &sserr;


SiteResponse::SiteResponse(std::string configureFile,std::string anaDir,std::string outDir) :
    m_configureFile(configureFile),
    m_analysisDir(anaDir),
    m_outputDir(outDir)

{

    //./siteresponse ../test/siteLayering.loc -bbp ../test/9130326.nwhp.vel.bbp out thisLog
    // read the layering file
    std::string layersFN("/Users/simcenter/Codes/SimCenter/SiteResponseTool/test/siteLayering.loc");
    std::string bbpOName(".");
    SiteLayering siteLayers(layersFN.c_str());
    int t = siteLayers.getNumLayers();


    // read the motion
    //OutcropMotion motionX;
    //OutcropMotion motionZ;

    int inputStyle = 2; // bbp=1 opensees=2

    if (inputStyle == 1)
    {
        std::string bbpFName("/Users/simcenter/Codes/SimCenter/SiteResponseTool/test/9130326.nwhp.vel.bbp");
        bbpOName = std::string("out");
        std::string bbpLName = std::string("thisLog");
        //ferr.setFile(bbpLName.c_str(), APPEND);
        // read bbp style motion
        motionX.setBBPMotion(bbpFName.c_str(), 1);
        //motionZ.setBBPMotion(bbpFName.c_str(), 2);

        model = new SiteResponseModel(siteLayers, "2D", &motionX);
        model->setOutputDir(bbpOName);
    }
    else {
        std::string bbpLName = "Log";
        //std::string motionXFN("/Users/simcenter/Codes/SimCenter/SiteResponseTool/test/RSN766_G02_000_VEL");
        std::string motionXFN(anaDir+"/Rock");//TODO: may not work on windows
        motionX.setMotion(motionXFN.c_str());
        bbpOName = "out";
        model = new SiteResponseModel("2D", &motionX);
        model->setOutputDir(bbpOName);
        model->setAnalysisDir(anaDir);
        model->setTclOutputDir(outDir);
        model->setConfigFile(configureFile);

    }



}

void SiteResponse::run()
{
    bool runAnalysis = false;
    model->buildEffectiveStressModel2D(runAnalysis);
}


SiteResponse::~SiteResponse()
{

}

