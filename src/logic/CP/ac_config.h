#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__
#include "ac_include.h"

// 控制器全局配置参数
#pragma pack(push, 1)
struct AC_PARM
{
    U32 mark;
    U8 CarNo;
    U16 FDampOpenTime_s;              // 新风阀全开时间(0.1秒)
    U16 FDampOpenCloseTimeAW0_s;      // 新风阀 全开->1档时间(0.1秒)
    U16 FDampOpenCloseTimeAW2_s;      // 新风阀 全开->2档时间(0.1秒)
    U16 FDampOpenCloseTimeStroVent_s; // 新风阀 全开->3档时间(0.1秒)

    U16 exhaustAOpenTime_s;              // 废排风阀A全开时间(0.1秒)
    U16 exhaustAOpenCloseTimeAW0_s;      // 废排风阀A 全开->1档时间(0.1秒)
    U16 exhaustAOpenCloseTimeAW2_s;      // 废排风阀A全开->2档时间(0.1秒)
    U16 exhaustAOpenCloseTimeStroVent_s; // 废排风阀A 全开->3档时间(0.1秒)

    U16 exhaustBCDOpenTime_s;              // 废排风阀BCD全开时间(0.1秒)
    U16 exhaustBCDOpenCloseTimeAW0_s;      // 废排风阀BCD 全开->1档时间(0.1秒)
    U16 exhaustBCDOpenCloseTimeAW2_s;      // 废排风阀BCD全开->2档时间(0.1秒)
    U16 exhaustBCDOpenCloseTimeStroVent_s; // 废排风阀BCD 全开->3档时间(0.1秒)

    U16 RDampOpenTime_s;              // 回风阀全开时间(0.1秒)
    U16 RDampOpenCloseTimeAW0_s;      // 回风阀 全开->1档时间(0.1秒)
    U16 RDampOpenCloseTimeAW2_s;      // 回风阀 全开->2档时间(0.1秒)
    U16 RDampOpenCloseTimeStroVent_s; // 回风阀 全开->3档时间(0.1秒)

    U8 CarLoad_AW2; // 0->1档 百分比

    U8 VentCondensorStartGap_s;        // 通风机启动->冷凝风机启动(秒)（13）
    U8 CondensorCompressorStartGap_s;  // 冷凝风机启动->压缩机启动(秒)（14）
    U8 CompressorCompressorStartGap_s; // 压缩机和压缩机启动时间间隔
    U8 VentHeatStartGap_s;             // 通风机和电加热启动时间间隔
    U8 CompressorCondensorStopGap_s;   // 压缩机停机到冷凝风机停机的时间间隔
    U8 VentHeatStopGap_s;              // 通风机和电加热停止时间间隔
    U8 EmPowerDelayTime_s;             // 紧急逆变器延时启动时间
    U8 EmVentKeepTime_m;               // 紧急通风状态保持时间，45分钟

    U16 CompressorStartStopGap_s; // 压缩机启动到停止的最短时间
    U16 CompressorStopStartGap_s; // 压缩机停止到启动的最短时间
    U16 CoolTargetTemp;           // 制冷模式目标温度(度)（22）0.1度
    U16 gInitCoolHeatKeepTime_m;  // 预冷最大时间(分钟)（24）
    U16 CompressorStartLPGap_s;   // 压缩机启动到检测低压的最短时间
    S16 Tf_adjust;                // 新风温度校准标定
    S16 Tr_adjust;                // 回风温度校准标定
    U16 HP_adjust;                // 高压压力传感器校准标定
    U16 LP_adjust;                // 低压压力传感器校准标定

    U8 FDampForceOpen; // 新风阀和废排风阀强制打开

    U8 ModeSwitchDelay_s;
    U8 TouchScreenDelay_s;
    U8 SelftestModeOffsetTime_s;
    U8 SelftestModeRandomTime_s;
    U16 SelftestEndToCoolTime_s; // Starting time of Cooling mode after selftesting
    U16 DiagnoseModeFreshTempLimit_t;
    U16 DiagnoseModeReturnTempLimit_t;

