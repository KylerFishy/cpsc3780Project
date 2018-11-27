#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
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

std::string decode(std::string msg) {
  std::string ASCIIMsg = "";
  for (int i = 0; i < msg.size(); i+=7) {
    if (msg.substr(i, 7).size() >= 7)
      ASCIIMsg += char(std::stoi(msg.substr(i, 7), nullptr, 2));
  }
  return ASCIIMsg;
}

unsigned int PORT_NUMBER = 30000;

int main(int argc, int argv[])
{
  try{
      // Create the socket
      ServerSocket server (PORT_NUMBER);
      std::cout << "Started Server on Port " << PORT_NUMBER << std::endl;

      while (true){
          // See the discussions above.
	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try{
	      while ( true ){
		        std::string data;
		        new_sock >> data;
            std::cout << "\nReceived frame. ";
            if (!parityIsOdd(data)) { std::cout << "Status: OK." << std::endl; }
            else { std::cout << "Status: ERROR DETECTED." << std::endl; }


            if (parityIsOdd(data) == false) {
              new_sock << "ACK";
              std::cout << "Sending back ACK. Contents:\n'";
              std::cout << decode(data) << "'" <<std::endl;
             }
            else { new_sock << "NACK";
            std::cout << "<< Sending back NAK. >>\n";
            }

	      }
	  }
	  catch(SocketException&){
          }

	  }
  }
  catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }

  return 0;
}
