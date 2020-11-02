// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <regex>
#include <cassert>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

void catfile (istream& infile, const string& filename, str_str_map& list) {
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   int line_cnt = 0;
   bool finished = false;
   for(;;) {

      string line;
      getline (infile, line);
      if (infile.eof()){
         finished = true;
      }
      
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         cout << filename << ": " << ++line_cnt << ": " << line << endl;
         
      }
      else if (regex_search (line, result, key_value_regex)) {
         cout << filename << ": " << ++line_cnt << ": " << line << endl;
         //cout <<  result[1] << " = " << result[2] << endl;
         //cout << "value: \"" << result[2] << "\"" << endl;
         if(result[1].compare("") != 0){
            if(result[2].compare("") != 0){
               cout <<  result[1] << " = " << result[2] << endl;
               str_str_pair pair_ (result[1],result[2]);
               list.insert(pair_);
            }
            else list.erase(list.find(result[1]));
         }
         else{
            if(result[2].compare("") == 0){
               for (str_str_map::iterator itor = list.begin(); itor != list.end(); ++itor) {
                  cout <<  itor->first << " = " << itor->second << endl;
               }
               
            }
            else{
               for (str_str_map::iterator itor = list.begin(); itor != list.end(); ++itor) {
                  if(itor->second.compare(result[2]) == 0) 
                  cout <<  itor->first << " = " << itor->second << endl;
               }
            }
         }
         
      
      }
      else if (regex_search (line, result, trimmed_regex)) {
         //cout << "query: \"" << result[1] << "\"" << endl;
         cout << filename << " :" << ++line_cnt << " " << line << endl;

         bool found = false;
         for (str_str_map::iterator itor = list.begin(); itor != list.end(); ++itor) {
                   
                  if(itor->first.compare(result[1]) == 0){ 
                     found = true;
                     cout <<  result[1] << " = " << itor->second << endl;
                  }
         }
         if(!found){
            cout << result[1] << ": key not found" << endl;
         }
      }
      
      else {
         assert (false and "This can not happen.");
      }
      
      if (finished){
         break;
      }
   }
}


int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   
   const string cin_name = "-";
   
   str_str_map map;
   bool afile = false;
   int status = 0;
   
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      
      afile = true;
      if(*argp == cin_name){
         catfile(cin,*argp,map);
      }
      else{
         ifstream infile (*argp);
         if(infile.fail()){
            status = 1;
            cout << *argp << strerror(errno) << endl;
         }
         else{
            catfile(infile,*argp,map);
            infile.close();
         }
      }
       
    //  str_str_pair pair (*argp, to_string<int> (argp - argv));
    //  cout << "Before insert: " << pair << endl;
    //  test.insert (pair); 
   }
   
   if(!afile){
      catfile(cin,"-",map);
      }

   /* for (str_str_map::iterator itor = map.begin();
      itor != map.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = map.begin();
   map.erase (itor);
   */





   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

