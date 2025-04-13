#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bytearray.h"
#define MB 1000000

int main(int argc, char** argv){
    size_t infile_chunk_cnt = 0;
    size_t final_chunk_size = 0;
    if (argc < 2){
        printf("USAGE: %s {in} {out}\n", argv[0]);
        return -1;
    }
    char* outfile = argv[1]; 
    if (argc > 3){
        outfile = argv[2];
    }
    ByteArray* full = file_to_byte_array(argv[1]); 
    if (!full){
        fprintf(stderr, "file %s not found\n", argv[1]);
        return -1;
    }
    char outfile_final[200];
    infile_chunk_cnt = ((full->bufsize / (8 * MB)) + 1);
    ByteArray* arr[infile_chunk_cnt]; 
    final_chunk_size = full->bufsize - ((8 * MB) * (infile_chunk_cnt - 1));
    // last chunk will often not be 8mb flat, allocate it separately.
    for (size_t i = 0; i < infile_chunk_cnt - 1; ++i){
        arr[i] = create_empty_byte_array(8 * MB);
        memcpy(arr[i]->buf, full->buf + i * MB, 8 * MB);
    }
    arr[infile_chunk_cnt - 1] = create_empty_byte_array(final_chunk_size);
    memcpy(arr[infile_chunk_cnt - 1]->buf, full->buf + ((infile_chunk_cnt - 1) * MB), final_chunk_size);
    cleanup_bytearray(&full);
    for (size_t i = 0; i < infile_chunk_cnt; ++i){
        sprintf(outfile_final, "%s_%zu.chk", outfile, i);
        byte_array_to_file(arr[i], outfile_final);
        cleanup_bytearray(&arr[i]);
    }
    return 0;
}
