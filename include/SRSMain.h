#ifndef _SRSMAIN_
#define _SRSMAIN_

//C++ Includes
#include <string>
#include <memory>

//Framework Includes

//ROOT Includes

class SRSMapping;
class SRSConfiguration;
class SRSOutputROOT;
class SRSMain{
public:
    SRSMain(const std::string& rawfile, const std::string& config);
    ~SRSMain();
    //static SRSMain* Reprocessor(const std::string& rawfile, const std::string& config);
    void Reprocess();
    void Close();

private:
    //SRSMain(const std::string& rawfile, const std::string& config);
    void Init();
private:
    //static SRSMain* _repro;
    //SRSConfiguration* _conf;
    //SRSMapping* _maps;
    //SRSOutputROOT* _root;

    std::unique_ptr<SRSConfiguration> _conf;
    //std::unique_ptr<SRSMapping> _maps;
    std::shared_ptr<SRSMapping> _maps;
    std::unique_ptr<SRSOutputROOT> _root;

private:
    std::string _rawfile;
    std::string _config;
    bool _isClosed;
    
};
#endif
