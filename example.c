/*
  What does it take to render something on the screen?
  1 - Filling the image buffer
  2 - Sending the buffer to the screen
  Here, we'll mainly focus on step 1, but if we have time, we'll
  also take a look at step 2.

  Goal: render a triangle and send it to the std output, following a
  .ppm format style.
*/

void write(int, char*, int);

int main (void) {
  /* Step 1: render an uniform image. */
  write(1, "hi world!!\n", 11);
  return 0;
}
