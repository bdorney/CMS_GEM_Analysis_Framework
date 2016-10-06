#include "SRSConfiguration.h"

SRSConfiguration::SRSConfiguration() {
  this->Init();
}
SRSConfiguration::SRSConfiguration(const char * file) {
  this->Init(file);
}
SRSConfiguration::~SRSConfiguration(){
}

bool SRSConfiguration::FileExists(const char* name) const {
  std::ifstream f(name);
  if (f.good()) {
    f.close();
    return true;
  } else {
    f.close();
    return false;
  }
}


//============================================================================================
SRSConfiguration & SRSConfiguration::operator=(const SRSConfiguration &rhs) {
  fRunType                 = rhs.GetRunType() ;
  fRunName                 = rhs.GetRunName() ;
  fZeroSupCut              = rhs.GetZeroSupCut() ;
  fROOTDataType            = rhs.GetROOTDataType();
  fRunNbFile               = rhs.GetRunNbFile();
  fMappingFile             = rhs.GetMappingFile();
  fPadMappingFile          = rhs.GetPadMappingFile();
  fSavedMappingFile        = rhs.GetSavedMappingFile();
  fHistosFile              = rhs.GetHistoCfgName();
  fDisplayFile             = rhs.GetDisplayCfgName();
  fPedestalFile            = rhs.GetPedestalFile() ;
  fTrackingOffsetDir       = rhs.GetTrackingOffsetDir();
  fPositionCorrectionFile  = rhs.GetClusterPositionCorrectionFile() ;
  fPositionCorrectionFlag  = rhs.GetClusterPositionCorrectionFlag() ;
  fMaskedChannelCut        = rhs.GetMaskedChannelCut() ;
  fStartEventNumber        = rhs.GetStartEventNumber() ;
  fEventFrequencyNumber    = rhs.GetEventFrequencyNumber() ;
  fRawPedestalFile         = rhs.GetRawPedestalFile() ;
  fMinClusterSize          = rhs.GetMinClusterSize() ;
  fMaxClusterSize          = rhs.GetMaxClusterSize() ;
  fMaxClusterMultiplicity  = rhs.GetMaxClusterMultiplicity() ;
  fIsHitMaxOrTotalADCs     = rhs.GetHitMaxOrTotalADCs() ;
  fIsClusterMaxOrTotalADCs = rhs.GetClusterMaxOrTotalADCs() ;
  fAPVGainCalibrationFile  = rhs.GetAPVGainCalibrationFile() ;
  return *this;
}

//============================================================================================
void SRSConfiguration::Init(const char * file) {
  if(!file) {
    std::cout <<"SRSConfiguration: Init conf file not specified. Setting defaults."<<std::endl;
    this->SetDefaults();
  }
  else {
    if(!Load(file)) {
      std::cout <<"SRSConfiguration: Init Cannot open conf file. Setting defaults."<<std::endl;
      this->SetDefaults();
    }
  }
}

//============================================================================================
void SRSConfiguration::SetDefaults() {
  fRunType                 = "RAWDATA" ;
  fRunName                 = "data" ;
  fCycleWait               = "1" ;
  fZeroSupCut              = "3" ;
  fMaskedChannelCut        = "20" ;
  fMaxClusterSize          = "20" ;
  fMinClusterSize          = "0" ;
  fMappingFile             = "../../configFileDir/mapping.cfg";
  fPadMappingFile          = "../../configFileDir/padMapping.cfg";
  fSavedMappingFile        = "../../results/savedMapping.cfg";
  fRunNbFile               = "../../configFileDir/runNb.cfg";
  fHistosFile              = "../../configFileDir/histogram.cfg";
  fDisplayFile             = "../../configFileDir/display.cfg" ;
  fPedestalFile            = "../../pedestalDir/pedestal.root";
  fRawPedestalFile         = "../../pedestalDir/rawpedestal.root";
  fPositionCorrectionFile  = "../../pedestalDir/clusterPositionCorrection.root";
  fPositionCorrectionFlag  = "NO" ;
  fTrackingOffsetDir      = "../../offsetDir/";
  fROOTDataType            = "HITS_ONLY" ;
  fAPVGainCalibrationFile  = "" ;
  fMaxClusterMultiplicity  = "1" ;
  fIsHitMaxOrTotalADCs     = "signalPeak" ;
  fIsClusterMaxOrTotalADCs = "TotalCharges" ;
  fStartEventNumber        = "0" ;
  fEventFrequencyNumber    = "1" ;
}

