#ifndef CLUSTER_API_H
#define CLUSTER_API_H

typedef enum {
    IC_HMI_OFF = 0,
    IC_HMI_ON,
} IC_HMI_ON_OFF;

#include <cluster_api_telltale.h>
#include <cluster_api_shiftposition.h>
#include <cluster_api_speed.h>
#include <cluster_api_tacho.h>
#include <cluster_api_tripcomputer.h>
#include <cluster_api_registernotify.h>

bool clusterInit(void);
bool clusterTerm(void);

// Callback function pointer type
typedef void (*IC_HMI_FUNC_NOTIFY_IC_HMI)(unsigned long long arg_1, IC_HMI_ON_OFF arg_2);

#endif // CLUSTER_API_H
