# dma

I've never used DMA before so wrote [this simple program](https://github.com/piersfinlayson/pico-samples/tree/main/dma) for a Pico (based heavily on [pico-examples/dma/hello_dma.c](https://github.com/raspberrypi/pico-examples/blob/master/dma/hello_dma/hello_dma.c)) to test the speed of a DMA transfer, how much work can be done during that transfer, and compare that to a memcpy of the same data.
* First it does a DMA transfer of a chunk of memory, 4 bytes at a time.
  * As this is DMA, the CPU is free during the transfer, so the CPU does some work (counts up).
* Then it does the same size memcpy, but this time doesn't have time to do any counting.

## Build

From the root of this repo

```
cmake .
make -j 4 dma
```

## Install

With your pico attached in BOOTSEL mode (as device sdx):

```
./flash-pico.sh sdx dma/dma.uf2
```

## Test

Connect a serial terminal, for example:

```
minicom -o -D /dev/ttyACM0
```

You should see output like the following:

```
DMA: DMA transfer of 16384 32-bit values took 132us, CPU counted to: 2048
DMA: memcpy of 65536 bytes took 479us, CPU didn't have time to count
```

I make 132us to be 16,500 clock cycles (at 125MHz) which makes sense - the DMA transfer will take a minimum of 16,384 clock cycles, plus some overhead (not least because in dma_channel_configure some stuff is done before the DMA transfer is started.

During 16,500 clock cycles, the CPU counts to 2048.  That's 8 clock cycles per count.  The assembler the compiler is generating for this counting while loop is, I think:

```
1000037e:       2201            movs    r2, #1
10000380:       4694            mov     ip, r2
10000382:       681a            ldr     r2, [r3, #0]
10000384:       44e1            add     r9, ip
10000386:       420a            tst     r2, r1
10000388:       d1f9            bne.n   1000037e <main+0x76>
```

I make:

* MOVS 1 cycle
* MOV 1 cycle
* LDR 2 cycles
* ADD 1 cycle
* TST 1 cycle
* BNE.n 2 cycles (as branch taken)

So 8 cycles