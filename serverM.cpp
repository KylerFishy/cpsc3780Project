#include "ServerSocket.h"
#include "SocketException.h"
#include "ClientSocket.h"
#include <string>
#include <pthread.h>



void* serverThreads(void *sSock){

    ServerSocket tempSock = *(ServerSocket *) sSock;

    try{
        while (true){
            std::string data;
            tempSock >> data;
            tempSock << data;
        }
    }
    catch(SocketException&){
    }
 pthread_exit(0);
}


int main(int arg)

{
int threadNumb = 5;


        ServerSocket server(30000);


            ServerSocket new_sock;
            server.accept(new_sock);

        pthread_t td[threadNumb];

        for (int i = 0; i < threadNumb; i++) {

            pthread_create(&td[i], NULL, serverThreads, (void *) &new_sock);
            pthread_join(td[i], NULL);
        }


    return 0;

}
