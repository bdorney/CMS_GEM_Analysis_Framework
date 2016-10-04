#include <string>
#include <list>
#include <sstream>
#include <iostream>

std::list<std::string> tokenize(const std::string& line, const char * p){
  std::list<std::string> pzs;
  std::string pz="";
  std::string buf=line;
  do{
    std::cout << "Line = "<<buf<<std::endl;
    int s=buf.find(p);
    std::string cz;
    if (s == buf.npos) {
      cz = buf;
      buf="";
    } else {      
      cz = buf.substr(0,s);      
      buf=buf.substr(s+1,buf.npos);
    }
    std::stringstream os;
    std::cout <<" Found cz |"<<cz<<"|"<<std::endl;
    os << cz;
    os >> pz;
    pzs.push_back(pz);
    
  }while(buf.size()>0);
  return pzs;
}

int main(){
  std::string line("DET , Pippo1,   2,   Pippo3");
  std::list<std::string> wr = tokenize(line,",");
  for (std::list<std::string>::iterator i=wr.begin();i!=wr.end();i++){
    std::cout <<" "<<*i<<std::endl;
  }
}
