#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
 
void readDataFromSerial(int totalByteCount, int bytesPerChunk, int fd, char *buf)
{
  int bytesRead = 0;
  while (bytesRead < totalByteCount) {
    int n = read(fd, &(buf[bytesRead]), bytesPerChunk);
    if (n <= 0) {
      continue;
    }
    bytesRead += n;
  }
  buf[totalByteCount] = 0;
}

int main(int argc, char ** argv) {
  int fd;
  char buf[15000];
  int n;
  int bytes_read = 0;
  // Open the Port. We want read/write, no "controlling tty" status, 
  // and open it no matter what state DCD is in
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);  
  //open mbed's USB virtual com port
  if (fd == -1) {  
    perror("open_port: Unable to open /dev/ttyACM0 - ");
    return(-1);
  }
  // Turn off blocking for reads, use (fd, F_SETFL, FNDELAY) if you want that
  fcntl(fd, F_SETFL, 0);
  //Linux Raw serial setup options
  struct termios options; 
  tcgetattr(fd,&options);   //Get current serial settings in structure
  cfsetspeed(&options, B9600);   //Change a only few
  options.c_cflag &= ~CSTOPB;
  options.c_cflag |= CLOCAL;
  options.c_cflag |= CREAD;
  cfmakeraw(&options);
  tcsetattr(fd,TCSANOW,&options);    //Set serial to new settings
  sleep(5);
  // Write to the port
  n = write(fd,"j",1);
  if (n < 0) {
    perror("Write failed - ");
    return -1;
  }
  // Read the characters from the port if they are there
  sleep(1);
  /*
  n = read(fd, buf, 4000);
  if (n < 0) {
    perror("Read failed - ");
    return -1;
  } else if (n == 0) printf("No data on port\n");
  else {
    bytes_read += n;
    buf[n] = 0;
    printf("%i bytes read back: %s\n\r", n, buf);
  }
  sleep(5);
    // Write to the port
  n = write(fd,"j",1);
  if (n < 0) {
    perror("Write failed - ");
    return -1;
  }
  // Read the characters from the port if they are there
  sleep(5);
  n = read(fd, buf, 14409);
  if (n < 0) {
    perror("Read failed - ");
    return -1;
  } else if (n == 0) printf("No data on port\n");
  else {
    buf[n] = 0;
    printf("%i bytes read back: %s\n\r", n, buf);
  }
  sleep(1);
  */
  readDataFromSerial(14409, 4000, fd, buf);
  printf("%s\n\r", buf);

  // Write to the port
  n = write(fd,"j",1);
  if (n < 0) {
    perror("Write failed - ");
    return -1;
  }

  sleep(1);

  readDataFromSerial(14409, 4000, fd, buf);
  printf("%s\n\r", buf);

  // Don't forget to clean up and close the port
  tcdrain(fd);
  close(fd);
  return 0;
}

