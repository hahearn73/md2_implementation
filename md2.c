#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLOCK_SIZE 16

static unsigned char PI_SUBST[256] = {
  41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
  19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
  76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
  138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
  245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
  148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
  39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
  181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
  150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
  112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
  96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
  85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
  234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
  129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
  8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
  203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
  166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
  31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};

/*
 * Performs md2 hash on str and puts result in result
 *
 * params:
    * unsigned char *str - string to be hashed
    * int str_length - strlen(str)
    * unsigned char *result - >= 16 byte array where hash is placed as a return value
*/
void md2_hash(unsigned char *str, int str_length, unsigned char *result) {
    // padding
    int original_length = str_length;
    int padding = BLOCK_SIZE - (original_length % BLOCK_SIZE);
    unsigned char *msg = (unsigned char *)malloc(original_length + padding + BLOCK_SIZE);
    if (msg == NULL) {
        fprintf(stderr, "failed to allocate");
        return;
    }
    memcpy(msg, str, original_length);
    for (int i = original_length; i < original_length + padding; i++)
        msg[i] = (unsigned char)padding;

    // checksum
    unsigned char checksum[BLOCK_SIZE] = {0};
    unsigned char l = 0;
    int blocks = (int)ceil(((double)(original_length + padding)) / BLOCK_SIZE);
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            l = PI_SUBST[msg[i * BLOCK_SIZE + j] ^ l] ^ checksum[j];
            checksum[j] = l;
        }
    }
    for (int i = 0; i < BLOCK_SIZE; i++)
        msg[original_length + padding + i] = checksum[i];
    blocks += 1;

    // message digest
    unsigned char message_digest[BLOCK_SIZE * 3] = {0};
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            message_digest[BLOCK_SIZE + j] = msg[i * BLOCK_SIZE + j];
            message_digest[2 * BLOCK_SIZE + j] = message_digest[BLOCK_SIZE + j] ^ message_digest[j];
        }
        unsigned char t = 0;
        for (int j = 0; j < 18; j++) {
            for (int k = 0; k < BLOCK_SIZE * 3; k++) {
                t = message_digest[k] ^ PI_SUBST[t];
                message_digest[k] = t;
            }
            t = (t+j) % 256;
        }
    }

    // output
    for (int i = 0; i < BLOCK_SIZE; i++) {
        result[i] = message_digest[i];
    }
    free(msg);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    unsigned char *str = argv[1];
    unsigned char *result = malloc(16);
    md2_hash(str, strlen(str), result);
    for (int i = 0; i < BLOCK_SIZE; i++)
        printf("%02x", result[i]);
    putc('\n', stdout);
    
    free(result);
    return 0;
}
