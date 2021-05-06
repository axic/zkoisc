/*
00000021 00000005 00000020 ffffffff 00000009 00000001 00000004 00000020
00000000 00000055 00000073 00000061 00000067 00000065 0000003a 00000020
00000075 0000006e 00000069 0000006c 00000065 00000071 00000020 00000066
00000069 0000006c 00000065 0000002e 00000075 0000006e 0000006c 0000000a
ffffffff
*/

/*mem = [0x21, 0x05, 0x20, 0xffffffff, 0x09, 0x01, 0x04, 0x20, 0x00, 0x55, 0x73,
0x61, 0x67, 0x65, 0x3a, 0x20,]
let mem: u32[]
let pc: u32
hile true:
let a =
mem[pc] let b = mem[pc + 1] let c = mem[pc + 2] if a == 0xffffffff: # syscall,
args are in b and c if c == 0: stop() elif c == 1: putchar(mem[b]) elif c == 2:
            mem[b] = getchar()
        else:
            panic()
    else:
        if mem[a] <= mem[b]:
            pc = c
        else:
            pc = pc + 3
        mem[a] = mem[a] - mem[b]
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

//uint32_t code[] = {0x21, 0x05, 0x20, 0xffffffff, 0x09,      0x01, 0x04,
//                   0x20, 0x00, 0x55, 0x73,       0x61,      0x67, 0x65,
//                   0x3a, 0x20, 0x75, 0x6e,       0x69,      0x6c, 0x65,
//                   0x71, 0x20, 0x66, 0x69,       0x6c,      0x65, 0x2e,
//                   0x75, 0x6e, 0x6c, 0x0a,       0xffffffff};
uint32_t code[] = {
//0x00000021, 
//0x00000005, 0x00000020, 0xffffffff, 0x00000009, 0x00000001, 0x00000004, 0x00000020, 0x00000000, 
//0x00000055, 0x00000073, 0x00000061, 0x00000067, 0x00000065, 0x0000003a, 0x00000020, 0x00000075, 
//0x0000006e, 0x00000069, 0x0000006c, 0x00000065, 0x00000071, 0x00000020, 0x00000066, 0x00000069, 
//0x0000006c, 0x00000065, 0x0000002e, 0x00000075, 0x0000006e, 0x0000006c, 0x0000000a, 0xffffffff, 
//0x00000018,

/*28, 
5, 27, 0xffffffff, 9, 1, 4, 27, 0, 
72, 101, 108, 108, 111, 32, 102, 114, 
111, 109, 32, 122, 107, 79, 73, 83, 
67, 33, 0xffffffff, 19, 0, 0, 0,
*/

0x00000017, 
0x00000005, 0x00000016, 0xffffffffffffffff, 0x00000009, 0x00000001, 0x00000004, 0x00000016, 0x00000000, 
0x00000048, 0x00000065, 0x0000006c, 0x0000006c, 0x0000006f, 0x00000020, 0x0000007a, 0x0000006b, 
0x0000004f, 0x00000049, 0x00000053, 0x00000043, 0x00000021, 0xffffffffffffffff, 0x0000000e, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

uint32_t mem[128] = {
    0,
};

uint32_t highest_read = 0;

uint32_t readmem(uint32_t addr) {
//printf("Read %llu\n", addr);
  //assert(addr < (sizeof(mem)/sizeof(mem[0])));
  if (addr > highest_read)
	highest_read = addr;
  return (addr > (sizeof(mem)/sizeof(mem[0]))) ? 0 : mem[addr];
}

void writemem(uint32_t addr, uint32_t val) {
  assert(addr < (sizeof(mem)/sizeof(mem[0])));
  mem[addr] = val;
}

/*
void unlrun(unlstate* st,u32 iters) {
        //Run unileq for a given number of iterations. If iters=-1, run forever.
        u32 dec=iters!=(u32)-1;
        u64 a,b,c,ma,mb,ip=st->ip;
        for (;iters && st->state==UNL_RUNNING;iters-=dec) {
                //Load a, b, and c.
                a=unlgetmem(st,ip++);
                b=unlgetmem(st,ip++);
                c=unlgetmem(st,ip++);
                //Execute a normal unileq instruction.
                mb=unlgetmem(st,b);
                if (a!=(u64)-1) {
                        ma=unlgetmem(st,a);
                        unlsetmem(st,a,ma-mb);
                        ip=ma>mb?ip:c;
                }
                //Otherwise, call the interpreter.
                else if (c==0) {
                        //Exit.
                        st->state=UNL_COMPLETE;
                } else if (c==1) {
                        //Write mem[b] to stdout.
                        putchar((char)mb);
                } else if (c==2) {
                        //Read stdin to mem[b].
                        unlsetmem(st,b,(uchar)getchar());
                }
        }
        st->ip=ip;
}
*/

void dumpmem() {
        for (unsigned pos = 0; pos < sizeof(mem)/sizeof(mem[0]); pos++) {
                printf("0x%08llx, ", mem[pos]);
                if ((pos % 8) == 0)
                        printf("\n");
        }
}


int main(int argc, char *argv[]) {
  // preload code
  int codesize = sizeof(code)/sizeof(code[0]);
  printf("codesize=%u memsize=%lu\n", codesize, sizeof(mem)/sizeof(mem[0]));
  assert(sizeof(mem) >= codesize);
  memcpy(mem, code, sizeof(uint32_t)*codesize);
  
//  dumpmem();

  uint32_t pc = 0;
  int running = 1;
  int steps = 0;
  while (running) {
  steps++;
    uint32_t a = readmem(pc);
    uint32_t b = readmem(pc + 1);
    uint32_t c = readmem(pc + 2);
    printf("a=%llx b=%llx c=%llx mem[a]=%llx mem[b]=%llx\n", a, b, c, readmem(a), readmem(b));
    if (a == 0xffffffff) {
      pc += 3;
//      printf("intcall %x %x\n", b, c);
      switch (c) {
      case 0:
        running = 0;
        break;
      case 1:
        putchar(readmem(b));
        break;
      case 2:
        writemem(b, getchar());
        break;
      }
    } else {
      if (readmem(a) <= readmem(b))
        pc = c;
      else
        pc += 3;
      writemem(a, readmem(a) - readmem(b));
    }
  }
  printf("highest_read=%llu stepcount=%d\n", highest_read, steps);
}

/*
// store 42 at mem[0] and stop
42 42 0 42

*/
