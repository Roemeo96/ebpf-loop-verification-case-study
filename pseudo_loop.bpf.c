// pseudo_loop.bpf.c
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int pseudo_loop(struct xdp_md *ctx)
{
    volatile unsigned long long sink = 0;

    for (unsigned long long i = 0; i < 10000000; i++) {
        sink += i;
    }

    return XDP_PASS;
}

char LICENSE[] SEC("license") = "GPL";
