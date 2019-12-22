#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "timing.h"

#ifdef _WIN32

#include <windows.h>

#else
#include <unistd.h>
#endif

#include "dr_wav.h"

#define DR_WAV_IMPLMENTATION

// write wav file
void wavWrite_int16(char *filename, int16_t *buffer,
                    int sampleRate, uint32_t totalSampleCount) {
    drwav_data_format format;
    // drwav_container_riff -> normal WAV files
    // drwav_container_w64 -> Sony Wave64
    format.container = drwav_container_riff;
    format.format = DR_WAVE_FORMAT_PCM;
    format.channels = 1;
    format.sampleRate = sampleRate;
    format.bitsPerSample = 16;
    drwav *pWav = drwav_open_file_write(filename, &format);
    if (pWav) {
        drwav_uint64 samplesWritten = drwav_write(pWav, totalSampleCount, buffer);
        drwav_uninit(pWav);
        if (samplesWritten != totalSampleCount) {
            fprintf(stderr, "write file [%s] error.\n", filename);
            exit(1);
        }
    }
}

// read wav file
int16_t *wavRead_int16(char *filename, uint32_t *sampleRate, uint64_t *totalSampleCount) {
    unsigned int channels;
    int16_t *buffer = drwav_open_file_and_read_pcm_frames_s16(filename, &channels, sampleRate, totalSampleCount);
    if (buffer == NULL) {
        printf("read wav file failed");
    }
    // just process mono audio
    if (channels != 1) {
        drwav_free(buffer);
        buffer = NULL;
        *sampleRate = 0;
        *totalSampleCount = 0;
    }
    return buffer;
}

void splitpath(const char *path, char *drv, char *dir, char *name, char *ext) {
    const char *end;
    const char *p;
    const char *s;
    if (path[0] && path[1] == ':') {
        if (drv) {
            *drv++ = *path++;
            *drv++ = *path++;
            *drv = '\0';
        }
    } else if (drv)
        *drv = '\0';
    for (end = path; *end && *end != ':';)
        end++;
    for (p = end; p > path && *--p != '\\' && *p != '/';)
        if (*p == '.') {
            end = p;
            break;
        }
    if (ext)
        for (s = end; (*ext = *s++);)
            ext++;
    for (p = end; p > path;)
        if (*--p == '\\' || *p == '/') {
            p++;
            break;
        }
    if (name) {
        for (s = p; s < end;)
            *name++ = *s++;
        *name = '\0';
    }
    if (dir) {
        for (s = path; s < p;)
            *dir++ = *s++;
        *dir = '\0';
    }
}

int main(int argc, char *argv[]) {
//    printf("a timing demo\n");
//    double startTime = now();
//    int delay = 100;
//#ifdef _WIN32
//    Sleep(delay);
//#else
//    usleep(delay);
//#endif
//    double time_interval = calcElapsed(startTime, now());
//    printf("time interval: %d ms\n", (int) (time_interval * 1e3));
//    printf("press any key to exit.\n");
//    getchar();

    printf("Audio Process");
    printf("Supoort mono Audio");
    if (argc < 2) return -1;
    char *in_file = argv[1];

    uint32_t sampleRate = 0;
    uint64_t totalSampleCount = 0;
    int16_t *wavBuffer = NULL;
    double startTime = now();
    wavBuffer = wavRead_int16(in_file, &sampleRate, &totalSampleCount);
    double time_interval = calcElapsed(startTime, now());
    printf("load file time cost: %d ms\n", (int) (time_interval * 1e3));
    // if wav load successfully
    if (wavBuffer != NULL) {
        // make first half audio mute
        for (uint64_t i = 0; i < totalSampleCount / 2; ++i) {
            wavBuffer[i] = 0;
        }
    }
    startTime = now();
    char drive[3];
    char dir[256];
    char filename[256];
    char ext[256];
    char out_file[1024];
    splitpath(out_file, drive, dir, filename, ext);
    sprintf(out_file, "%s%s%s_out%s", drive, dir, filename, ext);
    wavWrite_int16(out_file, wavBuffer, sampleRate, totalSampleCount);
    time_interval = calcElapsed(startTime, now());
    printf("write file time cost: %d ms\n", (int) (time_interval * 1e3));
    if (wavBuffer) {
        free(wavBuffer);
    }
    printf("press any key to exit\n");
    getchar();

    return 0;
}
