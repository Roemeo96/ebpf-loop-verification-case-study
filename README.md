# eBPF Loop Verification Experiments

Dieses Repository enthält reproduzierbare Experimente zur Untersuchung des Verhaltens des Linux-eBPF-Verifiers bei Schleifen.

Ziel ist die Analyse der Frage, wie der Verifier zwischen

- terminierenden Schleifen,
- sehr großen, aber terminierenden Schleifen (Pseudo-Endlosschleifen) und
- echten Endlosschleifen

unterscheidet.

## Inhalt

```text
.
├── pseudo_loop.bpf.c
├── infinite_loop.bpf.c
├── pseudo_loop.log
├── infinite_loop.log
└── README.md
```

### pseudo_loop.bpf.c

Enthält eine terminierende Schleife mit einer sehr großen Iterationszahl:

```c
for (unsigned long long i = 0; i < N; i++) {
    sink += i;
}
```

### infinite_loop.bpf.c

Enthält eine echte Endlosschleife:

```c
for (;;) {
    sink += 1;
}
```

## Voraussetzungen

Getestet unter Ubuntu Linux.

Benötigte Pakete:

```bash
sudo apt update

sudo apt install \
    clang \
    llvm \
    bpftool \
    libbpf-dev \
    linux-libc-dev \
    linux-headers-$(uname -r)
```

## Kompilieren

### Pseudo-Endlosschleife

```bash
clang -O2 -g -target bpf \
  -I/usr/include/x86_64-linux-gnu \
  -c pseudo_loop.bpf.c \
  -o pseudo_loop.bpf.o
```

### Endlosschleife

```bash
clang -O2 -g -target bpf \
  -I/usr/include/x86_64-linux-gnu \
  -c infinite_loop.bpf.c \
  -o infinite_loop.bpf.o
```

## Verifier-Logs reproduzieren

### Pseudo-Endlosschleife

```bash
sudo rm -f /sys/fs/bpf/test

sudo bpftool prog load \
  pseudo_loop.bpf.o \
  /sys/fs/bpf/test \
  2>&1 | tee pseudo_loop.log
```

### Endlosschleife

```bash
sudo rm -f /sys/fs/bpf/test

sudo bpftool prog load \
  infinite_loop.bpf.o \
  /sys/fs/bpf/test \
  2>&1 | tee infinite_loop.log
```

## Beobachtungen

Für die verwendete Kernel- und bpftool-Version wurden sowohl die Pseudo-Endlosschleife als auch die Endlosschleife mit einer Fehlermeldung der Form

```text
BPF program is too large.
Processed 1000001 insn
processed 1000001 insns (limit 1000000)
```

abgelehnt.

Die Ablehnung erfolgte dabei aufgrund eines überschrittenen Verifier-Limits für verarbeitete Instruktionen.

## Forschungsfrage

Die Experimente wurden durchgeführt, um die folgende Frage zu untersuchen:

> Unterscheidet der eBPF-Verifier explizit zwischen terminierenden und nicht terminierenden Schleifen oder werden beide Fälle letztlich durch dieselben Komplexitätsgrenzen der Verifikation begrenzt?

Die bereitgestellten Programme und Logs dienen als reproduzierbare Grundlage für weitere Untersuchungen.

## Disclaimer

Die Ergebnisse beziehen sich auf die konkret verwendete Kernel-, Clang-, libbpf- und bpftool-Version. Das Verhalten des Verifiers kann sich zwischen Kernelversionen unterscheiden.
```
