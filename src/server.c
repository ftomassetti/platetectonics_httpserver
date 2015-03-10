#define H2O_USE_LIBUV 1

#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "h2o.h"
#include "h2o/http1.h"
#include "h2o/http2.h"
#include "wrap_pt.h"
#include "map_drawing.hpp"

static h2o_globalconf_t config;
static h2o_context_t ctx;
static SSL_CTX *ssl_ctx;

static void register_handler(h2o_hostconf_t *hostconf, const char *path, int (*on_req)(h2o_handler_t *, h2o_req_t *))
{
    h2o_pathconf_t *pathconf = h2o_config_register_path(hostconf, path);
    h2o_handler_t *handler = h2o_create_handler(pathconf, sizeof(*handler));
    handler->on_req = on_req;
}

static int chunked_test(h2o_handler_t *self, h2o_req_t *req)
{
    static h2o_generator_t generator = {NULL, NULL};

    if (!h2o_memis(req->method.base, req->method.len, H2O_STRLIT("GET")))
        return -1;

    long seed = 1;
    size_t width = 128;
    size_t height = 128;
    float sea_level = 0.65;
    size_t erosion_period = 60;
    float folding_ratio = 0.02;
    float aggr_overlap_abs = 1000000;
    float aggr_overlap_rel = 0.33;
    size_t cycle_count = 2; 
    size_t num_plates = 10;

    printf("Creating map with seed %li\n", seed);
    void* p = platec_api_create(seed, width, height, sea_level, 
        erosion_period, folding_ratio,
        aggr_overlap_abs, aggr_overlap_rel,
        cycle_count, num_plates);
    float* heightmap = platec_api_get_heightmap(p);
    int res = writeImage("tmp.png", width, height, heightmap, "A-super-cool-map");

    h2o_iovec_t body = h2o_strdup(&req->pool, "hello world\n", SIZE_MAX);
    req->res.status = 200;
    req->res.reason = "OK";
    h2o_add_header(&req->pool, &req->res.headers, H2O_TOKEN_CONTENT_TYPE, H2O_STRLIT("text/plain"));
    h2o_start_response(req, &generator);
    h2o_send(req, &body, 1, 1);

    return 0;
}

static void on_accept(uv_stream_t *listener, int status)
{
    uv_tcp_t *conn;
    h2o_socket_t *sock;

    if (status != 0)
        return;

    conn = h2o_mem_alloc(sizeof(*conn));
    uv_tcp_init(listener->loop, conn);

    if (uv_accept(listener, (uv_stream_t *)conn) != 0) {
        uv_close((uv_handle_t *)conn, (uv_close_cb)free);
        return;
    }

    sock = h2o_uv_socket_create((uv_stream_t *)conn, NULL, 0, (uv_close_cb)free);
    if (ssl_ctx != NULL)
        h2o_accept_ssl(&ctx, ctx.globalconf->hosts, sock, ssl_ctx);
    else
        h2o_http1_accept(&ctx, ctx.globalconf->hosts, sock);
}

static int create_listener(void)
{
    static uv_tcp_t listener;
    struct sockaddr_in addr;
    int r;

    uv_tcp_init(ctx.loop, &listener);
    uv_ip4_addr("127.0.0.1", 7890, &addr);
    if ((r = uv_tcp_bind(&listener, (struct sockaddr *)&addr, 0)) != 0) {
        fprintf(stderr, "uv_tcp_bind:%s\n", uv_strerror(r));
        goto Error;
    }
    if ((r = uv_listen((uv_stream_t *)&listener, 128, on_accept)) != 0) {
        fprintf(stderr, "uv_listen:%s\n", uv_strerror(r));
        goto Error;
    }

    return 0;
Error:
    uv_close((uv_handle_t *)&listener, NULL);
    return r;
}

int main(int argc, char* argv[])
{
	h2o_hostconf_t *hostconf;

    signal(SIGPIPE, SIG_IGN);

    h2o_config_init(&config);
    hostconf = h2o_config_register_host(&config, "default");
    register_handler(hostconf, "/map", chunked_test);


#if H2O_USE_LIBUV
    uv_loop_t loop;
    uv_loop_init(&loop);
    h2o_context_init(&ctx, &loop, &config);
#else
    h2o_context_init(&ctx, h2o_evloop_create(), &config);
#endif

    if (create_listener() != 0) {
        fprintf(stderr, "failed to listen to 127.0.0.1:7890:%s\n", strerror(errno));
        goto Error;
    }

#if H2O_USE_LIBUV
    uv_run(ctx.loop, UV_RUN_DEFAULT);
#else
    while (h2o_evloop_run(ctx.loop) == 0)
        ;
#endif

Error:
    return 1;    
}
