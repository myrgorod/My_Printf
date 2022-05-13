#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

int my_strlen(char* str);
int my_putaddr(long i);
char* append_char(char* str, char c);
int my_putbase(long ptr, int base);
int my_putstr(char* str);
int print_arg(char* str, va_list args);
int my_printf(char* format, ...);
int my_putchar(char c);
int my_putnbr(long num);

//int main() {
//  int i = 0;
//  my_putstr("-------\n");
//  i = my_printf("%p!\n", i);
//  my_putstr("-------\nSize: ");
//  my_putnbr(i);
//  my_putchar('\n');
//  return 0;
//}

int my_printf(char* format, ...) { 
  va_list args;
  int i = 0;
  int j;

  va_start(args, format);
  while (*format)
  {
    j = 0;
    if (*format != '%') {
      format += write(1, format, 1);
	  i++;
	}
    else {
      format += (j += print_arg(format, args)) == 0 ? 1 : 2;
      i += j;
	}
  }
  va_end(args);
  return i;
}

// essential a function to direct the argument to
// the right function to get processed.
int print_arg(char* str, va_list args) {
  if (str[1] == 'c')
    return my_putchar(va_arg(args, int));
  else if (str[1] == 's')
    return my_putstr(va_arg(args, char*));
  else if (str[1] == 'o')
    return my_putbase(va_arg(args, int), 8);
  else if (str[1] == 'p')
    return my_putaddr((long)va_arg(args, void *));
  else if (str[1] == 'u')
    return my_putnbr(va_arg(args, unsigned int));
  else if (str[1] == 'd')
    return my_putnbr(va_arg(args, int));
  else if (str[1] == 'x')
    return my_putbase(va_arg(args, int), 16);
  return 0; // this is a check in case the format string ended with a '%'.
}

int my_putaddr(long i) {
  my_putstr("0x");
  return 2 + my_putbase((long)&i, 16);
}

int my_putchar(char c) {
  write(1, &c, 1);
  return 1;
}

int my_putstr(char* str) {
  int len;
  if (!str)
  	return my_putstr("(null)");
  len = my_strlen(str);
  write(1, str, len);
  return len;
}

int my_strlen(char* str) {
  int i = 0;

  if (!str)
    return i;
  while (str[i])
    i++;
  return i;
}

// converts the input to long so it can be used on both ints
// and unsigned ints. Then prints the input in the given base.
int my_putbase(long ptr, int base) {
  int result = 0;
  char* hexstr;
  int len = 0;

  hexstr = (char*)malloc(1);
  *hexstr = '\0';
  while (ptr > 0) {
    result = ptr % base;
    if (result < 10)
      hexstr = append_char(hexstr, result + '0');
    else
      hexstr = append_char(hexstr, result - 10 + 'a');
    ptr /= base;
  }
  result = my_strlen(hexstr);
  while (hexstr[--result] == '0');
  while (result >= 0) {
    my_putchar(hexstr[result]);
    result--;
	len++;
  }
  free(hexstr);
  return len;
}

// function which reallocates memory to append a char.
char* append_char(char* str, char c) {
  char* temp;
  int   i = 0;

  temp = (char*)malloc(my_strlen(str) + 2);
  while (str[i]) {
    temp[i] = str[i];
    i++;
  }
  temp[i] = c;
  temp[i + 1] = '\0';
  free(str);
  return temp;
}

// prints a number, converts input to long so it can handle more types.
int my_putnbr(long num) {
  int len = 0;
  int temp;
  if (num < 0) {
    my_putchar('-');
    num *= -1;
	len++;
  }
  temp = num;
  if (num > 10)
    my_putnbr(num / 10);
  my_putchar(num % 10 + '0');
  if (temp == 0)
  	return 1;
  while (temp > 0) {
    len++;
    temp /= 10;
  }
  return len;
}
