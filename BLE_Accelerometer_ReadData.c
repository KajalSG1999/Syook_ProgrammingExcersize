/*File: ble_accelerometer_reader_linux.c
*/
/*System Header Files*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*Bluetooth BlueZ(Linux) header files*/
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

/*File Macros*/
#define TAG_MOVING_THRESHOLD 0.5

/*
*   Function Name: Read_accelerometer_data
*   Description  : Placeholder function for handling accelerometer data
*/
void Read_accelerometer_data(const char *device_addr, const char *device_name) 
{
    // Print device address and name
    printf("Device Address: %s, Device Name: %s\n", device_addr, device_name);

    // Extract accelerometer data from the device name (assuming it's in the format "AccX:AccY:AccZ")
    char *token = strtok(device_name, ":");
    double accX = atof(token);
    token = strtok(NULL, ":");
    double accY = atof(token);
    token = strtok(NULL, ":");
    double accZ = atof(token);

    // Calculate acceleration magnitude
    double magnitude = sqrt(accX * accX + accY * accY + accZ * accZ);

    // Check if the magnitude exceeds the threshold
    if (magnitude > TAG_MOVING_THRESHOLD) 
    {
        // If acceleration exceeds the threshold, print "Tag is moving"
        printf("Tag is moving\n");
    } 
    else 
    {
        // If acceleration is below the threshold, print "Tag is stationary"
        printf("Tag is stationary\n");
    }
}

int main() {
    int device_id, socket, data_length, flags;
    inquiry_info *devices = NULL;
    int max_responses = 255;
    int num_responses, i;

    // Get the Bluetooth device ID
    device_id = hci_get_route(NULL);

    // Open a Bluetooth socket
    socket = hci_open_dev(device_id);

    if (device_id < 0 || socket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set inquiry parameters
    data_length = 8;
    flags = IREQ_CACHE_FLUSH; // Set the flags to IREQ_CACHE_FLUSH to ensure a fresh inquiry without using cached results

    // Allocate memory for devices array
    devices = (inquiry_info*)malloc(max_responses * sizeof(inquiry_info));

    // Perform Bluetooth inquiry to discover nearby devices
    num_responses = hci_inquiry(device_id, data_length, max_responses, NULL, &devices, flags);

    if (num_responses < 0) {
        perror("Error inquiring devices");
        exit(1);
    }

    // Iterate through the discovered devices
    for (i = 0; i < num_responses; i++) {
        char addr[19] = { 0 };
        // Convert Bluetooth address to string format
        ba2str(&(devices + i)->bdaddr, addr);

        // Perform remote name request to get the device name
        char name[256] = { 0 };
        if (hci_read_remote_name(socket, &(devices + i)->bdaddr, sizeof(name), name, 0) < 0) {
            strcpy(name, "[unknown]");
        }

        // Process accelerometer data and print whether the tag is moving or stationary
        handle_accelerometer_data(addr, name);
    }

    // Free allocated memory
    free(devices);
    // Close the Bluetooth socket
    close(socket);

    return 0;
}

