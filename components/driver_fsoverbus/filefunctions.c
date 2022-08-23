#include <esp_err.h>
#include <esp_log.h>
#include <esp_vfs.h>
#include <dirent.h>

#include <esp_task_wdt.h>

#include "include/fsob_backend.h"
#include "include/filefunctions.h"
#include "include/messageutils.h"
#include "include/driver_fsoverbus.h"

#define TAG "fsoveruart_ff"

const char root[] = {"dflash\ndsdcard"};

/***
 * All functions in this file follow the same flow. Each function gets called when data has arrived.
 * This can be partial data, or the complete packet.
 * data - Pointer to the bytes received.
 * Command - Command ID
 * Size - The complete payload size of the command in bytes
 * Received - How many bytes have already been received
 * Length - The length that has been received in this packet.
 * 
 * When the function returns 0 the next packet received will be appended to the previous received data.
 * When the function returns 1 the program will place the next received bytes at data[0], all previous received data will be deleted.
 * 
 * 
 ***/
int getdir(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    if(received != size) return 0;

    if(size == 0 || size == 1 || size == 2) { //Requesting root
        strcat((char *) data, "\n");  //Append folder name and type
        strcat((char *) data, root); //Append root structure
        uint8_t header[12];
        createMessageHeader(header, command, strlen((char *) data), message_id);
        fsob_write_bytes(true, false, (const char*) header, 12);
        fsob_write_bytes(false, true, (const char*) data, strlen((char *) data));
        return 1;
    }
     //TODO: Fix when folder list exceeds buffer
    ESP_LOGI(TAG, "%s", data);
    char dir_name[size+20];   //Take length of the folder and add the spiflash mountpoint
    buildfile((char *) data, dir_name);
    ESP_LOGI(TAG, "%s", dir_name);
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name);  //Loop through all files/directories
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            strncat((char *) data, "\n", RD_BUF_SIZE-1);
            strncat((char *) data, dir->d_type == DT_DIR ? "d" : "f", RD_BUF_SIZE-1);
            strncat((char *) data, dir->d_name, RD_BUF_SIZE-1);
            if(strlen((char *) data) > RD_BUF_SIZE-10) break;
        }
        closedir(d);    
    } else {
        strcpy((char *) data, "Directory_not_found");  //Cant find directory, request dir doesnt exists
    }
    uint8_t header[12];
    //ESP_LOGI(TAG, "len: %d", strlen((char *) data));
    createMessageHeader(header, command, strlen((char *) data), message_id);
    fsob_write_bytes(true, false, (const char*) header, 12);
    fsob_write_bytes(false, true, (const char*) data, strlen((char *) data));

    return 1;
}

int readfile(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    if(received != size) return 0;

    char dir_name[size+10];   //Take length of the folder and add the spiflash mountpoint
    buildfile((char *) data, dir_name);

    FILE *fptr_glb;
    fptr_glb = fopen(dir_name, "r");
    if(fptr_glb) {
        fseek(fptr_glb, 0, SEEK_END);
        uint32_t size_file = ftell(fptr_glb);
        ESP_LOGI(TAG, "file size: %d", size_file);    
        //Create header with file size
        uint8_t header[12];
        createMessageHeader(header, command, size_file, message_id);
        fsob_write_bytes(true, false, (const char*) header, 12);
        
        fseek(fptr_glb, 0, SEEK_SET);
        uint32_t read_bytes = 0;
        do {
            read_bytes += fread(data, 1, 64, fptr_glb);
            fsob_write_bytes(false, read_bytes == size_file, (const char*) data, read_bytes);
            vTaskDelay(1);  //Delay task to ensure data has time to gets flushed to webusb
        } while(read_bytes != size_file);
        fclose(fptr_glb);
    } else {
        strcpy((char *) data, "Can't open file");
        uint8_t header[12];
        createMessageHeader(header, command, strlen((char *) data), message_id);
        fsob_write_bytes(true, false, (const char*) header, 12);
        fsob_write_bytes(false, true, (const char*) data, strlen((char *) data));
    }
    return 1;
}

