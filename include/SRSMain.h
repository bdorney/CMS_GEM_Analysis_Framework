#ifndef _SRSMAIN_
#define _SRSMAIN_

#include <string>

class SRSMapping;
class SRSConfiguration;
class SRSOutputROOT;
class SRSMain{
 public:
  ~SRSMain();  
  static SRSMain* Reprocessor(const std::string& rawfile, const std::string& config);
  void Reprocess();
 private:
  SRSMain(const std::string& rawfile, const std::string& config);
  void Init();
  void Close();
 private:
  static SRSMain* _repro;
  SRSConfiguration* _conf;
  SRSMapping* _maps;
  SRSOutputROOT* _root;

 private:
  std::string _rawfile;
  std::string _config;

};
#endif
