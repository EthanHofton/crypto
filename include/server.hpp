#ifndef server_hpp
#define server_hpp

#include <sockets.hpp>

namespace net
{
    class server
    {
    public:

        // server constructor
        server(int t_port, std::string keys = "")
        {
            // get rsa keys
            if (keys == "")
            {
                crypto::prng rng;
                m_keys = crypto::genKeys(128, rng);
                std::cout << m_keys.publicKey.n << std::endl;
            } else {
                crypto::loadKeys(keys);
            }

            m_port = t_port;
            int opt = 1;
            // create socket
            if ( (m_serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
            {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            // set socket port
            if (setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
            {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }

            // set adress port
            m_address.sin_family = AF_INET;
            m_address.sin_addr.s_addr = INADDR_ANY;
            m_address.sin_port = htons(t_port);

            // bind socket to adress struct
            if (bind(m_serverFd, (struct sockaddr *)&m_address, sizeof(m_address)) < 0)
            {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
        }

        // wait and listen for a client to join the server
        // once client joins, init secure connection
        int listenClient()
        {
            // wait listen for a client on the server
            if (listen(m_serverFd, 3) < 0)
            {
                perror("listen");
                exit(EXIT_FAILURE);
            }

            // accept client once found
            int addrLen = sizeof(m_address);
            int clientFd;
            if ((clientFd = accept(m_serverFd, (struct sockaddr *)&m_address, (socklen_t*)&addrLen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // establish secure connection
            // secure connection algorithm
            // 1: wait for client to request public key
            // 2: send client public key
            // 3: wait for client to send rsa encrypted seed
            // 4: decrypt seed
            // 5: create a prng using the seed for that client

            // create a responce buffer
            char *responce = new char[1];

            // wait and read client responce
            read(clientFd, responce, 1);

            // if the request was not for a public key, error
            if (responce[0] != REQUEST_PUBKEY)
            {
                perror("invalid handshake formation");
                exit(EXIT_FAILURE);
            }

            // send the publci key to the recever
            send(clientFd, &m_keys.publicKey, sizeof(crypto::rsaPublicKey), 0);
            std::cout << m_keys.publicKey.n << std::endl;

            // delete responce buffer
            delete[] responce;

            return clientFd;
        }

        void saveKeys(std::string filepath)
        {
            crypto::saveKeys(m_keys, filepath);
        }

    private:

        // port of server
        int m_port;

        // server socket
        int m_serverFd;

        // server address
        struct sockaddr_in m_address;

        // server rsa keys
        crypto::rsaKeys m_keys;
    };
}

#endif