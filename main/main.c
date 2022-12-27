#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/mcpwm_prelude.h"
#include "sdkconfig.h"

static const char *TAG = "h-bridge";

// Set the GPIO pin numbers for the H-bridge inputs
#define PIN_PWM_IN_DVR_1_A_1 18
#define PIN_PWM_IN_DVR_1_A_2 18
#define PIN_PWM_IN_DVR_1_B_1 19
#define PIN_PWM_IN_DVR_1_B_2 19
#define PIN_PWM_IN_DVR_2_A_1 18
#define PIN_PWM_IN_DVR_2_A_2 18
#define PIN_PWM_IN_DVR_2_B_1 19
#define PIN_PWM_IN_DVR_2_B_2 19

// Set the frequency and duty cycle for the PWM signal
#define PWM_FREQUENCY 1000
#define PWM_DUTY_CYCLE 50

void app_main()
{
    // Configure the pwm timer
    ESP_LOGI(TAG, "SETTING PWM TIMER");
    const mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = SOC_MCPWM_SUPPORTED,
        .resolution_hz = PWM_FREQUENCY,
        .count_mode = MCPWM_TIMER_DIRECTION_UP,
        .period_ticks = 1000,
        .flags = {
            .update_period_on_empty = 1,
            .update_period_on_sync = 1,
        }
    };
    
    // Initialize the pwm timer
    ESP_LOGI(TAG, "INITIALIZING PWM TIMER");
    mcpwm_timer_handle_t timer_handle;
    esp_err_t timer_error_status =  mcpwm_new_timer(&timer_config, &timer_handle);
    if (timer_error_status != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing timer: %d", timer_error_status);
        return;
    }

    // Configure the pwm operator
    ESP_LOGI(TAG, "SETTING PWM OPERATOR");
    const mcpwm_operator_config_t operator_config = {
        .group_id = 0,
        .flags = {
            .update_dead_time_on_tez = 1,
            .update_dead_time_on_tep = 1,
            .update_dead_time_on_sync = 1,
            .update_gen_action_on_tez = 1,
            .update_gen_action_on_tep = 1,
            .update_gen_action_on_sync = 1,
        }
    };

    // Initialize the pwm operator
    ESP_LOGI(TAG, "INITIALIZING PWM OPERATOR");
    mcpwm_oper_handle_t operator_handle;
    esp_err_t operator_error_status = mcpwm_new_operator(&operator_config, &operator_handle);
    if (operator_error_status != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing operator: %d", operator_error_status);
        return;
    }

    // Configure the pwm comparator
    ESP_LOGI(TAG, "SETTING PWM COMPARATOR");
    const mcpwm_comparator_config_t comparator_config = {
        .flags = {
            .update_cmp_on_tez = 1,
            .update_cmp_on_tep = 1,
            .update_cmp_on_sync = 1,
        }
    };

    // Initialize the pwm comparator
    ESP_LOGI(TAG, "INITIALIZING PWM COMPARATOR");
    mcpwm_cmpr_handle_t comparator_handle;
    esp_err_t comparator_error_status = mcpwm_new_comparator(&operator_handle, &comparator_config, &comparator_handle);
    if (comparator_error_status != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing comparator: %d", comparator_error_status);
        return;
    }

    // Configure the pwm generator for PIN_PWM_IN_DVR_1_A_1
    ESP_LOGI(TAG, "SETTING PWM GENERATOR FOR PIN_PWM_IN_DVR_1_A_1");
    const mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = PIN_PWM_IN_DVR_1_A_1,
        .flags = {
            .invert_pwm = 0,
            .io_loop_back = 0,
        }
    };

    // Initialize the pwm generator for PIN_PWM_IN_DVR_1_A_1
    ESP_LOGI(TAG, "INITIALIZING PWM GENERATOR FOR PIN_PWM_IN_DVR_1_A_1");
    mcpwm_gen_handle_t generator_handle;
    esp_err_t generator_error_status = mcpwm_new_generator(operator_handle, &generator_config, &generator_handle);
    if (generator_error_status != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing generator: %d", generator_error_status);
        return;
    }

}