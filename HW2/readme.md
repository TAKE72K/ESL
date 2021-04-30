# ESLHW2 TLM2.0 wrapped DMA

by E24063058

1. the function of circuit was following the SPEC.
2. It's a 32bit DMA, in a little endian structure.
3. to control this DMA, use base address 0x63000000, SOURECE(0x0), TARGET(0x4), SIZE(0x8), START/CLEAR(0xC). Note that SIZE is size in bytes.
4. Although it's not suggest, it's supported to transferred an odd size(in bytes) data 
5. To compile the source code, command
```
make
```
if there's some errors, it's probably result from incorrect PATH, after correction it should work well.

6. In the sc_main, CPU would make two query, first ask to move 4 words from 0x2020 to 0x20, after receive Interrupt signal, START/CLEAR would be written. CPU would then request to move 7 bytes.

7. Test on ubuntu, for detailed information, please contact me at hyes941073@gmail.com