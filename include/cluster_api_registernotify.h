#ifndef CLUSTER_API_REGISTERNOTIFY_H
#define CLUSTER_API_REGISTERNOTIFY_H

#include <stdbool.h>

typedef void(* IC_HMI_FUNC_NOTIFY_IC_HMI)(unsigned long long arg_1, IC_HMI_ON_OFF arg_2);

bool registerIcHmi(unsigned long long arg_1, IC_HMI_FUNC_NOTIFY_IC_HMI addr);

#endif // CLUSTER_API_REGISTERNOTIFY_H