    U8 DiagnoseModeSupplyFanStartDelay_s;
    U8 DiagnoseModeHeatersStartDelay_s;
    U8 DiagnoseModeHeatersStopDelay_s;
    U8 DiagnoseModeCondenserStartDelay_s;
    U8 DiagnoseModeCompressor11StartDelay_s;
    U8 DiagnoseModeCompressor12StartDelay_s;
    U8 DiagnoseModeCompressor21StartDelay_s;
    U16 DiagnoseModeCompressor22StartDelay_s;
    U8 DiagnoseModeCompressor11StopDelay_s;
    U8 DiagnoseModeCompressor12StopDelay_s;
    U8 DiagnoseModeCompressor21StopDelay_s;

    U16 AutoModeOneCompressorTempOffset_t;
    U16 AutoModeTwoCompressorTempOffset_t;
    U16 AutoModeOneHeatTempOffset_t;
    U16 AutoModeTwoHeatTempOffset_t;
    U16 res;

    U16 CoolHeatSwitchVentTime_s;

    U8 TouchScreenCommTimeout_s;
    U8 PowerMeterCommTimeout_s;
    U8 SensorErrorDelay_s;

    U8 ComressorContactorErrTrigger;
    U16 ComressorContactorErrDelay;

    U8 ComressorHPErrTrigger;
    U16 ComressorHpErrDelay;

    U8 ComressorLPErrTrigger;
    U16 ComressorLPErrDelay;

    U8 ComressorCurrentErrTrigger;
    U16 ComressorCurrentErrDelay;

    U8 HeaterContactorErrTrigger;
    U16 HeaterContactorErrDelay;

    U8 HeaterOverheatErrTrigger;
    U16 HeaterOverheatErrDelay;

    U8 SupplyFanContactorErrTrigger;
    U16 SupplyFanContactorErrDelay;

    U8 CondensorFanContactorErrTrigger;
    U16 CondensorFanContactorErrDelay;

    U16 Compressor_current_Max1;
    U16 Compressor_current_Min1;
    U16 Compressor_current_Max2;
    U16 Compressor_current_Min2;
    U16 Compressor_error_Waiting_time;
    U16 Heater_error_Waiting_time;
    U16 Condensor_error_Waiting_time;
    U16 Supply_error_Waiting_time;
    U16 Temperature_setpoint;
    U16 Overheat_ventilation_temperature;
    U16 Overheat_ventilation_continuously;
    U16 Overheat_ventilation_Restore_temperature;
    U16 Overheat_ventilation_Restore_temperature_continuously;
    U16 Overheat_off_temperature;
    U16 Overheat_off_continuously;
    U16 Overheat_off_Restore_temperature;
    U16 Overheat_off_Restore_temperature_continuously;
    U8 Supply_run_to_condenser_run_waiting_time;
    U8 Condenser_run_to_Compressor_run_waiting_time;
    U8 Compressor_run_to_Compressor_run_waiting_time;
    U16 Compressor_min_running_time;
    U16 Compressor_min_stopping_time;
    U8 Min_off_time_of_other_devices;
    U8 Supply_run_to_heaters_run_waiting_time;
    U16 Max_supply_air_temperature;
    U16 Max_supply_air_temperature_Restore;
    U16 Supply_air_fan_running_time_after_off;
    U8 Full_cool_temp_parm_A;  // 全冷状态根据温度判断是否开两个压缩机  TS*a+Tr*b<c
    U8 Full_cool_temp_parm_B;  // 全冷状态根据温度判断是否开两个压缩机
    U16 Full_cool_temp_parm_C; // 全冷状态根据温度判断是否开两个压缩机
    U16 crc;
};
#pragma pack(pop)


#define DI_NUM_MAINBOARD (76)
#define DO_NUM_MAINBOARD (48)
#define AI_NUM_MAINBOARD (6)

#define DI_NUM (DI_NUM_MAINBOARD)
#define DO_NUM (DO_NUM_MAINBOARD)
#define AI_NUM (AI_NUM_MAINBOARD)

#define DI_BYTES ((DI_NUM + 7) >> 3)
#define DO_BYTES ((DO_NUM + 7) >> 3)

extern AC_PARM g_parm;

#endif // !__AC_CONFIG_H__
