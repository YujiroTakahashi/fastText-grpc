#include <iostream>
#include <grpcpp/grpcpp.h>
#include "client.h"

/**
 * Main
 *
 * @access public
 * @param int  argc
 * @param char **argv
 * @return int
 */
int main(int argc, char **argv)
{
    croco::fTextClient client(
        grpc::CreateChannel(
            "0.0.0.0:50051",
            grpc::InsecureChannelCredentials()
        )
    );

    //  client.LoadModel("wiki.ja.bin");
    client.LoadModel("wiki.ja.64.bin");
    uint32_t rows = client.WordRows();
    std::cout << rows << "\n";

    uint32_t id = client.WordID("明日");
    std::cout << id << "\n";


    return 0;
}