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

void* serverThreads(void *sVec){

  std::cout << "Z";

  usleep(100);
  std::random_device rd;     // initialise random (seed) engine
  std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(1,5); // guaranteed unbiased

  std::string reply;

    std::vector <ServerSocket>test = *(std::vector<ServerSocket>*) sVec;


    std::cout << "A";
    ServerSocket recSock = test[0];
std::cout << "B";
    ServerSocket sendSock = test[1];
    std::string message = readFile("messageFile");

    try {
        unsigned int MESSAGE_SIZE = 7 * 71; // 7 * 71 = 497
        unsigned int frameSentCounter = 0;
        std::string encodedMessage = encode(message, false);
        for (int i = 0; i < encodedMessage.size(); i+= MESSAGE_SIZE) {
          do {
            frameSentCounter++;
            std::string currentFrame = encodedMessage.substr(i, MESSAGE_SIZE);
            int random_integer = uni(rng); //rand int 1-5

            if (random_integer % 5 == 0) { // send wrong parity every 5 frames // CHANGE TO RANDOM!!!!!
              if (parityIsOdd(currentFrame)) { currentFrame += "0"; } //WRONG
              else { currentFrame += "1"; } //WRONG
            }
            else{
              if (parityIsOdd(currentFrame)) { currentFrame += "1"; } //CORRECT
              else { currentFrame += "0"; } //CORRECT
            }

            sendSock << currentFrame;
            std::cout << "Frame sent." << std::endl;
            recSock >> reply;
            std::cout << "H";
            if (reply == "ACK") {std::cout << "Received ACK.\n";}
            else if (reply == "NACK") {std::cout << "Received NACK, retransmitting.\n";}

            usleep(100);
            std::cout << "Q";
          } while (reply == "NACK");
          std::cout << std::endl;
        }

    }
    catch (SocketException&){
    }

 pthread_exit(0);
}

unsigned int PORT_NUMBER_SEND = 30000;
unsigned int PORT_NUMBER_REC = 30001;

int main(int argc, int argv[])
{

  try{
      // Create the socket

      unsigned int threadCount = 0;
      pthread_t td[4];

      while (true){

          std::vector<ServerSocket> socketsVector;
          std::cout << "E";
          ServerSocket serverSend(30000);
          std::cout << "K";
          ServerSocket serverRec(30001);
          std::cout << "Y";
          ServerSocket sSend;
          ServerSocket sRec;

          std::cout << "F";
          serverSend.accept(sRec);
          serverRec.accept(sSend);
          std::string temp;
          sRec >> temp;
          sSend >> temp;
           std::string reply;
           std::cout << "G";
           socketsVector.push_back(sSend);
           std::cout << "Y";
           socketsVector.push_back(sRec);

           pthread_create(&td[threadCount], NULL, serverThreads, (void *) &socketsVector);
           threadCount++;
           if(&td[4] != NULL){
             pthread_join(td[0], NULL);
             pthread_join(td[1], NULL);
             pthread_join(td[2], NULL);
             pthread_join(td[3], NULL);
             pthread_join(td[4], NULL);
             threadCount = 0;
      }

    }
  }
  catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }

  return 0;
}
