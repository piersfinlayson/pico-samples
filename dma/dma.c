#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"

#define COUNT 4*4096
uint32_t src[COUNT];
uint32_t dst[COUNT];

int main() {
    uint64_t time1, time2, time3;
    int ticks;
    dma_channel_config config;

    stdio_init_all();

    // Initialize src data to something
    memset(src, 0, COUNT*sizeof(src[0]));

    while (1)
    {
        // Get a free DMA channel - this function will panic if there are none
        // free
        int chan = dma_claim_unused_channel(true);

        // Do 32-bit transfers, as these are quicker than 8-bit (as 32-bits
        // can be copied in 1 cycle, as opposed to 8)
        config = dma_channel_get_default_config(chan);
        channel_config_set_transfer_data_size(&config, DMA_SIZE_32);
        channel_config_set_read_increment(&config, true);
        channel_config_set_write_increment(&config, true);

        // We'll use ticks for the CPU to count while the DMA transfer takes place
        ticks=0;
        time1=time_us_64();

        // Start the DMA transfer
        dma_channel_configure(chan,
                              &config,
                              dst,
                              src,
                              COUNT,
                              true);

        // While the DMA transfer is taking place, count up
        while (dma_channel_is_busy(chan))
        {
            ticks++;
        }
        time2=time_us_64();

        // Cleanup and free the DMA channel (we should be allocated channel 0
        // again the next time around as nothing else will be allocating DMA
        // channels).
        dma_channel_cleanup(chan);
        dma_channel_unclaim(chan);

        // Print out DMA results
        time3=time2-time1;
        printf("DMA: DMA transfer of %d 32-bit values took %lluus, CPU counted to: %d\n", COUNT, time3, ticks);

        // Now do the equivalent memcpy
        time1=time_us_64();
        memcpy(dst, src, COUNT*sizeof(src[0]));
        time2=time_us_64();
        
        // Print out memcpy results
        time3=time2-time1;
        printf("DMA: memcpy of %d bytes took %lluus, CPU didn't have time to count\n", COUNT*sizeof(src[0]), time3);

        // Pause and then do it again
        sleep_ms(1000);
    }
}
