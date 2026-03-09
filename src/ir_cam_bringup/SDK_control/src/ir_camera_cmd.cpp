
#include <stdlib.h>
#include <stdio.h>
#include "libircmd.h"
#include "libiruart.h"
#include <rclcpp/rclcpp.hpp>

class ParamConfig : public rclcpp::Node
{

    public: ParamConfig(): Node("Ir_param_config_node")
    {

        // --------- READ AND GET PARAMETERS ---------

        this->declare_parameter("set_params", false); // if set to false, the parameters will only be displayed
        this->declare_parameter("set_shutter", false); // set auto shutter parameters
        this->declare_parameter("set_filter", false); //set filter parameters / algorithm parameters
        this->declare_parameter("save_shutter",false); //save shutter config
        this->declare_parameter("save_filter",false); //save filter config
        this->declare_parameter("uart_data", "/dev/ttyUSB0"); // The IR cameras USB port
        this->declare_parameter("baudrate", 115200); //baudrate for communication channel
        this->declare_parameter("auto_ffc_shutter", 0); //baudrate for communication channel
        // this->declare_parameter("auto_ffc_shutter", 0); //baudrate for communication channel

        // Params for setting          
        bool p_set_params = this->get_parameter("set_params").as_bool();
        bool p_set_shutter = this->get_parameter("set_shutter").as_bool();
        bool p_set_filter = this->get_parameter("set_filter").as_bool();
        bool p_save_shutter = this->get_parameter("save_shutter").as_bool();
        bool p_save_filter = this->get_parameter("save_filter").as_bool();

        //Params for getting camera control
        std::string p_uart_data = this->get_parameter("uart_data").as_string();
        int p_baudrate = this->get_parameter("baudrate").as_int();

        //Params for shutter configuration
        int p_auto_ffc_shutter = this->get_parameter("auto_ffc_shutter").as_int();

        //Params for filter/algorithm configuration



        // --------- RUN CODE (FOR UART CONTROL) ---------

        // Register library logs and set log level to ERROR
        ircam_log_register(IRCAM_LOG_ERROR, NULL, NULL);
        ircmd_log_register(IRCMD_LOG_ERROR, NULL, NULL);
        iruart_log_register(IRUART_LOG_ERROR, NULL, NULL);

        // All handles used
        IrControlHandle_t* ir_control_handle = NULL;
        IruartHandle_t* iruart_handle = NULL;
        IrcmdHandle_t* ircmd_handle = NULL;

        int ret = IRLIB_SUCCESS;

        // Serial port parameters (Read from params above)
        UartConDevParams_t param = {0};
        // char uart_data[20] = {*p_uart_data.c_str()};        // Serial Port
        char uart_data[20] = "/dev/ttyUSB0";        // Serial Port
        param.baudrate = p_baudrate;             // Baudrate
        

        // Create control handle
        ir_control_handle_create(&ir_control_handle);

        // Create UART control handle
        iruart_handle = iruart_handle_create(ir_control_handle);

        // Open and initialize device
        ret = ir_control_handle->ir_control_open(ir_control_handle->ir_control_handle, (void*)(&uart_data));
        if (ret != IRLIB_SUCCESS) {
            printf("Device Open Failed! Return Error (ret) is: %d\n", ret);
            // goto fail;
        }
        printf("Deviced opened succesfully...\n");
        ret = ir_control_handle->ir_control_init(ir_control_handle->ir_control_handle, (void*)(&param));
        if (ret != IRLIB_SUCCESS) {
            printf("Device Initialization Failed! Return Error (ret) is: %d\n", ret);
            ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);
            // goto fail;
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
            // goto fail;
        }
        printf("Device Name: %s\n", data);

        // --------- PRINT CURRENT CONFIGURATION ---------
        printf("\n---------- CURRENT CONFIGURATION ----------\n");
        int ffc_stat;
        ret = basic_auto_ffc_status_get(ircmd_handle, &ffc_stat);
        printf("FFC STATUS: %d\n", ffc_stat);



        // --------- SETTING PARAMETERS ---------

        if (p_set_params) {
            printf("SETTING PARAMETERS: \n");
            if (p_set_shutter){


                // Auto shutter status
                printf("- Auto Shutter Status\n");
                ret = basic_auto_ffc_status_set(ircmd_handle, p_auto_ffc_shutter);
            }
            if (p_set_filter){
                // EXAMPLE Filter Param 1
                printf("- Filter Param 1\n");
                //ret = param_1_set(ircmd_handle, p_param_1);
            }


            if (p_set_shutter && p_save_shutter){
                    printf("SAVING SHUTTER CONFIGURATION\n");
                    // Save shutter parameters
                    ret = basic_save_data(ircmd_handle, BASIC_SAVE_SYSTEM_PARAMETERS);
                }
            
            if (p_set_filter && p_save_filter){
                printf("SAVING FILTER/ALGO CONFIGURATION\n");
                    // Save filter/algorithm parameters
                    ret = basic_save_data(ircmd_handle, BASIC_SAVE_ALGORITHM_PARAMETERS);
            }
            
            // Printing parameters after changes
        }


