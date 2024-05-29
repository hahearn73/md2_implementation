#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input\n", argv[0]);
        return 1;
    }

    // Padding
    char *message;
    int original_message_length = strlen(argv[1]);
    int padding_required = 16 - (original_message_length % 16);
    int message_length = original_message_length + padding_required;
    message = malloc(message_length + 1);
    if (message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    memcpy(message, argv[1], original_message_length);
    for (int i = original_message_length; i < message_length; i++) {
        message[i] = (char)padding_required;
    }
    message[message_length] = '\0';
    printf("padded message: %s\n", message);

    // Checksum
    char checksum[16];
    for (int i = 0; i < 16; i++) {
        checksum[i] = 0;
    }

    char L = 0;
    char c = 0;
    for (int i = 0; i <= message_length / 16 - 1; i++) {
        for (int j = 0; j < 16; j++) {
            c = message[i * 16 + j];
            checksum[j] = PI_SUBST[c ^ L];
            L = checksum[j];
        }
    }

    message = realloc(message, message_length + 16 + 1);
    if (message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    memcpy(message + message_length, checksum, 16);
    message[message_length + 16 + 1] = '\0';
    printf("checksum message: ");
    for (int i = 0; i < message_length + 16; i++) {
        putc(message[i], stdout);
    }
    printf("\n");

    // Initizalize MD buffer
    char X[48];
    for (int i = 0; i < 48; i++) {
        X[i] = 0;
    }

    // Process message in 16 byte blocks
    for (int i = 0; i <= (message_length + 16) / 16 - 1; i++) {
        for (int j = 0; j < 16; j++) {
            X[16 + j] = message[i * 16 + j];
            X[32 + j] = X[16 + j] ^ X[j];
        }
        char t = 0;

        for (int j = 0; i < 18; i++) {
            for (int k = 0; k < 48; k++) {
                t = X[k] ^ PI_SUBST[(uint8_t)t];
                X[k] = t;
            }
            t = t + j % 256;
        }
    }
    printf("message digest: ");
    for (int i = 0; i < 48; i++) {
        printf("%c", X[i]);
    }
    printf("\n");

    // cleanup
    free(message);
    return 0;
}
