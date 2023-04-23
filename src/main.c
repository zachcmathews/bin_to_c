#include <stdio.h>
#include <stdlib.h>

/**
 * Reads the content of a file into buffer.
 * @param[in] file the file to read
 * @param[out] pContents a pointer to a variable to store the created buffer's
 * address
 * @return -1 if error, otherwise the number of bytes read
 */
ssize_t read_bin(char const *file, char **pContents) {
  FILE *fd = fopen(file, "rb");
  if (!fd) {
    return -1;
  }

  if (fseek(fd, 0, SEEK_END) != 0) {
    fclose(fd);
    return -1;
  }

  long len = ftell(fd);
  if (len == -1L) {
    fclose(fd);
    return -1;
  }

  *pContents = malloc(len);
  if (!*pContents) {
    fclose(fd);
    return -1;
  }

  if (fseek(fd, 0, SEEK_SET) != 0) {
    free(*pContents);
    *pContents = NULL;

    fclose(fd);
    return -1;
  }

  unsigned long bytes_read = fread(*pContents, 1, len, fd);
  if (bytes_read != len) {
    free(*pContents);
    *pContents = NULL;

    fclose(fd);
    return -1;
  }

  fclose(fd);
  return len;
}

int main(int argc, char *argv[]) {
  if (argc < 3)
    return -1;

  if (argc % 2 == 0)
    return -1;

  size_t num_bins = (argc - 1) / 2;

  for (size_t i = 0; i < num_bins; ++i) {
    char *name = argv[i * 2 + 1];
    char *content;

    ssize_t len = read_bin(argv[i * 2 + 2], &content);
    if (len == -1)
      return -1;

    fprintf(stdout, "const char %s[] = {\n\t", name);

    for (size_t j = 0; j < len; ++j) {
      fprintf(stdout, "0x%02x, ", content[j]);
      if ((j + 1) % 10 == 0)
        fprintf(stdout, "\n\t");
    }

    fprintf(stdout, "\n};\n\n");

    free(content);
  }

  return 0;
}
