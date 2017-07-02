#include <iostream>
#include <sstream>
#include <fstream>
#include "getopt.h"
#include "gspan.h"

#define OPT " [-m minsup] [-x maxpat] [-i] graph-file"

int main(int argc, char **argv) {
  unsigned int minsup = 0;
  unsigned int minp = 0;
  unsigned int maxpat = -1;
  bool out_instances = false;
  bool percent=false;
  int opt;
  int wildcard_num = 0;
  int k = 10;
  int i = 0;
  int maxGN = 100000;
  while ((opt = getopt(argc, argv, "m:p:w:x:k:i:g:j")) != -1) {
    switch (opt) {
    case 'm':
      minsup = atoi (optarg);
      break;
    case 'p':
      minp = atoi (optarg);
      percent = true;
      break;
    case 'x':
      maxpat = atoi (optarg);
      break;
    case 'w':
      wildcard_num = atoi (optarg);
      break;
    case 'k':
      k = atoi (optarg);
      break;
    case 'i':
      i = atoi (optarg);
      break;
    case 'j':
      out_instances = true;
      break;
    case 'g':
      maxGN = atoi (optarg);
      break;
    default:
      std::cerr << "Usage: "<< argv[0] << OPT<< std::endl;
      return -1;
    }
  }
	
  if(argc-optind != 1){
    std::cerr << "Usage: "<< argv[0] << OPT<< std::endl;
    return -1;
  }
	
  std::ifstream graph_file(argv[optind++]);
  if(graph_file.fail()){
    std::cerr << "File not found: " << argv[optind-1] << std::endl;
    return -1;
  }
  
  Gspan gspan;
  gspan.wildcard_r = wildcard_num;
  if(maxpat>0){
    gspan.maxpat = maxpat;
  }
  gspan.out_instances = out_instances;
  gspan.set_data(graph_file);
  gspan.minsup = minsup;
  if(percent==true){
    gspan.minsup = gspan.gdata.size() * minp /(100*k);
  }
  gspan.k = k;
  gspan.i = i;
  gspan.maxGN = maxGN;
  gspan.run();

   /*頻出グラフ番号をテキストに出力*/
  
  std::ofstream ofs("feature.csv");
  unsigned int look=0;
  unsigned int graph_count=0;
  for(map<int , vector<int> >::iterator it = gspan.freq.begin();it != gspan.freq.end();it++){
    while(graph_count < it->first){
      ofs<<"0";
      for(unsigned int i=1;i<gspan.p_count;i++){
	ofs << ",0";
      }
      ofs<<std::endl;
      graph_count++;
    }
    //std::cout << it->first <<" ";
    for(unsigned int i = 0;i < (it->second).size();i++){
      //std::cout << it->second[i] <<",";
      while(look < it->second[i]){
	if(look==0){
	  ofs<<"0";
	}else{
	  ofs << ",0";
	}
	  look++;
      }
      if(look==0){
	ofs<<"1";
      }else{
      ofs << ",1";
      }
      look++;
      }
    while(look<gspan.p_count){
      ofs << ",0";
      look++;
    }
    ofs << std::endl;
    //std::cout<<std::endl;
    graph_count++;
    
    look=0;
  }
  while(graph_count<gspan.gdata.size()){
    //std::cout<<graph_count<<std::endl;
    ofs<<"0";
    for(unsigned int i=1;i<gspan.p_count;i++){
      ofs << ",0";
    }
    ofs<<std::endl;
    graph_count++;
  }

  
  return 0;
}
