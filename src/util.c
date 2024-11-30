#include "util.h"

#include "logging.h"

#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

char *read_file(const char *path)
{
    FILE *f;
    int ret;
    long fsize;
    char *content;

    if (path == NULL) {
        log_error("Input file cannot be NULL");
        return NULL;
    }

    // open file
    f = fopen(path, "rb");
    if (f == NULL) {
        log_error("Failed to open file \'%s\': %s", path, strerror(errno));
        return NULL;
    }

    log_info("Successfully opened file \'%s\'", path);

    // seek to end of file
    ret = fseek(f, 0, SEEK_END);
    if (ret) {
        log_error("Failed to seek end of file \'%s\': %s", path, strerror(errno));
        fclose(f);
        return NULL;
    }

    // get file size
    fsize = ftell(f);
    if (fsize < 0) {
        log_error("Failed to tell end-position of file \'%s\': %s", path, strerror(errno));
        fclose(f);
        return NULL;
    }

    log_debug("File has size of %ld bytes", fsize);

    // seek to beginning of file
    ret = fseek(f, 0, SEEK_SET);
    if (ret) {
        log_error("Failed to seek beginning of file \'%s\': %s", path, strerror(errno));
        fclose(f);
        return NULL;
    }

    // allocate memory for content
    content = (char *) malloc(fsize + 1);
    content[fsize] = 0;

    // return if size is 0
    if (fsize == 0) {
        log_warning("Could not read \'%s\', file is empty", path);
        fclose(f);
        return content;
    }

    // read content in one swoop
    ret = fread(content, fsize, 1, f);
    if (ret != 1) {
        if (feof(f)) {
            log_warning("Failed to read whole file content of \'%s\': encounterd EOF character", path);
        } else {
            log_error("Failed to read file \'%s\'", path);
            fclose(f);
            free(content);
            return NULL;
        }
    }
    fclose(f);

    log_info("Successfully read %ld bytes from \'%s\'", fsize, path);

    return content;
}

int write_file_mode(const char *path, const char *content, size_t length, const char *mode)
{
    FILE *f; 
    size_t ret;

    if (path == NULL) {
        log_error("Output file cannot be NULL");
        return 0;
    }

    // open file
    f = fopen(path, mode);
    if (f == NULL) {
        log_error("Failed to create file \'%s\': %s", path, strerror(errno));
        return 0;
    }

    log_info("Successfully created file \'%s\'", path);

    // create file but don't write if length is 0
    if (length == 0) {
        log_warning("Content has length zero", path);
        fclose(f);
        return 1;
    }

    // write content to file
    ret = fwrite(content, 1, length, f);
    if (ret < length) {
        log_error("Failed to write whole content to \'%s\', characters written: %zu bytes", path, ret);
        fclose(f);
        return 0;
    }

    log_info("Successfully wrote %zu bytes to \'%s\'", ret, path);

    fclose(f);
    return 1;
}

int write_file(const char *path, const char *content, size_t length)
{
    return write_file_mode(path, content, length, "wb");
}

int append_file(const char *path, const char *content, size_t length)
{
    return write_file_mode(path, content, length, "ab");
}
