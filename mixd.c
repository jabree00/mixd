#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <err.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(X,Y) ((X) < (Y)? (X) : (Y))
typedef uint8_t u8;

//-- Options ----------------
struct offset_range { off_t start, end; };

size_t option_columns      = 16;
size_t option_groupsize    = 8;
bool option_use_formatting = true;
bool option_collapse_repetition = true;
struct offset_range option_range  = { 0, -1 };

const char *formatting_zero      = "38;5;238";
const char *formatting_all       = "38;5;167";
const char *formatting_low       = "38;5;150";
const char *formatting_high      = "38;5;141";
const char *formatting_printable = "";

//-- Hexdump impl -----------
const char *format_of(int v) {
  return v == 0x00?  formatting_zero
       : v == 0xFF?  formatting_all
       : v <  0x20?  formatting_low
       : v >= 0x7F?  formatting_high
       :             formatting_printable;
}

const char *CHAR_AREA_HIGH_LUT[] = {
  "€", ".", "‚", "ƒ", "„", "…", "†", "‡", "ˆ", "‰", "Š", "‹", "Œ", ".", "Ž", ".",
  ".", "‘", "’", "“", "”", "•", "–", "—", "˜", "™", "š", "›", "œ", ".", "ž", "Ÿ",
  ".", "¡", "¢", "£", "¤", "¥", "¦", "§", "¨", "©", "ª", "«", "¬", ".", "®", "¯",
  "°", "±", "²", "³", "´", "µ", "¶", "·", "¸", "¹", "º", "»", "¼", "½", "¾", "¿",
  "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï",
  "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "×", "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "Þ", "ß",
  "à", "á", "â", "ã", "ä", "å", "æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï",
  "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "÷", "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ",
};

void comparator(FILE *f, const char *filename, FILE *f2, const char *filename2) {
  
  u8 line[option_columns]; 
  u8 line2[option_columns]; 

  // Seek to start for file; fall back to a consuming loop for non-seekable files
  if (fseeko(f, option_range.start, SEEK_SET) < 0) {
    off_t remaining = option_range.start;
    while (remaining != 0 && fgetc(f) != EOF) remaining--;
    if (ferror(f)) err(1, "(while seeking) %s", filename);
  }
  printf("File 1 is ready for processing.\n"); 
  
  // Seek to start for file #2; fall back to a consuming loop for non-seekable files
  if (fseeko(f2, option_range.start, SEEK_SET) < 0) {
    off_t remaining2 = option_range.start;
    while (remaining2 != 0 && fgetc(f2) != EOF) remaining2--;
    if (ferror(f2)) err(1, "(while seeking) %s", filename2);
  }
  
  printf("File 2 is ready for processing.\n"); 
  
  //Add some whitespace to console 
  printf("\n");
  
  //Starting at the user-provided start value keep going until the user-provided end point or EOF?
  for (off_t offset = option_range.start; offset < option_range.end || option_range.end == -1; offset += option_columns) {
    off_t read = offset - option_range.start;
    size_t n; 
    size_t num_bytes_read_f = fread(line, 1, option_columns, f);
    size_t num_bytes_read_f2 = fread(line2, 1, option_columns, f2);
    
    //No data read, move on to next set of bytes
    if (num_bytes_read_f == 0 && num_bytes_read_f2 == 0) {
      break;
    }
    
    //Choose the smaller of the two sets of bytes 
    if (num_bytes_read_f2 < num_bytes_read_f){
      n = num_bytes_read_f2; 
    } else {
      n = num_bytes_read_f;   
    }
  

    // Offset
    intmax_t offset = option_range.start + read;
    printf("%5jx%03jx", offset >> 12, offset & 0xFFF);

    // Print hex area
    const char *prev_fmt = NULL;
    for (size_t j = 0; j < option_columns; j++) {
    
      //add some spacing?
      if (option_groupsize != 0 && j % option_groupsize == 0) {
        printf(" ");
      }
      
      if (j < n) {
  
        const char *fmt = format_of(line[j]);
        if (prev_fmt != fmt && option_use_formatting) {
          printf("\x1B[%sm", fmt);
        } 
        
        (line[j] == line2[j]) ? printf(" %02x", line[j]) : printf(" __"); 
        
        prev_fmt = fmt;
    
      } 
      
      //If not enough bytes read to fill requested number of columns
      //print blank space 
      else {
        printf("   ");
      }
    }
    putchar(' ');

    // Print char area
    for (size_t j = 0; j < option_columns; j++) {
      if (option_groupsize != 0 && j % option_groupsize == 0) {
        printf(" ");
      }
      
      if (j < n) {
        const char *fmt = format_of(line[j]);
        
        //Change the color if needed and user wants formatting
        if (prev_fmt != fmt && option_use_formatting) {
          printf("\x1B[%sm", fmt);
        }
        
        if (line[j] >= 0x80 && (line[j] == line2[j])) {
          printf("%s", CHAR_AREA_HIGH_LUT[line[j] - 0x80]);
        }
        else if (line[j] == line2[j]){
          putchar(isprint(line[j])? line[j] : '.');
        } else {
          putchar('.');
        }
        
        //Update previous format with current format in 
        //preparation for next iteration 
        prev_fmt = fmt;
      } 
      
      else {
        putchar(' ');
      }
    }
    printf("%s\n", option_use_formatting? "\x1B[m" : "");

  }

  if (ferror(f)) {
    err(1, "(while reading) %s", filename);
  }
  
  if (ferror(f2)) {
    err(1, "(while reading) %s", filename2);
  }
}

