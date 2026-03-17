
#include <stdlib.h>
#include <stdio.h>
#include "libircmd.h"
#include "libiruart.h"
#include <rclcpp/rclcpp.hpp>

class ParamConfig : public rclcpp::Node {
public: 
    ParamConfig(): Node("Ir_param_config_node") {
        this->declare_parameter("set_params", true);                // if set to false, the parameters will only be displayed
        this->declare_parameter("set_shutter", true);               // set auto shutter parameters
        this->declare_parameter("set_fps", true);                   // set fps parameter
        this->declare_parameter("set_filter", true);                //set filter parameters / algorithm parameters
        this->declare_parameter("save_params",false);               //save shutter config
        
        this->declare_parameter("uart_data", "/dev/ttyHS1");        // The IR cameras USB port
        this->declare_parameter("baudrate", 115200);                //baudrate for communication channel
        
        this->declare_parameter("auto_ffc_shutter", 0);             //baudrate for communication channel
        
        this->declare_parameter("fps", 30);                         // frame rate (30 or 60)
        this->declare_parameter("image_detail_enhance_level", 50);  //detail enhance level (0-100)
        this->declare_parameter("image_noise_reduction_level", 50); //detail enhance level (0-100)
        this->declare_parameter("time_noise_reduction_level", 50);  //time noise reduction level (0-100)
        this->declare_parameter("space_noise_reduction_level", 50); //space noise reduction level (0-100)
        this->declare_parameter("edge_enhance_level", 0);           //edge enhancement level (0-100)
        this->declare_parameter("image_scene_mode", 0);             //image scene mode (0 is default mode)

        // Params for setting          
        bool p_set_params = this->get_parameter("set_params").as_bool();
        bool p_set_shutter = this->get_parameter("set_shutter").as_bool();
        bool p_set_fps = this->get_parameter("set_fps").as_bool();
        bool p_set_filter = this->get_parameter("set_filter").as_bool();
        bool p_save_params = this->get_parameter("save_params").as_bool();

        //Params for getting camera control
        std::string p_uart_data = this->get_parameter("uart_data").as_string();
        int p_baudrate = this->get_parameter("baudrate").as_int();

        //Params for shutter configuration
        int p_auto_ffc_shutter = this->get_parameter("auto_ffc_shutter").as_int();

        //Param for fps
        int p_fps = this->get_parameter("fps").as_int();

        //Params for filter/algorithm configuration
        int p_image_detail_enhance_level = this->get_parameter("image_detail_enhance_level").as_int();
        int p_image_noise_reduction_level = this->get_parameter("image_noise_reduction_level").as_int();
        int p_time_noise_reduction_level = this->get_parameter("time_noise_reduction_level").as_int();
        int p_space_noise_reduction_level = this->get_parameter("space_noise_reduction_level").as_int();
        int p_edge_enhance_level = this->get_parameter("edge_enhance_level").as_int();
        int p_image_scene_mode = this->get_parameter("image_scene_mode").as_int();
        

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
        UartConDevParams_t param = {};
        char uart_data[100];
        std::strcpy(uart_data, p_uart_data.c_str());        // Serial Port
        param.baudrate = p_baudrate;                        // Baudrate
        
        // Create control handle
        ir_control_handle_create(&ir_control_handle);

        // Create UART control handle
        iruart_handle = iruart_handle_create(ir_control_handle);
        (void)iruart_handle;

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

        // --------- PRINT CURRENT CONFIGURATION ---------
        printf("\n---------- CURRENT CONFIGURATION ----------\n");
        
        int ffc_stat;
        ret = basic_auto_ffc_status_get(ircmd_handle, &ffc_stat);
        printf("FFC STATUS: %d\n", ffc_stat);
        
        int fps;
        ret = adv_output_frame_rate_get(ircmd_handle, &fps);
        printf("FPS: %d\n", fps);

        int current_detail_enhance_level;
        ret = basic_current_detail_enhance_level_get(ircmd_handle, &current_detail_enhance_level);
        printf("DETAIL ENCHANCE LEVEL: %d\n", current_detail_enhance_level);
        
        int current_image_noise_reduction_level;
        ret = basic_current_image_noise_reduction_level_get(ircmd_handle, &current_image_noise_reduction_level);
        printf("IMAGE NOISE REDUCTION LEVEL: %d\n", current_image_noise_reduction_level);

        int current_time_noise_reduction_level;
        ret = basic_time_noise_reduce_level_get(ircmd_handle, &current_image_noise_reduction_level);
        printf("TIME NOISE REDUCTION LEVEL: %d\n", current_time_noise_reduction_level);

        int current_space_noise_reduction_level;
        ret = basic_space_noise_reduce_level_get(ircmd_handle, &current_space_noise_reduction_level);
        printf("SPACE NOISE REDUCTION LEVEL: %d\n", current_space_noise_reduction_level);

        int edge_enhance_level;
        ret = adv_edge_enhance_get(ircmd_handle, &edge_enhance_level);
        printf("EDGE ENHANCE LEVEL: %d\n", edge_enhance_level);

        int current_image_scene_mode;
        ret = basic_current_image_scene_mode_get(ircmd_handle, &current_image_scene_mode);
        printf("IMAGE SCENE MODE: %d\n", current_image_scene_mode);


        // --------- SETTING PARAMETERS ---------

        if (p_set_params) {

            // ret = adv_basic_reset_to_normal(ircmd_handle);

            printf("SETTING PARAMETERS: \n");
            if (p_set_shutter){
                // Auto Shutter Status
                printf("- Auto Shutter Status\n");
                ret = basic_auto_ffc_status_set(ircmd_handle, p_auto_ffc_shutter);
            }
            if (p_set_fps) {
                // fps (DOES NOT WORK CORRECTLY)
                printf("- FPS\n");
                ret = adv_output_frame_rate_set(ircmd_handle, p_fps);
            }
            if (p_set_filter){
                // Detail Enhance Level
                printf("- Detail Enhance Level\n");
                ret = basic_image_detail_enhance_level_set(ircmd_handle, p_image_detail_enhance_level);

                // Image Noise Reduction
                printf("- Image Noise Reduction Level\n");
                ret = basic_image_noise_reduction_level_set(ircmd_handle, p_image_noise_reduction_level);

                // Time Noise Reduction
                printf("- Time Noise Reduction Level\n");
                ret = basic_time_noise_reduce_level_set(ircmd_handle, p_time_noise_reduction_level);

                // Space Noise Reduction
                printf("- Space Noise Reduction Level\n");
                ret = basic_space_noise_reduce_level_set(ircmd_handle, p_space_noise_reduction_level);

                // Edge Enhancement Level
                // printf("- Edge Enhancement Level\n");
                // ret = adv_edge_enhance_set(ircmd_handle, p_edge_enhance_level);
                
                // Image Scene Mode
                printf("- Image Scene Mode\n");
                ret = basic_image_scene_mode_set(ircmd_handle, p_image_scene_mode);
            }

            // Printing parameters after changes
            printf("\n---------- AFTER CONFIGURATION ----------\n");
            
            int ffc_stat;
            ret = basic_auto_ffc_status_get(ircmd_handle, &ffc_stat);
            printf("FFC STATUS: %d\n", ffc_stat);
            
            int fps;
            ret = adv_output_frame_rate_get(ircmd_handle, &fps);
            printf("FPS: %d\n", fps);

            int current_detail_enhance_level;
            ret = basic_current_detail_enhance_level_get(ircmd_handle, &current_detail_enhance_level);
            printf("DETAIL ENCHANCE LEVEL: %d\n", current_detail_enhance_level);
            
            int current_image_noise_reduction_level;
            ret = basic_current_image_noise_reduction_level_get(ircmd_handle, &current_image_noise_reduction_level);
            printf("IMAGE NOISE REDUCTION LEVEL: %d\n", current_image_noise_reduction_level);

            int current_time_noise_reduction_level;
            ret = basic_time_noise_reduce_level_get(ircmd_handle, &current_image_noise_reduction_level);
            printf("TIME NOISE REDUCTION LEVEL: %d\n", current_time_noise_reduction_level);

            int current_space_noise_reduction_level;
            ret = basic_space_noise_reduce_level_get(ircmd_handle, &current_space_noise_reduction_level);
            printf("SPACE NOISE REDUCTION LEVEL: %d\n", current_space_noise_reduction_level);

            int edge_enhance_level;
            ret = adv_edge_enhance_get(ircmd_handle, &edge_enhance_level);
            printf("EDGE ENHANCE LEVEL: %d\n", edge_enhance_level);

            int current_image_scene_mode;
            ret = basic_current_image_scene_mode_get(ircmd_handle, &current_image_scene_mode);
            printf("IMAGE SCENE MODE: %d\n", current_image_scene_mode);

            if (p_set_shutter && p_save_params){
                    printf("SAVING SHUTTER CONFIGURATION\n");
                    // Save shutter parameters
                    ret = basic_save_data(ircmd_handle, BASIC_SAVE_SYSTEM_PARAMETERS);
                }
        }


        //force camera to blink/shutter once 
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
    }
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin_some(std::make_shared<ParamConfig>());
    rclcpp::shutdown();
    printf(" \n");
    return 0;
}
