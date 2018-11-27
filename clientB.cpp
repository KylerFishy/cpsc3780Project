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

int main(int argc, int argv[])
{

  try{
      ClientSocket client_socket ( "localhost", 30000 );

      std::string message = readFile("messageFile");
      std::string reply;
      try {
	        //client_socket << encode(message);
          unsigned int MESSAGE_SIZE = 7 * 71; // 7 * 71 = 497
          unsigned int frameSentCounter = 0;
          std::string encodedMessage = encode(message, false);
          for (int i = 0; i < encodedMessage.size(); i+= MESSAGE_SIZE) {
            //client_socket << encodedMessage.substr(i, MESSAGE_SIZE
            do {
              frameSentCounter++;
              std::string currentFrame = encodedMessage.substr(i, MESSAGE_SIZE);

              if (frameSentCounter % 5 == 0) { // send wrong parity every 5 frames
                if (parityIsOdd(currentFrame)) { currentFrame += "0"; } //WRONG
                else { currentFrame += "1"; } //WRONG
              }
              else{
                if (parityIsOdd(currentFrame)) { currentFrame += "1"; } //CORRECT
                else { currentFrame += "0"; } //CORRECT
              }

              client_socket << currentFrame;
              std::cout << "Frame sent." << std::endl;
              client_socket >> reply;
              if (reply == "ACK") {std::cout << "Received ACK.\n";}
              else if (reply == "NACK") {std::cout << "Received NACK, retransmitting.\n";}

              usleep(100);
            } while (reply == "NACK");
            std::cout << std::endl;
          }

      }
      catch (SocketException&){
      }


  }
  catch ( SocketException& e ){
      std::cout << "Exception was caught:" << e.description() << "\n";
  }



  return 0;
}
