clear
#make clean
make
#clear
./midas2root.exe ~/AllData/NeatData/run00018.mid
./midas2root.exe ~/AllData/NeatData/run00019.mid
./midas2root.exe ~/AllData/NeatData/run00020.mid
./midas2root.exe ~/AllData/NeatData/run00021.mid
./midas2root.exe ~/AllData/NeatData/run00022.mid

cp ./outfiles/run_csv00018.csv ../../online/
cp ./outfiles/run_csv00019.csv ../../online/
cp ./outfiles/run_csv00020.csv ../../online/
cp ./outfiles/run_csv00021.csv ../../online/
cp ./outfiles/run_csv00022.csv ../../online/
read varname