//-- Entry point ------------
/** Parses a range "start-end" (both ends optional) or "start+size" (neither
 *  optional) into a `struct offset_range` instance. */
struct offset_range parse_range(const char *str) {
  struct offset_range res = { 0, -1 };
  
  //first points to first character in string 
  //delim points to location of delimiter 
  //second points to location after delimiter 
  const char *first = str, *delim = str + strcspn(str, "+-"), *second = delim + 1;
  if (*delim == '\0') errx(1, "no delimiter in range %s", str);

  //handle invalid inputs
  char *end;
  if (first != delim) {
    res.start = strtoimax(first, &end, 0);
    if (!isdigit(*first) || end != delim) errx(1, "invalid start value in range %s", str);
  }
  if (*second != '\0') {
    res.end = strtoimax(second, &end, 0);
    if (!isdigit(*second) || *end != '\0') errx(1, "invalid end/size value in range %s", str);
  }

  if (*delim == '+') {
    if (first == delim) errx(1, "start unspecified in range %s", str);
    if (*second == '\0') errx(1, "size unspecified in range %s", str);
    res.end += res.start;
  }

  if (res.end < res.start && res.end != -1) errx(1, "end was less than start in range %s", str);
  return res;
}

int main(int argc, char *argv[]) {
  // Default to colourful output if output is a TTY
  option_use_formatting = isatty(1);

  // Update settings variables based on command-line arguments
  int opt;
  while (opt = getopt(argc, argv, "g:hpPr:w"), opt != -1) {
    switch (opt) {
      case 'g': option_groupsize = atol(optarg); break;
      case 'p': option_use_formatting = false; break;
      case 'P': option_use_formatting = true; break;
      case 'r': option_range = parse_range(optarg); break;
      case 'w': option_columns = atol(optarg); break; 
      case 'h': // fall through
      default:
        fprintf(stderr, "usage: hexd [-p] [-P] [-v] [-g groupsize] [-r range] [-w width]\n");
        return 1;
    }
  }
  
  //Subtract already-used arguments from the count  
  argc -= optind;
  
  //Move past already-used arguments in the array
  argv += optind;

  // Parse HEXD_COLORS
  char *colors_var = getenv("HEXD_COLORS");
  if (colors_var != NULL) {
    colors_var = strdup(colors_var);
    if (colors_var == NULL) errx(1, "strdup");

    for (char *p = colors_var, *token; token = strtok(p, " "), token != NULL; p = NULL) {
      char *key = token, *value = strchr(token, '=');
      if (value == NULL) warnx("no '=' found in HEXD_COLORS property '%s'", p);
      *value++ = '\0';

      if      (strcmp(key, "zero")      == 0) formatting_zero      = value;
      else if (strcmp(key, "low")       == 0) formatting_low       = value;
      else if (strcmp(key, "printable") == 0) formatting_printable = value;
      else if (strcmp(key, "high")      == 0) formatting_high      = value;
      else if (strcmp(key, "all")       == 0) formatting_all       = value;
      else warnx("unknown HEXD_COLORS property '%s'", key);
    }
  }

  if (argc != 2) {
    printf("Please supply two filepaths as input."); 
    return 0; 
  } 
  
  else {
    //Attempt to open each file 
    FILE *f = fopen(argv[0], "r");
    if (f == NULL) {
      warn("%s", argv[0]);
      return 0; 
    }
    
    FILE *f2 = fopen(argv[1], "r");
    if (f2 == NULL){
      warn("%s", argv[1]); 
      return 0; 
    }
    
    printf("Input have been validated. Beginning comparison...\n"); 
    comparator(f, argv[0], f2, argv[1]);
    
    //Signal that the used memory space is available for use again
    fclose(f);
    fclose(f2);
    free(colors_var);
    return 0;
  }
  
  
}
