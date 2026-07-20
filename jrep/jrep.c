#include "vect.h"

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

size_t *findMatchInLines(const char *lines[], const char *target) {
  size_t *matches = VECT();

  for (size_t i = 0; i < VECT_LEN(lines); i++) {
    if (strstr(lines[i], target) != NULL) {
      VECT_PUSH(matches, i);
    }
  }

  return matches;
}

char **jrep_file(const char *fileName, const char *target) {
  FILE *file_handle = fopen(fileName, "r");
  if (file_handle == NULL) {
    return NULL;
  }

  const char **lines = VECT();

  char *line = VECT();
  int c;

  while ((c = fgetc(file_handle)) != EOF) {
    VECT_PUSH(line, (char)c);

    if (c == '\n') {
      VECT_PUSH(line, '\0');
      VECT_PUSH(lines, line);
      line = VECT();
    }
  }

  if (VECT_LEN(line) > 0) {
    VECT_PUSH(line, '\0');
    VECT_PUSH(lines, line);
  } else {
    VECT_FREE(line);
  }

  fclose(file_handle);

  size_t *matches = findMatchInLines(lines, target);

  char **results = VECT();

  for (size_t i = 0; i < VECT_LEN(matches); i++) {
    size_t len = VECT_LEN(lines[matches[i]]) + 8;
    char *tmp = malloc(len);
    snprintf(tmp, len, "%zu : %s", i + 1, lines[matches[i]]);
    VECT_PUSH(results, tmp);
  }

  for (size_t i = 0; i < VECT_LEN(lines); i++) {
    VECT_FREE(lines[i]);
  }

  VECT_FREE(lines);
  VECT_FREE(matches);

  return results;
}

char **jrep_dir(const char *dirName, const char *target) {
  DIR *d = opendir(dirName);

  if (d == NULL) {
    perror("failed to open directory");
    return NULL;
  }

  char **results = VECT();
  struct dirent *entry;

  while ((entry = readdir(d)) != NULL) {
    const char *curr = entry->d_name;

    if (strcmp(curr, ".") == 0 || strcmp(curr, "..") == 0)
      continue;

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dirName, curr);

    struct stat info;
    if (stat(path, &info) != 0)
      continue;

    if (!S_ISREG(info.st_mode))
      continue;

    char **tmp = jrep_file(path, target);

    if (tmp == NULL) {
      continue;
    }

    if (VECT_LEN(tmp) == 0) {
      VECT_FREE(tmp);
      continue;
    }
    strcat(path, "\n");

    VECT_PUSH(results, strdup(path));
    for (size_t j = 0; j < VECT_LEN(tmp); j++) {
      VECT_PUSH(results, tmp[j]);
    }

    VECT_FREE(tmp);
  }

  closedir(d);

  return results;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: jrep PATTERN [FILE]\n");
    return EXIT_FAILURE;
  }

  const char *target = argv[1];
  const char *fileName = argv[2];

  struct stat info;

  if (stat(fileName, &info) != 0) {
    perror("stat");
    return EXIT_FAILURE;
  }

  char **results = NULL;

  if (S_ISDIR(info.st_mode)) {
    results = jrep_dir(fileName, target);
  } else if (S_ISREG(info.st_mode)) {
    results = jrep_file(fileName, target);
  } else {
    fprintf(stderr, "Unsupported file type.\n");
    return EXIT_FAILURE;
  }

  if (results == NULL) {
    return EXIT_FAILURE;
  }
  if (VECT_LEN(results) == 0) {
    VECT_FREE(results);
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < VECT_LEN(results); i++) {
    printf("%s", results[i]);
  }

  for (size_t i = 0; i < VECT_LEN(results); i++) {
    free(results[i]);
  }

  VECT_FREE(results);

  return EXIT_SUCCESS;
}
