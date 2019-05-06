/**
 * float to ascii transform
 * 
*/
char *ftoa(char *buffer, double d, int precision);
/**
 * float to ascii transform assuming 8 byte buffer and 1 point of precission
 * 
*/
char *ftoa8_1(char *buffer, double d);
/**
 * pads input string using padding character to length in output buffer
 * */
char *pad(char *input, char *output, int length, char pad_char);