//============================================================================================
void SRSConfiguration::Save(const char * filename) const {
  std::cout << "saving conf in " << filename << std::endl;

  std::ofstream file (filename);
  file << "RUNNAME "              << fRunName                << std::endl;
  file << "RUNTYPE "              << fRunType                << std::endl;
  file << "CYCLEWAIT "            << fCycleWait              << std::endl;
  file << "ZEROSUPCUT "           << fZeroSupCut             << std::endl;
  file << "CHMASKCUT "            << fMaskedChannelCut       << std::endl;
  file << "MINCLUSTSIZE "         << fMinClusterSize         << std::endl;
  file << "MAXCLUSTSIZE "         << fMaxClusterSize         << std::endl;
  file << "MAXCLUSTMULT "         << fMaxClusterMultiplicity << std::endl;
  file << "MAPFILE "              << fMappingFile            << std::endl;
  file << "PADMAPFILE "           << fPadMappingFile         << std::endl;
  file << "SAVEDMAPFILE "         << fSavedMappingFile       << std::endl;
  file << "RUNNBFILE "            << fRunNbFile              << std::endl;
  file << "HISTCFG "              << fHistosFile             << std::endl;
  file << "DISPCFG "              << fDisplayFile            << std::endl;
  file << "OFFSETDIR "            << fTrackingOffsetDir      << std::endl;
  file << "ROOTDATATYPE "         << fROOTDataType           << std::endl;
  file << "PEDFILE "              << fPedestalFile           << std::endl;
  file << "RAWPEDFILE "           << fRawPedestalFile        << std::endl;
  file << "CLUSTERPOSCORRFILE "   << fPositionCorrectionFile << std::endl;
  file << "CLUSTERPOSCORRFLAG "   << fPositionCorrectionFlag << std::endl;
  file << "STARTEVENTNUMBER "     << fStartEventNumber       << std::endl;
  file << "EVENTFREQUENCYNUMBER " << fEventFrequencyNumber   << std::endl;
  file << "APVGAINCALIB "         << fAPVGainCalibrationFile << std::endl;
  file << "HIT_ADCS "             << fIsHitMaxOrTotalADCs << std::endl;
  file << "CLUSTER_ADCS "         << fIsClusterMaxOrTotalADCs << std::endl;
  file.close();
}

