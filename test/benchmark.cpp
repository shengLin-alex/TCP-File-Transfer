#include <benchmark/benchmark.h>
#include "../src/server.h"
#include "../src/client.h"

static void BM_create_socket(benchmark::State &state) {
    for (auto _ : state) {
        int server_sfd = create_server_socket(8000);
        int client_sfd = create_client_socket(8000, "0.0.0.0");
        close(client_sfd);
        close(server_sfd);
    }
}

// Register the function as a benchmark
BENCHMARK(BM_create_socket);

BENCHMARK_MAIN();