#include <stdint.h>
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
  if (argc < 3) {
    fprintf(stderr, "You must provide consecutive pairs of byte array names "
                    "and binary source files as input.\n");
    return -1;
  }

  if (argc % 2 == 0) {
    fprintf(stderr, "You must provide consecutive pairs of byte array names "
                    "and binary source files as input.\n");
    return -1;
  }

  size_t num_bins = (argc - 1) / 2;

  fprintf(stdout, "#include <stdint.h>\n\n");

  for (size_t i = 0; i < num_bins; ++i) {
    char *name = argv[i * 2 + 1];
    char *content;

    ssize_t len = read_bin(argv[i * 2 + 2], &content);
    if (len == -1) {
      fprintf(stderr, "Failed to read file %s\n", argv[i * 2 + 2]);
      return -1;
    }

    const size_t spv_len = len / 4;
    uint32_t *spv = (uint32_t *)content;

    fprintf(stdout, "const uint32_t %s[] = {\n\t", name);

    for (size_t j = 0; j < spv_len; ++j) {
      fprintf(stdout, "0x%08x, ", spv[j]);
      if ((j + 1) % 10 == 0)
        fprintf(stdout, "\n\t");
    }

    fprintf(stdout, "\n};\n\n");

    free(content);
  }

  return 0;
}
