#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <limits>


bool parityIsOdd (std::string msg) {
  unsigned int numEven = 0;

  for (int i = 0; i < msg.size(); i++) {
    if (int(msg[i]) == 49) {
      numEven++;
    }
  }

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


//////////////////////////////////////////////////////////////////////////////
int main(int argc, int argv[])
{
  bool correctFileName = false;

  // select file
  try {
    ClientSocket client_socket ("localhost", 30000);
    std::string reply;
    std::string fileName;
    std::cout << "Enter file name (text1, text2): ";
    std::cin >> fileName;

    ////////////////////////////////////////////////
    client_socket << fileName; // SEND FILENAME
    std::cout << "(1) SENDING FILENAME" << std::endl;
    /////////////////////////////////////////////////


    /////////////////////////////////////////////
    client_socket >> reply; // RECIEVE reply
    std::cout << "(2) RECIEVING REPLY" << std::endl;
    std::cout << "reply: " << reply << std::endl;
    //////////////////////////////////////////////////
    if (reply == "File found") {
      correctFileName = true;
      std::cout << "set correct filename to true" << std::endl;
    }

  }
  catch (SocketException& e) {
      std::cout << "Exception was caught:" << e.description() << "\n";
  }

  // get file
  if (correctFileName) {
    try {

        ClientSocket client_socket ("Network", 30000);
        std::string reply;
        std::string message;

        client_socket << "ready";

        client_socket >> reply;
        std::cout << "reply" << reply << std::endl;

        client_socket << "ACK";
        std::cout << "sent ACK" << reply << std::endl;
    }
    catch (SocketException& e) {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }
  }
  else {
    std::cout << "File does not exist on server, exiting..." << std::endl;
  }



  return 0;
}
