@echo off

@echo *********************************************************************
@echo 			                AC630N SDK APP_OTA
@echo *********************************************************************
@echo %date%

cd /d %~dp0

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe
set ELFFILE=sdk.elf

REM %OBJDUMP% -D -address-mask=0x1ffffff -print-dbg $1.elf > $1.lst
%OBJCOPY% -O binary -j .text %ELFFILE% text.bin
%OBJCOPY% -O binary -j .data %ELFFILE% data.bin

%OBJDUMP% -section-headers -address-mask=0x1ffffff %ELFFILE%
%OBJDUMP% -t %ELFFILE% >  symbol_tbl.txt

copy /b text.bin+data.bin app.bin

cd tool_resource\app_ota
copy * ..\..\
cd ..\..\

json_to_res.exe json.txt

del isd_config.ini
copy isd_config_app_ota.ini isd_config.ini

isd_download.exe -tonorflash -dev bd29 -boot 0x2000 -div8 -wait 300 -uboot uboot.boot -app app.bin cfg_tool.bin -res tone.cfg config.dat -format all
@REM ��������˵��
@rem -format vm         // ����VM ����
@rem -format all        // ��������
@rem -reboot 500        // reset chip, valid in JTAG debug

del isd_config.ini

@REM ɾ����ʱ�ļ�
if exist *.mp3 del *.mp3 
if exist *.PIX del *.PIX
if exist *.TAB del *.TAB
if exist *.res del *.res
if exist *.sty del *.sty

if exist json_to_res.exe del json_to_res.exe
if exist json.txt del json.txt
if exist config.dat del config.dat
if exist isd_config_app_ota.ini del isd_config_app_ota.ini

@REM ���ɹ̼������ļ�
fw_add.exe -noenc -fw jl_isd.fw  -add ota.bin -type 100 -out jl_isd.fw
@REM ������ýű��İ汾��Ϣ�� FW �ļ���
fw_add.exe -noenc -fw jl_isd.fw -add script.ver -out jl_isd.fw

ufw_maker.exe -fw_to_ufw jl_isd.fw
copy jl_isd.ufw update.ufw
del jl_isd.ufw

@REM ���������ļ������ļ�
rem ufw_maker.exe -chip AC630N %ADD_KEY% -output config.ufw -res bt_cfg.cfg

ping /n 2 127.1>null
IF EXIST null del null

::pause
