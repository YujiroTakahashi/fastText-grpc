#include <iostream>
#include <grpcpp/grpcpp.h>

#include "config.h"
#include "di.h"
#include "service.h"

/**
 * help message
 *
 * @package     exchanger
 * @param  int  argc
 * @param  char **argv
 * @return int
 */
static void syntax(void)
{
    std::cout
        << "Usage: ftext-server [options]\n"
        << "    -c, --config PATH   Config file path\n"
        << "    -h, --help          This Help.\n"
        << std::endl;
    exit(1);
}

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
    if (argc < 2) {
        syntax();
    }

    croco::config& cfg = croco::Di<croco::config>::get();
    for (int pos=1; pos < argc; pos++) {
        if (!strcmp(argv[pos], "-c") || !strcmp(argv[pos], "--config")) {
            cfg.load(argv[++pos]);
        } else {
            syntax();
        }
    }

    if (!cfg.loaded()) {
        syntax();
    }

    std::string server_address(cfg.get("address"));
    croco::fTextServiceImpl service;

    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    gpr_log(GPR_INFO, "Server listening on  %s.", server_address.c_str());

    server->Wait();

    return 0;
}
