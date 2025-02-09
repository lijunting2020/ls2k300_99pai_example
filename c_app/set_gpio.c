#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

#define GPIO_MUX_2_ADDR 0x16000498
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

int gpio_mux (off_t target, unsigned long writeval)
{
    int fd;
    void *map_base, *virt_addr; 

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) 
    {
        fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__, __FILE__, errno, strerror(errno)); 
        return -1;
    }

    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1)
    {
        fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__, __FILE__, errno, strerror(errno)); 
        return -1;
    }

    virt_addr = map_base + (target & MAP_MASK);
    *((unsigned long *) virt_addr) = writeval;
    return -1;
}

int main(int argc, char **argv) 
{
    if(argc < 2)
    {
        fprintf(stderr, "\nUsage:\t%s { op_type }\n"
			"\top_type    : 1.main; 0.gpio\n\n",
			argv[0]);
		exit(1);
    }

    if(strtoul(argv[1], 0, 10) == 1)
    {
        gpio_mux(GPIO_MUX_2_ADDR, 0xFFFFFFFF);
    }
    else
    {
        gpio_mux(GPIO_MUX_2_ADDR, 0xFF00FFFF);
    }
    return 0;
}
