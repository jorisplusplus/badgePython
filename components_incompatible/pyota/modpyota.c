#ifndef NO_QSTR
#include "esp_https_ota.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "shared/runtime/pyexec.h"
#endif

#define TAG "pyota"

static TaskHandle_t otatask = NULL;
static float s_progress = 0.0f;

static esp_err_t validate_image_header(esp_app_desc_t *new_app_info)
{
    if (new_app_info == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
    }

#ifndef CONFIG_PYOTA_SKIP_VERSION_CHECK
    if (memcmp(new_app_info->version, running_app_info.version, sizeof(new_app_info->version)) == 0) {
        ESP_LOGW(TAG, "Current running version is the same as a new. We will not continue the update.");
        return ESP_FAIL;
    }
#endif
    return ESP_OK;
}

static esp_err_t _http_client_init_cb(esp_http_client_handle_t http_client)
{
    esp_err_t err = ESP_OK;
    /* Uncomment to add custom headers to HTTP request */
    // err = esp_http_client_set_header(http_client, "Custom-Header", "Value");
    return err;
}

void advanced_ota_task(void *pvParameter)
{
    ESP_LOGI(TAG, "Starting Advanced OTA");

    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = CONFIG_OTA_WEB_SERVER,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 1000,
        .keep_alive_enable = true,
        .buffer_size_tx = 5000
    };

#ifdef CONFIG_PYOTA_SKIP_COMMON_NAME_CHECK
    config.skip_cert_common_name_check = true;
#endif

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
        otatask = NULL;
        vTaskDelete(NULL);
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        goto ota_end;
    }
    err = validate_image_header(&app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "image header verification failed");
        goto ota_end;
    }
    int totallen = esp_https_ota_get_image_size(https_ota_handle);
    while (1) {
        err = esp_https_ota_perform(https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
        int currentlen = esp_https_ota_get_image_len_read(https_ota_handle);
        s_progress = ((float) currentlen)/totallen;
        ESP_LOGD(TAG, "Image bytes read: %d", currentlen);
    }

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Complete data was not received.");
    } else {
        ota_finish_err = esp_https_ota_finish(https_ota_handle);
        if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
            ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            esp_restart();
        } else {
            if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
                ESP_LOGE(TAG, "Image validation failed, image is corrupted");
            }
            ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
            otatask = NULL;
            vTaskDelete(NULL);
        }
    }

ota_end:
    esp_https_ota_abort(https_ota_handle);
    ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed");
    otatask = NULL;
    vTaskDelete(NULL);
}

// Python bindings
STATIC mp_obj_t pyota_start() {
    if (otatask == NULL) {
        s_progress = 0.0f;
        xTaskCreate(&advanced_ota_task, "advanced_ota_task", 1024 * 8, NULL, 5, &otatask);
    } else {
        mp_raise_ValueError("OTA already started");

    }
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(pyota_start_obj, pyota_start);

STATIC mp_obj_t pyota_status() {
  return mp_obj_new_float_from_f(s_progress);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(pyota_status_obj, pyota_status);


STATIC const mp_rom_map_elem_t pyota_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&pyota_start_obj)},
    {MP_ROM_QSTR(MP_QSTR_status), MP_ROM_PTR(&pyota_status_obj)},
};

STATIC MP_DEFINE_CONST_DICT(pyota_module_globals, pyota_module_globals_table);

const mp_obj_module_t pyota_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&pyota_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_httpsota, pyota_module);