int writefile(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    static FILE *fptr = NULL;
    static int failed_open = 0;
    static char dir_name[256];
    static char dir_name_tmp[256];


    if(received == length) {                //Opening new file, cleaning up statics just in case
        if(fptr) fclose(fptr);
        failed_open = 0;
        fptr = NULL;
    }

    if(fptr == NULL && failed_open == 0) {
        for(int i = 0; i < received; i++) {
            if(data[i] == 0) {
                if(i > 250) {
                    failed_open = 1;
                    return 1;
                }
                buildfile((char *) data, dir_name);
                int len = snprintf(dir_name_tmp, 255, "%s.tmp", dir_name);
                if (len < 0) {
                    ESP_LOGE(TAG, "Buffer is too small");
                }
                ESP_LOGI(TAG, "Writing: %s", dir_name_tmp);

                fptr = fopen(dir_name_tmp, "w");
                if(fptr) {
                    if(received > i) {
                        fwrite(&data[i+1], 1, received-i-1, fptr);
                    }
                } else {
                    ESP_LOGI(TAG, "Open failed");
                    failed_open = 1;
                }

                if(received == size) {    //Creating an empty file or short. Close the file and send reply
                    failed_open = 0;
                    if(fptr) {
                        sendok(command, message_id);
                        fclose(fptr);
                        remove(dir_name);
                        rename(dir_name_tmp, dir_name);
                        fptr = NULL;
                    } else {
                        sender(command, message_id);
                    }
                }
                return 1;
            }
        }
        return 0;   //Found no 0 terminator. File path not received. Wait for more data to arrived to get the filename
    } else if(fptr) {
        ESP_LOGI(TAG, "fptr: %d %d", (uint32_t) data, length);
        fwrite(data, 1, length, fptr);
        if(received == size) {  //Finished receiving
            sendok(command, message_id);
            fclose(fptr);
            remove(dir_name);
            rename(dir_name_tmp, dir_name);
            fptr = NULL;
            failed_open = 0;            
        } else {
            vTaskDelay(1);
        }
        return 1;
    } else {
        if(received == size) {
            sender(command, message_id);
            fptr = NULL;
            failed_open = 0;
        }
        return 1;
    }
}

int delfile(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    if(received != size) return 0;
    
    char dir_name[size+10];   //Take length of the folder and add the spiflash mountpoint
    buildfile((char *) data, dir_name);
    ESP_LOGI(TAG, "Source: %s", data);
    ESP_LOGI(TAG, "Deleting: %s", dir_name);
    
    if(remove(dir_name) == 0) {
        sendok(command, message_id);
    } else {
        sender(command, message_id);
    }
    return 1;
}

int cpyfile(uint8_t *data, uint16_t command, uint32_t size, uint32_t received, uint32_t length, uint32_t delete_source) {
    int source_len = strlen((char *) data);
    uint8_t *dest = &data[source_len+1];    
    int dest_len = strlen((char *) dest);
    int isfolder = dest[dest_len-1] == '/';

    

    int filename_index = 0;

    for(int i = source_len-1; i > 0; i--) {
        if(data[i] == '/') {
            filename_index = i+1;
            break;
        }
    }

    if(filename_index == 0) { //no filename found
        return 0;
    }

    uint32_t dest_file_len = dest_len + (source_len-filename_index) + 10;
    uint8_t dest_tmp[dest_file_len];

    if(isfolder) {
        strncpy((char *) dest_tmp, (char *) dest, dest_file_len);
        strncat((char *) dest_tmp, (char *) &data[filename_index], dest_file_len);
        dest_len = strlen((char *) dest_tmp);
        dest = dest_tmp;
    }

    char source_file[source_len+30];
    buildfile((char *) data, source_file);
    char dest_file[dest_len+30];
    buildfile((char *) dest, dest_file);

    ESP_LOGI(TAG, "source: %s", source_file);
    ESP_LOGI(TAG, "dest: %s", dest_file);

    if(strcmp(source_file, dest_file) == 0) return 0;   //If dest and source are the same return error

    FILE *source, *target;
    source = fopen(source_file, "r");
    target = fopen(dest_file, "w");

    if(source && target) {
        int read_bytes;
        uint8_t buf[64];
        do {
            read_bytes = fread(buf, 1, 64, source);
            fwrite(buf, 1, read_bytes, target);
        } while(read_bytes > 0);
        fclose(source);
        fclose(target);
        if(delete_source) remove(source_file);
        return 1;
    } else {
        return 0;
    }

}

int duplfile(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    if(received != size) return 0;
   
    if(cpyfile(data, command, size, received, length, 0)) {       
        sendok(command, message_id);
    } else {
        sender(command, message_id);
    }

    return 1;
}

int mvfile(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    if(received != size) return 0;
   
    if(cpyfile(data, command, size, received, length, 1)) {       
        sendok(command, message_id);
    } else {
        sender(command, message_id);
    }

    return 1;
}

int makedir(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    if(received != size) return 0;

    char dir_name[size+20];   //Take length of the folder and add the spiflash mountpoint
    buildfile((char *) data, dir_name);
    ESP_LOGI(TAG, "mkdir: %s", dir_name);

    if(mkdir(dir_name, 0777) == 0) {
        sendok(command, message_id);
    } else {
        sender(command, message_id);
    }
    return 1;

}