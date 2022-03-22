#ifndef client_hpp
#define client_hpp

#include <sockets.hpp>

namespace net
{
    // client class
    class client
    {
    public:
    
        // client constructor
        // init socket and bind to server
        // init secure connection
        client(int port, std::string ip = "127.0.0.1")
        {
            m_port = port;
            m_ipAddress = ip;

            // create a server socket
            if ((m_serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror("client socket error");
                exit(EXIT_FAILURE);
            }

            // set server address
            m_serverAddr.sin_family = AF_INET;
	        m_serverAddr.sin_port = htons(m_port);

            // Convert IPv4 and IPv6 addresses from text to binary form
            if(inet_pton(AF_INET, ip.c_str(), &m_serverAddr.sin_addr) <= 0)
            {
                perror("Invalid address/ Address not supported");
                exit(EXIT_FAILURE);
            }

            // connect to server
            if (connect(m_serverFd, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr)) < 0)
            {
                perror("Connection Failed");
                exit(EXIT_FAILURE);
            }

            // establish secure connection
            // secure connection algorithm
            // 1: request server public key
            // 2: encrypt a randomly generated seed using the servers public key
            // 3: send the server the randomly generated key
            // 4: create a random number generator using the seed 

            // create a request byte buffer
            char *request = new char[1];

            // set the request to be a public key request
            request[0] = REQUEST_PUBKEY;

            // send public key request
            send(m_serverFd, request, 1, 0);

            // wait for server to respond
            crypto::rsaPublicKey serverPublicKey;
            read(m_serverFd, &serverPublicKey, sizeof(crypto::rsaPublicKey));
            std::cout << serverPublicKey.n << std::endl;

            // delete request byte buffer
            delete[] request;
        }

    private:

        // store ip address
        std::string m_ipAddress;

        // server address
        struct sockaddr_in m_serverAddr;

        // server port and socket
        int m_port;
        int m_serverFd;

        crypto::prng clientRNG;
    };

}

#endif