//============================================================================================
bool SRSConfiguration::Load(const char * filename) {
  std::cout<<"  SRSConfiguration::Load() ==> Loading cfg from "<<filename<<std::endl;
  std::ifstream file;
  if (this->FileExists(filename)) {
    try {
      file.open(filename, std::ifstream::in);
    } catch (std::ifstream::failure e) {
      std::cerr << filename
		<< ": File does not exist or cannot be opened!\n";
    } catch (...) {
      std::cerr << "Non-processed exception!\n";
    }
    
    if (!file.is_open()) {
      return false;
    }
  } else {
    return false;
  }
  
  char raw[1000];
  std::string line;
  do {
    file.getline(raw,1000);
    line = std::string(raw);
    if (line.find("#") < 10) continue;
    
    if(line.find("RUNTYPE")!=line.npos) {
      char hfile[1000];
      sscanf(line.c_str(), "RUNTYPE %s", hfile);
      fRunType = std::string(hfile);
    }
    if(line.find("RUNNAME")!=line.npos) {
      char hfile[1000];
      sscanf(line.c_str(), "RUNNAME %s", hfile);
      fRunName = hfile;
    }
    if(line.find("ZEROSUPCUT")!=line.npos) {
      char hfile[10];
      sscanf(line.c_str(), "ZEROSUPCUT %s", hfile);
      fZeroSupCut = hfile;
    }
    if(line.find("MAXCLUSTSIZE")!=line.npos) {
      char hfile[10];
      sscanf(line.c_str(), "MAXCLUSTSIZE %s", hfile);
      fMaxClusterSize = hfile;
    }
    if(line.find("MINCLUSTSIZE")!=line.npos) {
      char hfile[10];
      sscanf(line.c_str(), "MINCLUSTSIZE %s", hfile);
      fMinClusterSize = hfile;
    }
    if(line.find("MAXCLUSTMULT")!=line.npos) {
      char hfile[100];
      sscanf(line.c_str(), "MAXCLUSTMULT %s", hfile);
      fMaxClusterMultiplicity = hfile;
    }
    if(line.find("CYCLEWAIT")!=line.npos) {
      char hfile[100];
      sscanf(line.c_str(), "CYCLEWAIT %s", hfile);
      fCycleWait = hfile;
    }
    if(line.find("STARTEVENTNUMBER")!=line.npos) {
      char hfile[100];
      sscanf(line.c_str(), "STARTEVENTNUMBER %s", hfile);
      fStartEventNumber = hfile;
    }
    if(line.find("EVENTFREQUENCYNUMBER")!=line.npos) {
      char hfile[1000];
      sscanf(line.c_str(), "EVENTFREQUENCYNUMBER %s", hfile);
      fEventFrequencyNumber = hfile;
    }
    if(line.find("HISTCFG")!=line.npos) {
      char hfile[1000];
      sscanf(line.c_str(), "HISTCFG %s", hfile);
      fHistosFile = hfile;
    }
    if(line.find("PEDFILE")!=line.npos) {  
      char pedfile[1000];  
      sscanf(line.c_str(), "PEDFILE %s", pedfile);  
      fPedestalFile = pedfile; 
    }
    if(line.find("RAWPEDFILE")!=line.npos) {  
      char rawpedfile[1000];  
      sscanf(line.c_str(), "RAWPEDFILE %s", rawpedfile);  
      fRawPedestalFile = rawpedfile; 
    }
    if(line.find("OFFSETDIR")!=line.npos) {  
      char offsetfile[1000];  
      sscanf(line.c_str(), "OFFSETDIR %s", offsetfile);  
      fTrackingOffsetDir = offsetfile; 
    }
    if(line.find("CLUSTERPOSCORRFILE")!=line.npos) {  
      char posCorrectfile[1000];  
      sscanf(line.c_str(), "CLUSTERPOSCORRFILE %s", posCorrectfile);  
      fPositionCorrectionFile = posCorrectfile; 
    }
    if(line.find("CLUSTERPOSCORRFLAG")!=line.npos) {  
      char posCorrectflag[10];  
      sscanf(line.c_str(), "CLUSTERPOSCORRFLAG %s", posCorrectflag);  
      fPositionCorrectionFlag = posCorrectflag; 
    }
    if(line.find("APVGAINCALIB")!=line.npos) {  
      char apvGainCalibrationFile[1000];  
      sscanf(line.c_str(), "APVGAINCALIB %s", apvGainCalibrationFile);  
      fAPVGainCalibrationFile = apvGainCalibrationFile; 
    }
    if(line.find("MAPFILE")!=line.npos) {
      char mapfile[1000];
      sscanf(line.c_str(), "MAPFILE %s", mapfile);
      fMappingFile = mapfile;
    }
    if(line.find("PADMAPFILE")!=line.npos) {
      char padmapfile[1000];
      sscanf(line.c_str(), "PADMAPFILE %s", padmapfile);
      fPadMappingFile = padmapfile;
    }
    if(line.find("SAVEDMAPFILE")!=line.npos) {
      char savedmapfile[1000];
      sscanf(line.c_str(), "SAVEDMAPFILE %s", savedmapfile);
      fSavedMappingFile = savedmapfile;
    }
    if(line.find("RUNNBFILE")!=line.npos) {
      char runNbfile[1000];
      sscanf(line.c_str(), "RUNNBFILE %s", runNbfile);
      fRunNbFile = runNbfile;
    }
    if(line.find("DISPCFG")!=line.npos) {
      char displayfile[1000];
      sscanf(line.c_str(), "DISPCFG %s", displayfile);
      fDisplayFile = displayfile;
    }
    if(line.find("CHMASKCUT")!=line.npos) {
      char maskchannels[1000];
      sscanf(line.c_str(), "CHMASKCUT %s", maskchannels);
      fMaskedChannelCut = maskchannels;
    }
    if(line.find("ROOTDATATYPE")!=line.npos) {
      char rootdatatype[1000];
      sscanf(line.c_str(), "ROOTDATATYPE %s", rootdatatype);
      fROOTDataType = rootdatatype;
    }
    if(line.find("CLUSTER_ADCS")!=line.npos) {
      char isClusterMaxOrTotalADCs[1000];
      sscanf(line.c_str(), " CLUSTER_ADCS%s", isClusterMaxOrTotalADCs);
      fIsClusterMaxOrTotalADCs = isClusterMaxOrTotalADCs ;
    }
    if(line.find("HIT_ADCS")!=line.npos) {
      char isHitMaxOrTotalADCs[1000];
      sscanf(line.c_str(), " HIT_ADCS%s", isHitMaxOrTotalADCs);
      fIsHitMaxOrTotalADCs = isHitMaxOrTotalADCs ;
    }
  }while (!file.eof());
  this->Dump();
  return true;
}

