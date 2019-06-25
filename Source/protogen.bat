@echo off
set protocRoot=.\MCU\maincontroller\lib\nanopb\generator-bin\
set includeRoot=.\MCU\maincontroller\lib\nanopb\generator\
set outputDir=.\MCU\maincontroller\src\proto\
%protocRoot%\protoc -I. -I%includeRoot% -I%includeRoot%\proto --nanopb_out %outputDir% FCubed.proto