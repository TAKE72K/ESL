# ESLHW1 DMA

by E24063058

1. the function of circuite was following the SPEC.
2. It's a 32bit DMA, in a little endian structure.
3. to control this DMA, use base address 0x2000, SOURECE(0x0), TARGET(0x4), SIZE(0x8), START/CLEAR(0xC). Note that SIZE is size in bytes. 
4. To compile the source code,
```
make
```
if there's some errors, it's probobly result from incorrect PATH, after correction it should work well.
5. Test on ubuntu, for detailed ifo, please contact me at hyes941073@gmail.com
