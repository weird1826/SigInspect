#include <stdio.h>
#include <string.h>

typedef struct
{
    const char *name;
    const unsigned char *signature;
    int sig_length;
} FileSignature;

const unsigned char ZIP_SIG[] = {0x50, 0x4B, 0x03, 0x04};                                  // ZIP
const unsigned char PDF_SIG[] = {0x25, 0x50, 0x44, 0x46};                                  // PDF
const unsigned char GIF_SIG[] = {0x47, 0x49, 0x46, 0x38};                                  // GIF
const unsigned char PNG_SIG[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};          // PNG
const unsigned char EXE_SIG[] = {0x4D, 0x5A};                                              // EXE
const unsigned char JPEG_SIG[] = {0xFF, 0xD8, 0xFF};                                       // JPEG
const unsigned char ELF_SIG[] = {0x7F, 0x45, 0x4C, 0x46};                                  // ELF (Linux Executable)
const unsigned char JAVA_SIG[] = {0xCA, 0xFE, 0xBA, 0xBE};                                 // Java Class
const unsigned char MP3_SIG[] = {0x49, 0x44, 0x33};                                        // ID3 (MP3 with ID3v2 container)
const unsigned char BMP_SIG[] = {0x42, 0x4D};                                              // BMP
const unsigned char RAR_SIG[] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07};                      // RAR Archive
const unsigned char SEVENZ_SIG[] = {0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C};                   // 7-Zip Archive
const unsigned char GZIP_SIG[] = {0x1F, 0x8B};                                             // GZIP
const unsigned char FLAC_SIG[] = {0x66, 0x4C, 0x61, 0x43};                                 // FLAC Audio
const unsigned char MIDI_SIG[] = {0x4D, 0x54, 0x68, 0x64};                                 // MIDI Audio
const unsigned char TIFF_LE_SIG[] = {0x49, 0x49, 0x2A, 0x00};                              // TIFF (Little Endian)
const unsigned char TIFF_BE_SIG[] = {0x4D, 0x4D, 0x00, 0x2A};                              // TIFF (Big Endian)
const unsigned char UTF8_BOM[] = {0xEF, 0xBB, 0xBF};                                       // UTF-8 Text
const unsigned char UTF16LE_BOM[] = {0xFF, 0xFE};                                          // UTF-16 LE Text
const unsigned char UTF16BE_BOM[] = {0xFE, 0xFF};                                          // UTF-16 BE Text
const unsigned char RTF_SIG[] = {0x7B, 0x5C, 0x72, 0x74, 0x66, 0x31};                      // RTF ({\rtf1)
const unsigned char HTML_SIG_1[] = {0x3C, 0x21, 0x64, 0x6F, 0x63, 0x74, 0x79, 0x70, 0x65}; // <!doctype html (lowercase)
const unsigned char HTML_SIG_2[] = {0x3C, 0x68, 0x74, 0x6D, 0x6C, 0x3E};                   // <html> (lowercase)
const unsigned char HTML_SIG_3[] = {0x3C, 0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45}; // <!DOCTYPE HTML (uppercase)

FileSignature SIGNATURE_DB[] = {
    {"ZIP Archive", ZIP_SIG, sizeof(ZIP_SIG)},
    {"PDF Document", PDF_SIG, sizeof(PDF_SIG)},
    {"PNG Image", PNG_SIG, sizeof(PNG_SIG)},
    {"JPEG Image", JPEG_SIG, sizeof(JPEG_SIG)},
    {"GIF Image", GIF_SIG, sizeof(GIF_SIG)},
    {"Windows Executable", EXE_SIG, sizeof(EXE_SIG)},
    {"Linux Executable", ELF_SIG, sizeof(ELF_SIG)},
    {"Java Class File", JAVA_SIG, sizeof(JAVA_SIG)},
    {"MP3 Audio", MP3_SIG, sizeof(MP3_SIG)},
    {"BMP Image", BMP_SIG, sizeof(BMP_SIG)},
    {"RAR Archive", RAR_SIG, sizeof(RAR_SIG)},
    {"7-Zip Archive", SEVENZ_SIG, sizeof(SEVENZ_SIG)},
    {"GZIP Archive", GZIP_SIG, sizeof(GZIP_SIG)},
    {"FLAC Audio", FLAC_SIG, sizeof(FLAC_SIG)},
    {"MIDI Audio", MIDI_SIG, sizeof(MIDI_SIG)},
    {"TIFF Image (LE)", TIFF_LE_SIG, sizeof(TIFF_LE_SIG)},
    {"TIFF Image (BE)", TIFF_BE_SIG, sizeof(TIFF_BE_SIG)},
    {"UTF-8 Text (with BOM)", UTF8_BOM, sizeof(UTF8_BOM)},
    {"UTF-16 LE Text", UTF16LE_BOM, sizeof(UTF16LE_BOM)},
    {"UTF-16 BE Text", UTF16BE_BOM, sizeof(UTF16BE_BOM)},
    {"Rich Text Format (RTF)", RTF_SIG, sizeof(RTF_SIG)},
    {"HTML Document (<!doctype)", HTML_SIG_1, sizeof(HTML_SIG_1)},
    {"HTML Document (<html>)", HTML_SIG_2, sizeof(HTML_SIG_2)},
    {"HTML Document (<!DOCTYPE)", HTML_SIG_3, sizeof(HTML_SIG_3)}};

int DB_SIZE = sizeof(SIGNATURE_DB) / sizeof(SIGNATURE_DB[0]);

const char *check_signature(FILE *f)
{
    unsigned char header[64];

    fseek(f, 0, SEEK_SET);

    size_t bytes_read = fread(header, 1, 64, f);
    if (bytes_read == 0)
    {
        return "Empty or unreadable";
    }

    for (int i = 0; i < DB_SIZE; i++)
    {
        if (bytes_read >= SIGNATURE_DB[i].sig_length)
        {
            if (memcmp(header, SIGNATURE_DB[i].signature, SIGNATURE_DB[i].sig_length) == 0)
            {
                return SIGNATURE_DB[i].name;
            }
        }
    }

    return "Unknown Type";
}

void open_file(const char *filepath)
{
    FILE *f = fopen(filepath, "rb");

    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s: ", filepath);
        perror("");
        return;
    }

    printf("[*] File opened successfully!");
    printf("\n[!] Analysis Results for: %s\n", filepath);
    const char *file_type = check_signature(f);
    printf("\tFile Type (Signature): %s\n", file_type);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file_to_scan>\n", argv[0]);
    }

    open_file(argv[1]);

    return 0;
}