void SRSConfiguration::Dump() const {
  printf("  SRSConfiguration::Load() ==> RUNTYPE               %s\n", fRunType.c_str()) ;
  printf("  SRSConfiguration::Load() ==> RUNNAME               %s\n", fRunName.c_str()) ;
  printf("  SRSConfiguration::Load() ==> CYCLEWAIT             %s\n", fCycleWait.c_str()) ;
  printf("  SRSConfiguration::Load() ==> ZEROSUPCUT            %s\n", fZeroSupCut.c_str()) ;
  printf("  SRSConfiguration::Load() ==> MINCLUSTSIZE          %s\n", fMinClusterSize.c_str()) ;
  printf("  SRSConfiguration::Load() ==> MAXCLUSTSIZE          %s\n", fMaxClusterSize.c_str()) ;
  printf("  SRSConfiguration::Load() ==> MAXCLUSTMULT          %s\n", fMaxClusterMultiplicity.c_str()) ;
  printf("  SRSConfiguration::Load() ==> HISTCFG               %s\n", fHistosFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> DISPCFG               %s\n", fDisplayFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> MAPFILE               %s\n", fMappingFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> PADMAPFILE            %s\n", fPadMappingFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> SAVEDMAPFILE          %s\n", fSavedMappingFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> PEDFILE               %s\n", fPedestalFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> RAWPEDFILE            %s\n", fRawPedestalFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTERPOSCORRFILE    %s\n", fPositionCorrectionFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTERPOSCORRFLAG    %s\n", fPositionCorrectionFlag.c_str()) ;
  printf("  SRSConfiguration::Load() ==> HIT_ADCS              %s\n", fIsHitMaxOrTotalADCs.c_str()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTER_ADCS          %s\n", fIsClusterMaxOrTotalADCs.c_str()) ;
  printf("  SRSConfiguration::Load() ==> STARTEVENTNUMBER      %s\n", fStartEventNumber.c_str()) ;
  printf("  SRSConfiguration::Load() ==> EVENTFREQUENCYNUMBER  %s\n", fEventFrequencyNumber.c_str()) ;
  printf("  SRSConfiguration::Load() ==> APVGAINCALIB          %s\n", fAPVGainCalibrationFile.c_str()) ;
  printf("  SRSConfiguration::Load() ==> ROOTDATATYPE          %s\n", fROOTDataType.c_str()) ;
  printf("  SRSConfiguration::Load() ==> OFFSETDIR             %s\n", fTrackingOffsetDir.c_str()) ;
}
