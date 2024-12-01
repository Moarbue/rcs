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

    log_info("Opening file \'%s\'...", path);

    // open file
    f = fopen(path, "rb");
    if (f == NULL) {
        log_error("Failed to open file \'%s\': %s", path, strerror(errno));
        return NULL;
    }

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
        log_warning("File \'%s\' is empty, aborting read operation...", path);
        fclose(f);
        return content;
    }

    // read content in one swoop
    ret = fread(content, fsize, 1, f);
    if (ret != 1) {
        if (feof(f)) {
            log_warning("Encountered EOF character before file end, aborting read operation...", path);
        } else {
            log_error("Read operation failed, \'%s\'", path);
            fclose(f);
            free(content);
            return NULL;
        }
    }
    fclose(f);

    log_info("Read %ld bytes from file", fsize);

    return content;
}

int write_file(const char *path, const char *content, size_t length)
{
    FILE *f; 
    size_t ret;

    if (path == NULL) {
        log_error("Output file cannot be NULL");
        return 0;
    }
    
    log_info("Creating file \'%s\'...", path);

    // open file
    f = fopen(path, "wb");
    if (f == NULL) {
        log_error("Failed to create file \'%s\': %s", path, strerror(errno));
        return 0;
    }

    // create file but don't write if length is zero
    if (length == 0) {
        log_warning("Content has length zero, aborting write operation...", path);
        fclose(f);
        return 1;
    }

    // write content to file
    ret = fwrite(content, 1, length, f);
    if (ret < length) {
        log_error("Write operation failed, \'%s\', characters written: %zu bytes", path, ret);
        fclose(f);
        return 0;
    }

    log_info("Wrote %zu bytes to \'%s\'", ret, path);

    fclose(f);
    return 1;
}

int append_file(const char *path, const char *content, size_t length)
{
    FILE *f; 
    size_t ret;

    if (path == NULL) {
        log_error("Output file cannot be NULL");
        return 0;
    }
    
    log_info("Opening file \'%s\'...", path);

    // open file
    f = fopen(path, "ab");
    if (f == NULL) {
        log_error("Failed to open file \'%s\': %s", path, strerror(errno));
        return 0;
    }

    // create file but don't write if length is zero
    if (length == 0) {
        log_warning("Content has length zero, aborting append operation...", path);
        fclose(f);
        return 1;
    }

    // write content to file
    ret = fwrite(content, 1, length, f);
    if (ret < length) {
        log_error("Append operation failed, \'%s\', characters appended: %zu bytes", path, ret);
        fclose(f);
        return 0;
    }

    log_info("Appended %zu bytes to \'%s\'", ret, path);

    fclose(f);
    return 1;
}
