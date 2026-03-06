
#include <stdlib.h>
#include <stdio.h>
#include "libircmd.h"
#include "libiruart.h"

int main(int argc, char* argv[]) {

    bool set_params = true;

    // Register library logs and set log level to ERROR
    ircam_log_register(IRCAM_LOG_ERROR, NULL, NULL);
    ircmd_log_register(IRCMD_LOG_ERROR, NULL, NULL);
    iruart_log_register(IRUART_LOG_ERROR, NULL, NULL);

    // All handles used
    IrControlHandle_t* ir_control_handle = NULL;
    IruartHandle_t* iruart_handle = NULL;
    IrcmdHandle_t* ircmd_handle = NULL;

    int ret = IRLIB_SUCCESS;

    // Serial port parameters (Should be read from config)
    UartConDevParams_t param = {0};
    char uart_data[20] = "/dev/ttyUSB0";        // Serial Port
    param.baudrate = 115200;                    // Baudrate

    // Create control handle
    ir_control_handle_create(&ir_control_handle);

    // Create UART control handle
    iruart_handle = iruart_handle_create(ir_control_handle);

    // Open and initialize device
    ret = ir_control_handle->ir_control_open(ir_control_handle->ir_control_handle, (void*)(&uart_data));
    if (ret != IRLIB_SUCCESS) {
        printf("Device Open Failed! Return Error (ret) is: %d\n", ret);
        goto fail;
    }
    printf("Deviced opened succesfully...\n");
    ret = ir_control_handle->ir_control_init(ir_control_handle->ir_control_handle, (void*)(&param));
    if (ret != IRLIB_SUCCESS) {
        printf("Device Initialization Failed! Return Error (ret) is: %d\n", ret);
        ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);
        goto fail;
    }
    printf("Device initialized succesfully...\n");

    // Create command handle
    ircmd_handle = ircmd_create_handle(ir_control_handle);

    // --------- RUN START UP COMMANDS FOR THE IRCAM ---------
    // Get device name (WhoAmI)
    char data[100];
    ret = basic_device_info_get(ircmd_handle, BASIC_DEV_NAME, data);
    if (ret != IRLIB_SUCCESS) {
        printf("UART Communication failed (WhoAmI)\n");
        ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);
        goto fail;
    }
    printf("Device Name: %s\n", data);

    if (set_params) {
        // Auto shutter status
        ret = basic_auto_ffc_status_set(ircmd_handle, BASIC_AUTO_FFC_ENABLE);
        
        // 

        // Save parameters
        ret = basic_save_data(ircmd_handle, BASIC_SAVE_SYSTEM_PARAMETERS);
    }


    int ffc_stat;
    ret = basic_auto_ffc_status_get(ircmd_handle, &ffc_stat);
    printf("FFC STATUS: %d\n", ffc_stat);



    ret = basic_ffc_update(ircmd_handle);

    // Cleanup
    ir_control_handle->ir_control_release(ir_control_handle->ir_control_handle, (void*)(&param));
    ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);

fail:
    printf(" \n");
    printf("Cleaning up and Exiting...\n");
    if (ircmd_handle != NULL) {
        ircmd_delete_handle(ircmd_handle);
        ircmd_handle = NULL;
    }
    if (ir_control_handle != NULL) {
        iruart_handle_delete(ir_control_handle);
        ir_control_handle = NULL;
    }
    return 0;
}