        //force camera to blink/shutter once 
        ret = basic_ffc_update(ircmd_handle);

        // Cleanup
        ir_control_handle->ir_control_release(ir_control_handle->ir_control_handle, (void*)(&param));
        ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);

        // fail:
        //     printf(" \n");
        //     printf("Cleaning up and Exiting...\n");
        //     if (ircmd_handle != NULL) {
        //         ircmd_delete_handle(ircmd_handle);
        //         ircmd_handle = NULL;
        //     }
        //     if (ir_control_handle != NULL) {
        //         iruart_handle_delete(ir_control_handle);
        //         ir_control_handle = NULL;
        //     }
    }
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin_some(std::make_shared<ParamConfig>());
    rclcpp::shutdown();
    printf(" \n");
    return 0;
}










// #include <stdlib.h>
// #include <stdio.h>
// #include "libircmd.h"
// #include "libiruart.h"

// int main(int argc, char* argv[]) {

//     bool set_params = true;

//     // Register library logs and set log level to ERROR
//     ircam_log_register(IRCAM_LOG_ERROR, NULL, NULL);
//     ircmd_log_register(IRCMD_LOG_ERROR, NULL, NULL);
//     iruart_log_register(IRUART_LOG_ERROR, NULL, NULL);

//     // All handles used
//     IrControlHandle_t* ir_control_handle = NULL;
//     IruartHandle_t* iruart_handle = NULL;
//     IrcmdHandle_t* ircmd_handle = NULL;

//     int ret = IRLIB_SUCCESS;

//     // Serial port parameters (Should be read from config)
//     UartConDevParams_t param = {0};
//     char uart_data[20] = "/dev/ttyUSB0";        // Serial Port
//     param.baudrate = 115200;                    // Baudrate

//     // Create control handle
//     ir_control_handle_create(&ir_control_handle);

//     // Create UART control handle
//     iruart_handle = iruart_handle_create(ir_control_handle);

//     // Open and initialize device
//     ret = ir_control_handle->ir_control_open(ir_control_handle->ir_control_handle, (void*)(&uart_data));
//     if (ret != IRLIB_SUCCESS) {
//         printf("Device Open Failed! Return Error (ret) is: %d\n", ret);
//         goto fail;
//     }
//     printf("Deviced opened succesfully...\n");
//     ret = ir_control_handle->ir_control_init(ir_control_handle->ir_control_handle, (void*)(&param));
//     if (ret != IRLIB_SUCCESS) {
//         printf("Device Initialization Failed! Return Error (ret) is: %d\n", ret);
//         ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);
//         goto fail;
//     }
//     printf("Device initialized succesfully...\n");

//     // Create command handle
//     ircmd_handle = ircmd_create_handle(ir_control_handle);

//     // --------- RUN START UP COMMANDS FOR THE IRCAM ---------
//     // Get device name (WhoAmI)
//     char data[100];
//     ret = basic_device_info_get(ircmd_handle, BASIC_DEV_NAME, data);
//     if (ret != IRLIB_SUCCESS) {
//         printf("UART Communication failed (WhoAmI)\n");
//         ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);
//         goto fail;
//     }
//     printf("Device Name: %s\n", data);
    
//     if (set_params) {
//         // Auto shutter status
//         ret = basic_auto_ffc_status_set(ircmd_handle, BASIC_AUTO_FFC_ENABLE);
        
//         // 

//         // Save parameters
//         ret = basic_save_data(ircmd_handle, BASIC_SAVE_SYSTEM_PARAMETERS);
//     }


//     int ffc_stat;
//     ret = basic_auto_ffc_status_get(ircmd_handle, &ffc_stat);
//     printf("FFC STATUS: %d\n", ffc_stat);



//     ret = basic_ffc_update(ircmd_handle);

//     // Cleanup
//     ir_control_handle->ir_control_release(ir_control_handle->ir_control_handle, (void*)(&param));
//     ir_control_handle->ir_control_close(ir_control_handle->ir_control_handle);

// fail:
//     printf(" \n");
//     printf("Cleaning up and Exiting...\n");
//     if (ircmd_handle != NULL) {
//         ircmd_delete_handle(ircmd_handle);
//         ircmd_handle = NULL;
//     }
//     if (ir_control_handle != NULL) {
//         iruart_handle_delete(ir_control_handle);
//         ir_control_handle = NULL;
//     }
//     return 0;
// }