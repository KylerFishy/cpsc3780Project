#include "ServerSocket.h"
#include "SocketException.h"
#include "ClientSocket.h"
#include <string>
#include <thread>



void serverThreads(ServerSocket sSock){
    try{
        while (true){
            std::string data;
            sSock>> data;
            sSock<< data;
        }
    }
    catch(SocketException&){
    }

}


int main()

{


    try{
        ServerSocket server(30000);



        while(true){
            ServerSocket new_sock;
            server.accept(new_sock);

            std::thread th1(serverThreads,(new_sock), 1);
            std::thread th2(serverThreads,(new_sock), 2);
            std::thread th3(serverThreads,(new_sock), 3);
            std::thread th4(serverThreads,(new_sock), 4);
            std::thread th5(serverThreads,(new_sock), 5);

            th1.join();
            th2.join();
            th3.join();
            th4.join();
            th5.join();
        }
    }
    catch (SocketException& e){
        std::cout <<"exception was caught:" << e.description() << "\nExiting. \n";
    }
    return 0;

}
