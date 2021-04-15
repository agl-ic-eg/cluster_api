#ifndef CLUSTER_API_INTERNAL_H
#define CLUSTER_API_INTERNAL_H

#define E_CHECK(condition, value, label) \
    do { \
        if (!(condition)) { \
            printf("%s:%d ERROR:%s %s:%d\n", __func__, __LINE__, #condition, #value, (int)value); \
            goto label; \
        } \
    } while(0)

// Table with signal bits (defined to fit IPC_KIND_IC_SERVICE_E from ipc_protocol.h)
const unsigned long long gSigTable[] = {
    IC_HMI_TT_TURN_R,
    IC_HMI_TT_TURN_L,
    IC_HMI_TT_BRAKE,
    IC_HMI_TT_SEATBELT,
    IC_HMI_TT_HIGHBEAM,
    IC_HMI_TT_DOOR,
    IC_HMI_TT_EPS,
    IC_HMI_TT_SRS_AIRBAG,
    IC_HMI_TT_ABS,
    IC_HMI_TT_LOW_BATTERY,
    IC_HMI_TT_OIL_PRESS,
    IC_HMI_TT_ENGINE,
    IC_HMI_TT_FUEL,
    IC_HMI_TT_IMMOBI,
    IC_HMI_TT_TM_FAIL,
    IC_HMI_TT_ESP_ACT,
    IC_HMI_TT_ESP_OFF,
    IC_HMI_TT_ADAPTING_LIGHTING,
    IC_HMI_TT_AUTO_STOP,
    IC_HMI_TT_AUTO_STOP_FAIL,
    IC_HMI_TT_PARKING_LIGHTS,
    IC_HMI_TT_FRONT_FOG,
    IC_HMI_TT_EXTERIOR_LIGHT_FAULT,
    IC_HMI_TT_ACC_FAIL,
    IC_HMI_TT_LDW_OFF,
    IC_HMI_TT_HILL_DESCENT,
    IC_HMI_TT_AUTO_HI_BEAM_GREEN,
    IC_HMI_TT_AUTO_HI_BEAM_AMBER,
    IC_HMI_TT_LDW_OPERATE,
    IC_HMI_TT_GENERAL_WARN,
    IC_HMI_TT_SPORTS_MODE,
    IC_HMI_TT_DRIVING_POWER_MODE,
    IC_HMI_TT_HOT_TEMP,
    IC_HMI_TT_LOW_TEMP
};

#endif // CLUSTER_API_INTERNAL_H
