here we will note whenever we allocate some memory of our Heap

## RAM
- RAM stats at 0x2000_0000
- Stack grows fron 0x2000_4000 downward

=> Heap should grow from 0x2000_0000 upward

## Allocated Heap-Sections
### Boards
- 0x2000_0000 to 0x2000_0063 (100 Items) => Playerboard
- 0x2000_0064 to 0x2000_00C7 (100 Items) => Botboard
### User-Input
- 0x2000_00C8 to 0x2000_00DB (20 Items) => User_Input