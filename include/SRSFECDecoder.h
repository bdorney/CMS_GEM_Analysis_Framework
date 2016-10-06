#include <vector>
#include <map>
class SRSEventBuilder;
class SRSFECDecoder{
 public:
  SRSFECDecoder(SRSEventBuilder* eB);
  void decodeFEC(unsigned int nw, unsigned long* buffer);
  //  void BuildHits(std::vector<unsigned long int> data32bits, int fec_no, int fec_channel);
  void BuildHits(std::vector<unsigned long int> data32bits, int fec_no, int fec_channel, SRSEventBuilder * eventBuilder) ;
 private:
  SRSEventBuilder* feventBuilder;
  int fPacketSize;
  std::vector <int>  fActiveFecChannels ;
  std::map<int, std::vector<int> > fActiveFecChannelsMap ;
};
