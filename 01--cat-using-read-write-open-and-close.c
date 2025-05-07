#include <fcntl.h>    // for open
#include <unistd.h>   // for read, write, close
#include <stdio.h>    // for perror, snprintf
#include <stdlib.h>   // for exit, atoi
#include <string.h>   // for strcmp

#define BUFFER_SIZE 1024

void print_with_line_numbers(int fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int line_number = 1;
    char line_prefix[32];
    int new_line = 1;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (new_line) {
                int len = snprintf(line_prefix, sizeof(line_prefix), "%6d\t", line_number++);
                if (write(STDOUT_FILENO, line_prefix, len) != len) {
                    perror("write");
                    exit(1);
                }
                new_line = 0;
            }

            if (write(STDOUT_FILENO, &buffer[i], 1) != 1) {
                perror("write");
                exit(1);
            }

            if (buffer[i] == '\n') {
                new_line = 1;
            }
        }
    }

    if (bytes_read < 0) {
        perror("read");
    }
}

void print_plain(int fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
            perror("write");
            exit(1);
        }
    }

    if (bytes_read < 0) {
        perror("read");
    }
}

int main(int argc, char *argv[]) {
    int line_number_flag = 0;
    int file_start_index = 1;

    // Check for -n flag
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        line_number_flag = 1;
        file_start_index = 2;
    }

    // If no files are provided, read from stdin
    if (file_start_index == argc) {
        if (line_number_flag)
            print_with_line_numbers(STDIN_FILENO);
        else
            print_plain(STDIN_FILENO);
        return 0;
    }

    // Process each file
    for (int i = file_start_index; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror(argv[i]);
            continue;
        }

        if (line_number_flag)
            print_with_line_numbers(fd);
        else
            print_plain(fd);

        close(fd);
    }

    return 0;
}

//Ran this within