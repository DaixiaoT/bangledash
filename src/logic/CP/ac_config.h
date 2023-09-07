#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__
#include "ac_include.h"

// ������ȫ�����ò���
#pragma pack(push, 1)
struct AC_PARM
{
    U32 mark;
    U8 CarNo;
    U16 FDampOpenTime_s;              // �·緧ȫ��ʱ��(0.1��)
    U16 FDampOpenCloseTimeAW0_s;      // �·緧 ȫ��->1��ʱ��(0.1��)
    U16 FDampOpenCloseTimeAW2_s;      // �·緧 ȫ��->2��ʱ��(0.1��)
    U16 FDampOpenCloseTimeStroVent_s; // �·緧 ȫ��->3��ʱ��(0.1��)

    U16 exhaustAOpenTime_s;              // ���ŷ緧Aȫ��ʱ��(0.1��)
    U16 exhaustAOpenCloseTimeAW0_s;      // ���ŷ緧A ȫ��->1��ʱ��(0.1��)
    U16 exhaustAOpenCloseTimeAW2_s;      // ���ŷ緧Aȫ��->2��ʱ��(0.1��)
    U16 exhaustAOpenCloseTimeStroVent_s; // ���ŷ緧A ȫ��->3��ʱ��(0.1��)

    U16 exhaustBCDOpenTime_s;              // ���ŷ緧BCDȫ��ʱ��(0.1��)
    U16 exhaustBCDOpenCloseTimeAW0_s;      // ���ŷ緧BCD ȫ��->1��ʱ��(0.1��)
    U16 exhaustBCDOpenCloseTimeAW2_s;      // ���ŷ緧BCDȫ��->2��ʱ��(0.1��)
    U16 exhaustBCDOpenCloseTimeStroVent_s; // ���ŷ緧BCD ȫ��->3��ʱ��(0.1��)

    U16 RDampOpenTime_s;              // �ط緧ȫ��ʱ��(0.1��)
    U16 RDampOpenCloseTimeAW0_s;      // �ط緧 ȫ��->1��ʱ��(0.1��)
    U16 RDampOpenCloseTimeAW2_s;      // �ط緧 ȫ��->2��ʱ��(0.1��)
    U16 RDampOpenCloseTimeStroVent_s; // �ط緧 ȫ��->3��ʱ��(0.1��)

    U8 CarLoad_AW2; // 0->1�� �ٷֱ�

    U8 VentCondensorStartGap_s;        // ͨ�������->�����������(��)��13��
    U8 CondensorCompressorStartGap_s;  // �����������->ѹ��������(��)��14��
    U8 CompressorCompressorStartGap_s; // ѹ������ѹ��������ʱ����
    U8 VentHeatStartGap_s;             // ͨ����͵��������ʱ����
    U8 CompressorCondensorStopGap_s;   // ѹ����ͣ�����������ͣ����ʱ����
    U8 VentHeatStopGap_s;              // ͨ����͵����ֹͣʱ����
    U8 EmPowerDelayTime_s;             // �����������ʱ����ʱ��
    U8 EmVentKeepTime_m;               // ����ͨ��״̬����ʱ�䣬45����

    U16 CompressorStartStopGap_s; // ѹ����������ֹͣ�����ʱ��
    U16 CompressorStopStartGap_s; // ѹ����ֹͣ�����������ʱ��
    U16 CoolTargetTemp;           // ����ģʽĿ���¶�(��)��22��0.1��
    U16 gInitCoolHeatKeepTime_m;  // Ԥ�����ʱ��(����)��24��
    U16 CompressorStartLPGap_s;   // ѹ��������������ѹ�����ʱ��
    S16 Tf_adjust;                // �·��¶�У׼�궨
    S16 Tr_adjust;                // �ط��¶�У׼�궨
    U16 HP_adjust;                // ��ѹѹ��������У׼�궨
    U16 LP_adjust;                // ��ѹѹ��������У׼�궨

    U8 FDampForceOpen; // �·緧�ͷ��ŷ緧ǿ�ƴ�

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
    U8 Full_cool_temp_parm_A;  // ȫ��״̬�����¶��ж��Ƿ�����ѹ����  TS*a+Tr*b<c
    U8 Full_cool_temp_parm_B;  // ȫ��״̬�����¶��ж��Ƿ�����ѹ����
    U16 Full_cool_temp_parm_C; // ȫ��״̬�����¶��ж��Ƿ�����ѹ����
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
