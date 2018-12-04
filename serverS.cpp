#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <limits>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <random>
#include <pthread.h>
#include <vector>


bool parityIsOdd (std::string msg) {
  unsigned int numEven = 0;

  for (int i = 0; i < msg.size(); i++) {
    if (int(msg[i]) == 49) {
      numEven++;
    }
  }

  // if number of  bits is odd:
  if (numEven % 2) {
    return true;
    }
  else { return false; }
}

std::string encode(std::string msg, bool causeError) {
  std::string binaryMsg = "";
  for (int i = 0; i < msg.size(); i++) {
    std::string binary = std::bitset<7>(int(msg[i])).to_string();
    binaryMsg += binary;
  }
  return binaryMsg;
}

std::string decode(std::string msg) {
  std::string ASCIIMsg = "";
  for (int i = 0; i < msg.size(); i+=7) {
    if (msg.substr(i, 7).size() >= 7)
      ASCIIMsg += char(std::stoi(msg.substr(i, 7), nullptr, 2));
  }
  return ASCIIMsg;
}

std::string readFile(std::string fileName) {
  std::string output;
  std::string line;
  std::string fullFileName = fileName + ".txt";
  std::ifstream myfile (fullFileName);

  if (myfile.is_open()) {
    while (getline (myfile,line)) {
      output += line;
    }
    myfile.close();
  }
  else std::cout << "Unable to open file\n";

  return output;
}


///////////////////////////////////////////////////////////////////////////////
int main(int argc, int argv[])
{
  try {
      // Read the file
      std::string messageFile = "messageFile";
      std::string message = readFile(messageFile);

      // Make vector of text file names
      static std::vector<std::string> fileNames;
      fileNames = {"text1", "text2"};

      // Create the socket
      ServerSocket server (30000);
      std::cout << "Server started on port " << 30000 << "\n\n";

      while (true){
          std::string data;
          std::string reply;
          ServerSocket sock;
	        server.accept(sock);

          /////////////////////////////////////
          sock >> reply; // RECIEVE FILENAME
          std::cout << "(1) RECEIVE FILENAME: " << reply << std::endl;
          /////////////////////////////////////

          usleep(1000);
          bool correctFileName = false;

          for (int i = 0; i < fileNames.size(); i++){
            if (fileNames[i].find(data, 0) != std::string::npos) {


              /////////////////////////////////
              sock << "File found"; // SEND FOUND
              std::cout << "(2) SEND FOUND" << std::endl;
              /////////////////////////////////

              std::cout << "File located, attempting to send file...";
              correctFileName = true;
            }
          }

          if (!correctFileName) {

            //////////////////////////////////
            sock << "File not found"; // SEND NOT found
            std::cout << "(3) SEND NOT FOUND" << std::endl;
            /////////////////////////////////

            std::cout << "File not found...";
          }


          if (correctFileName) {
      	     try {

                  sock >> reply;
                  std::cout << "received ready" << std::endl;

      	          while (true) {
      		        std::string data;
                  std::string reply;

                  std::cout << "sending 0101";

                  ////////////////////////////////////////////
      		        sock << "0101"; // SEND message
                  std::cout << "(4) SEND MSG" << std::endl;
                  ///////////////////////////////////////////


                  ///////////////////////////////////////
                  sock >> reply; // REC REPLY
                  ///////////////////////////////////////
                  std::cout << reply << std::endl;
                  usleep(100);
      	       }
      	     }
      	     catch(SocketException&){
            }
          }

	    }
    }
  catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
  return 0;
}
