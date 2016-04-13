#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

int main()
{
    WSADATA WsaDat;
    WSAStartup(MAKEWORD(2,2),&WsaDat);

    char *szServer = "localhost";
    int nPort = 5000;
    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    u_long iMode = 1;
    int iResult = ioctlsocket(clientSock, FIONBIO, &iMode);
    if (iResult != NO_ERROR) {
        printf("ioctlsocket failed with error: %ld\n", iResult);
    }

    struct hostent *host;
    if((host=gethostbyname(szServer))==NULL)
    {
        printf("Unable to resolve host name\n");
    }

    // Set up our socket address structure
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(nPort);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr=*((unsigned long*)host->h_addr);

    if(connect(clientSock, (LPSOCKADDR)(&SockAddr),sizeof(SockAddr)) == SOCKET_ERROR) {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            printf("Failed to connect to server on %i!\n", nPort);
            printf("Error: %i\n", WSAGetLastError());
            // cleanup
            return 1;
        }
    }

    while (1) {
        char szBuffer[1024];
        ZeroMemory(szBuffer,sizeof(szBuffer));

        strcpy(szBuffer, "Hello");

        int inDataLength = send(clientSock, szBuffer, strlen(szBuffer), 0);

        if(inDataLength!=-1)
        {
            printf("Sent: %s\n", szBuffer);
        }
        Sleep(100);
    }
    shutdown(clientSock, SD_BOTH);
    closesocket(clientSock);

    WSACleanup();
    return 0;
}
