#ifndef __SRSCONFIGURATION__
#define __SRSCONFIGURATION__
/*******************************************************************************
*  based on AMORE FOR SRS - SRS                                                *
*  SRSConfiguration                                                            *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*  Modified: Marcello Maggi 09/09/2016  Root independent                       *
*******************************************************************************/
#include <fstream>
#include <iostream>

// Class containing all configuration parameters for configuration from file(s)
class SRSConfiguration {

 public:
  SRSConfiguration();
  SRSConfiguration(const char * file);
  ~SRSConfiguration();

  void Init(const char * file = 0);
  bool FileExists(const char* name) const;

  const char * GetRunType() const {return fRunType.c_str();}
  void  SetRunType(const char * name) {fRunType = std::string(name);}

  const char * GetRunName() const {return fRunName.c_str();}
  void  SetRunName(const char * name) {fRunName = std::string(name);}

  const char * GetZeroSupCut() const {return fZeroSupCut.c_str();}
  void  SetZeroSupCut(const char * name) {fZeroSupCut = std::string(name);}

  const char * GetMaskedChannelCut() const {return fMaskedChannelCut.c_str();}
  void  SetMaskedChannelCut(const char * name) {fMaskedChannelCut = std::string(name);}
    
  const char * GetStartEventNumber() const {return fStartEventNumber.c_str();}
  void  SetStartEventNumber(const char * name) {fStartEventNumber = std::string(name);}
 
  const char * GetEventFrequencyNumber() const {return fEventFrequencyNumber.c_str();}
  void  SetEventFrequencyNumber(const char * name) {fEventFrequencyNumber = std::string(name);}
  
  const char * GetMinClusterSize() const {return fMinClusterSize.c_str();}
  void  SetMinClusterSize(const char * name) {fMinClusterSize = std::string(name);}

  const char * GetMaxClusterSize() const {return fMaxClusterSize.c_str();}
  void  SetMaxClusterSize(const char * name) {fMaxClusterSize = std::string(name);}

  const char * GetMaxClusterMultiplicity() const {return fMaxClusterMultiplicity.c_str();}
  void  SetMaxClusterMultiplicity(const char * name) {fMaxClusterMultiplicity = std::string(name);}

  const char * GetMappingFile() const {return fMappingFile.c_str();}
  void  SetMappingFile(const char * name) {fMappingFile = std::string(name);}

  const char * GetPadMappingFile() const {return fPadMappingFile.c_str();}
  void  SetPadMappingFile(const char * name) {fPadMappingFile = std::string(name);}

  const char * GetSavedMappingFile() const {return fSavedMappingFile.c_str();}
  void  SetSavedMappingFile(const char * name) {fSavedMappingFile = std::string(name);}

  const char * GetRunNbFile() const {return fRunNbFile.c_str();}
  void  SetRunNbFile(const char * name) {fRunNbFile = std::string(name);}

  const char * GetHistoCfgName() const {return fHistosFile.c_str();}
  void  SetHistoCfgName(const char * name) {fHistosFile = std::string(name);}
 
  const char * GetDisplayCfgName() const {return fDisplayFile.c_str();}
  void  SetDisplayCfgName(const char * name) {fDisplayFile = std::string(name);}

  const char * GetROOTDataType() const {return fROOTDataType.c_str();}
  void  SetROOTDataType(const char * name) {fROOTDataType = std::string(name);}

  const char * GetPedestalFile() const {return fPedestalFile.c_str();}
  void  SetPedestalFile(const char * name) {fPedestalFile = std::string(name);}

  const char * GetTrackingOffsetDir() const {return fTrackingOffsetDir.c_str();}
  void  SetTrackingOffsetDir(const char * name) {fTrackingOffsetDir = std::string(name);}

  const char * GetRawPedestalFile() const {return fRawPedestalFile.c_str();}
  void  SetRawPedestalFile(const char * name) {fRawPedestalFile = std::string(name);}

  const char * GetClusterPositionCorrectionFile() const {return fPositionCorrectionFile.c_str();}
  void  setClusterPositionCorrectionFile(const char * name) {fPositionCorrectionFile = std::string(name);}

  const char * GetClusterPositionCorrectionFlag() const {return fPositionCorrectionFlag.c_str();}
  void  setClusterPositionCorrectionFlag(const char * name) {fPositionCorrectionFlag = std::string(name);}

  const char * GetAPVGainCalibrationFile() const {return fAPVGainCalibrationFile.c_str();}
  void  SetAPVGainCalibrationFile(const char * name) {fAPVGainCalibrationFile = std::string(name);}

  const char * GetClusterMaxOrTotalADCs() const {return fIsClusterMaxOrTotalADCs.c_str();};
  void  SetClusterMaxOrTotalADCs(const char * name) {fIsClusterMaxOrTotalADCs  = std::string(name);}

  const char * GetHitMaxOrTotalADCs() const {return fIsHitMaxOrTotalADCs.c_str();};
  void  SetHitMaxOrTotalADCs(const char * name) {fIsHitMaxOrTotalADCs  = std::string(name);}

  bool Load(const char * filename);
  void Save(const char * filename) const; 
  void Dump() const;

  SRSConfiguration & operator=(const SRSConfiguration &rhs);

 private:
  void BookArrays();
  void SetDefaults();

  std::string fMappingFile, fPadMappingFile, fSavedMappingFile, fRunNbFile, fRunName, fRunType, fROOTDataType, fCycleWait, fZeroSupCut, fMaskedChannelCut, fHistosFile, fTrackingOffsetDir;
  std::string fDisplayFile, fPositionCorrectionFile, fPositionCorrectionFlag, fPedestalFile, fRawPedestalFile, fAPVGainCalibrationFile;
  std::string fMaxClusterSize, fMinClusterSize, fStartEventNumber, fMaxClusterMultiplicity, fIsHitMaxOrTotalADCs, fIsClusterMaxOrTotalADCs, fEventFrequencyNumber;

};

#endif
