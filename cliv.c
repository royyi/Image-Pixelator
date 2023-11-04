/* Roy Yi Meiqing Wang
 * 11/4/23
 * Reduce pgm format (black and white image) to unicode blocks
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

#define BLOCK_H 8
#define BLOCK_W 4

// print the unicode character based on brightness value
void print_char(int value) {
  assert(value >= 0 && value <= 255);

  if (value >= 0 && value <= 50)          // black
    printf("%lc", L'\u2588');
  else if (value >= 51 && value <= 101)   // dark
    printf("%lc", L'\u2593');
  else if (value >= 102 && value <= 152)  // medium
    printf("%lc", L'\u2592');
  else if (value >= 153 && value <= 203)  // light
    printf("%lc", L'\u2591');
  else                                    // white
    printf("%lc", L' ');
}

// return the average value for a block size 4x8 pixels
int reduce_size(unsigned char* buffer, int start, int width, int height) {
  int val = 0;

  for (int r = 0; r < BLOCK_H; r++) 
    for (int c = 0; c < BLOCK_W; c++) {
      assert(start + r*width + c < width*height);
      val += buffer[start + r*width + c];
    }

  return val / (BLOCK_H * BLOCK_W);
}

int main(int argc, char* argv[]) 
{
  if (argc != 2) {
    fprintf(stderr, "Usage: ./cliv <file.pgm>\n"); 
    exit(1);
  }

  FILE *fin = fopen(argv[1], "r");

  if (!fin) {
    fprintf(stderr, "Error: cannot open file %s", argv[1]);
    exit(1);
  }

  char ignore[5];
  int width;
  int height;

  fgets(ignore, sizeof(ignore), fin);

  fscanf(fin, "%8i", &width);
  fscanf(fin, "%8i", &height);

  fgets(ignore, sizeof(ignore), fin);

  int count = width * height;

  unsigned char* buffer = malloc(sizeof(unsigned char)*count);

  fread(buffer, sizeof(unsigned char), count, fin);

  // configure $LC_ALL for unicode 
  setlocale(LC_ALL, "en_US.UTF-8");

  // print out average value for each block
  for (int r = 0; r < height - BLOCK_H; r += BLOCK_H) {
    for (int c = 0; c < width - BLOCK_W; c += BLOCK_W) {
      print_char(reduce_size(buffer, r*width + c, width, height));
    }
    printf("\n");
  }

  fclose(fin);
  free(buffer);

  return 0;
}
