@echo off
if "%1" == "h" goto begin
mshta vbscript:createobject("wscript.shell").run("""%~nx0""h",0)(window.close)&&exit
:begin
cd ./graphviz_lite2
set startDir=%cd%
npm run serve
