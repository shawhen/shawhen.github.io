#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#include <time.h>
#include <stdlib.h>

/*
 * @return 0 for little, 1 for big 
 */
int endian() {
  static const uint32_t t = 0x11000022;
  return (*((uint8_t*)&t)) == 0x11;
}

/*
 * @return 0 for error
 */
int32_t ipv4toint32(const char* ipv4) {
  int32_t result_i32 = 0;
  int big_endian = endian();
  uint8_t* result_u8p = ((uint8_t*)&result_i32)+(big_endian?0:3);

  int segment_cursor = 0;
  int ipv4_len = strlen(ipv4);
  for (int cursor = 0; cursor <= ipv4_len; ++ cursor) {
    if (!isdigit(ipv4[cursor])) {
      if (ipv4[cursor] == ' ') {
	if (isdigit(ipv4[cursor-1]) && isdigit(ipv4[cursor+1])) { // spaces can't locate between digits
	  fprintf(stderr, "spaces between digits is illegal\r\n");
	  return 0;
	} else if (cursor == 0 || cursor == ipv4_len) {
	  ;
	} 
      } else if (ipv4[cursor] == '.' || ipv4[cursor] == '\0') { // segment checkpoint
	uint8_t segment = 0;
	int space_inside = ipv4[cursor-1] == ' ' ? 1 : 0; // 0 or 1
	for (; segment_cursor < cursor; segment_cursor ++) {
	  segment += (ipv4[segment_cursor] == ' ') ? 0 : (ipv4[segment_cursor]-48/*'0'*/)*pow(10, (cursor-segment_cursor-1-space_inside));
	}
	(*result_u8p) = segment;
	big_endian ? result_u8p ++ : result_u8p --;
	
	segment_cursor += 1;
      } else {
	fprintf(stderr, "character %c(@%d) should not be here\r\n", ipv4[cursor], cursor);
	return 0;
      }
    }
  }

  return result_i32;
}

void test1();
void test2();
int tests() {
  int big_endian = endian();

  fprintf(stdout, "*-tests begin\r\n");
  fprintf(stdout, "endian: %s\r\n", big_endian ? "big" : "little");
  
  test1();
  test2();

  fprintf(stdout, "*-tests end\r\n");
  
  return 0;
}

int main(int argc, char* argv[])
{
  srand(time(NULL));
  
  tests();
  
  char ipv4[512] = {0}; // 512 should be enough even for a mussy ipv4 address
  fprintf(stdout, "\r\nnow, input your ipv4 address: ");
  char* result_s = fgets(ipv4, sizeof(ipv4), stdin);
  if (result_s == NULL) {
    fprintf(stderr, "something wrong!\r\n");
    return -1;
  }
  ipv4[strlen(ipv4)-1] = '\0';
  int32_t result_i32 = ipv4toint32(ipv4);
  if (result_i32 != 0) {
    fprintf(stdout, "ipv4 address(%s) converted to int32(%d)\r\n", ipv4, result_i32);
  }

  return 0;
}

void test1() {
  fprintf(stdout, "*--test1 begin----\r\n");
  const char* ipv4s[] = {"172.168.5.1", "172 . 168.5.1", "1 72.168.5.1"};
  for (int cursor = 0; cursor < sizeof(ipv4s)/sizeof(char*); ++ cursor) {
    const char* ipv4 = ipv4s[cursor];
    fprintf(stdout, "converting ipv4: %s\r\n", ipv4);
    int32_t result = ipv4toint32(ipv4);
    if (result != 0) {
      fprintf(stdout, "ipv4(%15s) -> %d/%d %d %d %d\r\n", ipv4, result, *((uint8_t*)&result), *((uint8_t*)(&result)+1), *((uint8_t*)(&result)+2), *((uint8_t*)(&result)+3));
    }
  }
  fprintf(stdout, "*--test1 end----\r\n");
}

void test2() {
  fprintf(stdout, "*--test2 begin----\r\n");
  
  for (int cursor = 0; cursor < 5; cursor ++) {
    uint8_t b_1 = (uint8_t)rand();
    uint8_t b_2 = (uint8_t)rand();
    uint8_t b_3 = (uint8_t)rand();
    uint8_t b_4 = (uint8_t)rand();
    char ipv4[512] = {0};
    sprintf(ipv4, "%d.%d.%d.%d", b_1, b_2, b_3, b_4);
    fprintf(stdout, "converting ipv4: %s\r\n", ipv4);
    int32_t result = ipv4toint32(ipv4);
    if (result != 0) {
      fprintf(stdout, "ipv4(%15s) -> %d/%d %d %d %d\r\n", ipv4, result, *((uint8_t*)&result), *((uint8_t*)(&result)+1), *((uint8_t*)(&result)+2), *((uint8_t*)(&result)+3));
    }
  }

  fprintf(stdout, "*--test2 end----\r\n");
}
