#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bytearray.h"
#include "tinyfiledialogs.h"
#define MB 1000000

int main(int argc, char** argv){
    char* in;
    char* outfile;
    if (argc < 2){
        in = tinyfd_openFileDialog("Select File to Split", "./", 0, NULL, NULL, 0);
    }
    else{
        in = argv[1];
    }
    if (!in){
        fprintf(stderr, "ERROR: filename invalid!\n");
        return -1;
    }
    outfile = in;
    if (argc == 3){
        outfile = argv[2];
    }
    printf("%s\n", in);
    size_t infile_chunk_cnt = 0;
    size_t final_chunk_size = 0;
    ByteArray* full = file_to_byte_array(in);
    if (!full){
        fprintf(stderr, "error opening file %s\n", in);
        return -1;
    }
    char outfile_final[0xFF];
    infile_chunk_cnt = ((full->bufsize / (8 * MB)) + 1);
    ByteArray* arr[infile_chunk_cnt]; 
    final_chunk_size = full->bufsize - ((8 * MB) * (infile_chunk_cnt - 1));
    // last chunk will often not be 8mb flat, allocate it separately.
    for (size_t i = 0; i < infile_chunk_cnt - 1; ++i){
        arr[i] = create_empty_byte_array(8 * MB);
        memcpy(arr[i]->buf, &full->buf[i * 8 * MB], 8 * MB);
    }
    arr[infile_chunk_cnt - 1] = create_empty_byte_array(final_chunk_size);
    memcpy(arr[infile_chunk_cnt - 1]->buf, &full->buf[(infile_chunk_cnt - 1) * (MB * 8)], final_chunk_size);
    cleanup_bytearray(&full);
    char suffix = 0x41;
    for (size_t i = 0; i < infile_chunk_cnt; ++i){
        if (suffix + i == 0x5A){
            suffix = 0x61;
        }
        sprintf(outfile_final, "%s_%c.chk", outfile, suffix + i);
        byte_array_to_file(arr[i], outfile_final);
        cleanup_bytearray(&arr[i]);
    }
    return 0;
}
