#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int infinite_loop(struct xdp_md *ctx)
{
    volatile unsigned long long sink = 0;

    for (;;) {
        sink += 1;
    }

    return XDP_PASS;
}

char LICENSE[] SEC("license") = "GPL";
