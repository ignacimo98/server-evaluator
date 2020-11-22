#include "sobel.h"

void apply_filter(char* filename) {
  int filename_length = strlen(filename);
  //"octave-cli sobel.m " 19 chars
  char* command = malloc((20 + filename_length)*sizeof(char));

  sprintf(command, "octave-cli sobel.m %s", filename);

  system(command);

  free(command);
}