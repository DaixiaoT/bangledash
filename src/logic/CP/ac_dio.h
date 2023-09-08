#ifndef _AC_DIO_H_
#define _AC_DIO_H_

enum DI_DEFINE
{
    // √œº”¿≠
    DI_U1_CP1_Feedback = 0,       // HVAC 1 Compressor 1 contactor feedback
    DI_U1_CP2_Feedback,           // HVAC 1 Compressor 2 contactor feedback
    DI_U1_SF_Normal_Feedback,     // HVAC 1 Supply air fan feedback
    DI_U1_CF1_Feedback,           // HVAC 1 Condensor fan 1 feedback
    DI_U1_CF2_Feedback,           // Spare
    DI_U1_CP1_HP,                 // HVAC 1 High pressure switch 1 feedback
    DI_U1_CP2_HP,                 // HVAC 1 High pressure switch 2 feedback
    DI_U1_CP1_LP,                 // HVAC 1 Low pressure switch 1 feedback
    DI_U1_CP2_LP,                 // HVAC 1 Low pressure switch 2 feedback
    DI_U1_SF_THERMAL,             // HVAC 1 Supplier air fan thermal protection switch feedback
    DI_U1_CF12_THERMAL,           // HVAC 1 Condenser fan 1 2 thermal protection switch feedback
    DI_U1_SF_Redundancy_Feedback, // HVAC 1 Supply air fan redundancy contactor K8 feedback
    DI_U2_CP1_Feedback,           // HVAC 2 Compressor 1 contactor feedback
    DI_U2_CP2_Feedback,           // HVAC 2 Compressor 2 contactor feedback
    DI_U2_SF_Normal_Feedback,     // HVAC 2 Supply air fan feedback
    DI_U2_CF1_Feedback,           // HVAC 2 Condensor fan 1 feedback
    DI_U2_CF2_Feedback,           // HVAC 2 Condensor fan 2 contactor K27 feedback
    DI_U2_CP1_HP,                 // HVAC 2 High pressure switch 1 feedback
    DI_U2_CP2_HP,                 // HVAC 2 High pressure switch 2 feedback
    DI_U2_CP1_LP,                 // HVAC 2 Low pressure switch 1 feedback
    DI_U2_CP2_LP,                 // HVAC 2 Low pressure switch 2 feedback
    DI_U2_SF_THERMAL,             // HVAC 2 Supplier air fan thermal protection switch feedback
    DI_U2_CF12_THERMAL,           // HVAC 2 Condenser fan 12 thermal protection switch feedback
    DI_U2_SF_Redundancy_Feedback, // HVAC 2 Supply air fan redundancy contactor K8 feedback
    DI_1P_POWER_DETECTOR,         // 1P Power detector feedback
    DI_3P_POWER_DETECTOR,         // 3P Power detector feedback
    DI_MODE_SWITCH_1,             // Mode switch 1
    DI_MODE_SWITCH_2,             // Mode switch 2
    DI_MODE_SWITCH_3,             // Mode switch 3
    DI_U1_HEATER1_Feedback,       // HVAC1 Heater Contactor 1 feedback
    DI_U1_HEATER2_Feedback,       // HVAC1 Heater Contactor 2 feedback
    DI_U1_HEATER_THERMAL,         // HVAC1 Heater thermal protection
    DI_U2_HEATER1_Feedback,       // HVAC2 Heater Contactor 1 feedback
    DI_U2_HEATER2_Feedback,       // HVAC2 Heater Contactor 2 feedback
    DI_U2_HEATER_THERMAL,         // HVAC2 Heater thermal protection
    DI_U1_SMOKE_Feedback,         // HVAC1 Smoke sensor feedback
    DI_U2_SMOKE_Feedback,         // HVAC2 Smoke sensor feedback
    DI_CTRL_SMOKE_Feedback,       // Control panel Smoke sensor feedback
    DI_U1_PRESSURE_DIFF,          // HVAC 1 Pressure difference switch
    DI_U2_PRESSURE_DIFF,          // HVAC 2 Pressure difference switch
    DI_U1_Inverter_Feedback,      // HVAC 1 Inverter fault feedback
    DI_U2_Inverter_Feedback,      // HVAC 2 Inverter fault feedback
    DI_CAR_ID2,                   // Car definition2
    DI_CAR_ID3,                   // Car definition3
    DI_CAR_ID4,                   // Car definition4
    DI_NULL
};

enum DO_DEFINE
{

    DO_U1_CP1_Contactor = 0, // HVAC 1 Compressor 1 contactor
    DO_U1_CP2_Contactor,     // HVAC 1 Compressor 2 contactor
    DO_U1_HEATER1_Contactor, // HVAC 1 Heater 1 Contactor
    DO_U1_HEATER2_Contactor, // HVAC 1 Heater 2 Contactor
    DO_U1_SF_Normal,         // HVAC 1 Supply air fan
    DO_U1_CF1,               // HVAC 1 Condensor fan 1
    DO_U1_CF2,               // HVAC 1 Condensor fan 2 contactor K7
    DO_U1_SMOKE_TEST,        // HVAC 1 Smoke sensor test
    DO_U1_SF_Redundancy,     // HVAC 1 Supply air fan redundancy contactor K8
    DO_U2_CP1_Contactor,     // HVAC 2 Compressor 1 contactor
    DO_U2_CP2_Contactor,     // HVAC 2 Compressor 2 contactor
    DO_U2_HEATER1_Contactor, // HVAC 2 Heater 1 Contactor
    DO_U2_HEATER2_Contactor, // HVAC 2 Heater 2 Contactor
    DO_U2_SF_Normal,         // HVAC 2 Supply air fan
    DO_U2_CF1,               // HVAC 2 Condensor fan 1
    DO_U2_CF2,               // HVAC 2 Condensor fan 2 contactor K27
    DO_U2_SMOKE_TEST,        // HVAC 2 Smoke sensor test
    DO_U2_SF_Redundancy,     // HVAC 2 Supply air fan redundancy contactor K8
    DO_U1U2_Inverter,        // HVAC1&2 Inverter of supply air fan
    DO_CTRL_SMOKE_TEST,      // Control panel Smoke sensor test
    DO_NULL
};

#endif // _AC_DIO_H_
