#include <stdio.h>
#include <string.h>

static int is_unknown_error_msg(int errnum, const char *msg) {
  char unknown[128];
  snprintf(unknown, sizeof(unknown), "Unknown error: %d", errnum);
  return strcmp(msg, unknown) == 0;
}

static void print_escaped_c_string(const char *s) {
  putchar('"');
  for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
    if (*p == '\\' || *p == '"') {
      putchar('\\');
      putchar(*p);
    } else if (*p == '\n') {
      fputs("\\n", stdout);
    } else if (*p == '\t') {
      fputs("\\t", stdout);
    } else if (*p >= 32 && *p <= 126) {
      putchar(*p);
    } else {
      printf("\\x%02X", *p);
    }
  }
  putchar('"');
}

int main(void) {
  const int scan_limit = 512;
  int max_known = -1;

  for (int i = 0; i <= scan_limit; ++i) {
    if (!is_unknown_error_msg(i, strerror(i))) {
      max_known = i;
    }
  }

  if (max_known < 0) {
    fprintf(stderr, "No known strerror messages found.\n");
    return 1;
  }

  printf("/* Generated on macOS. */\n");
  printf("#define S21_MAC_ERROR_COUNT %d\n", max_known + 1);
  printf("static const char *s21_mac_error_list[S21_MAC_ERROR_COUNT] = {\n");
  for (int i = 0; i <= max_known; ++i) {
    printf("    ");
    print_escaped_c_string(strerror(i));
    if (i != max_known) {
      putchar(',');
    }
    putchar('\n');
  }
  printf("};\n");

  return 0;
}
