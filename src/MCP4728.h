#ifndef __MCP4728_H__
#define __MCP4728_H__

//ch:通道位  0～3 data:输出电压 0～4095
BOOL MCP4728_DA_OUT(U8 ch,U16 vol);



BOOL MCP4728ReadDAC();


//顺序写 vol1~4:输出电压 0～4095  mv
BOOL MCP4728_DA_OUT_Sequential(U16 vol1, U16 vol2, U16 vol3, U16 vol4);








#endif//__MCP4728_H__