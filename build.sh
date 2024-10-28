mkdir build
cd build
cmake ..
cmake --build .
scp cli service root@ssh.jacktym.dev:~/CoflFlipServer/executable
scp client root@ssh.jacktym.dev:~/CoflFlipServer/executable/marketshark