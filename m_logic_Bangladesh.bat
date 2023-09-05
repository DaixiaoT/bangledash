call d.bat

call core_path.bat

del src/cpu_main.o

set ProjectName=ac_v7_Bangladesh_logic
set AppDef="-DPLL_M=8 -DPLL_Q=7 -D_AC_BASE_ -D_BTIM3_ -D_CP_V7_1_ -D_UART3_ -D_UART6_ -D_UART4_ -D_RTC_1208_ -D_Bangladesh_Pro_"
set LogicPath=CP


del %ProjectName%.hex
del %ProjectName%.bin
del src\cpu_main.o

make PROJECT=%ProjectName% CORE=%COER_PATH% COMM_LIB=%COMM_PATH% APP_DEF=%AppDef% LOGIC=%LogicPath% LSCRIPT=iap.ld CPU

copy %ProjectName%.hex out\%ProjectName%.hex
copy %ProjectName%.bin out\%ProjectName%.bin

