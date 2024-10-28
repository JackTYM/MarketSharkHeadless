# Replace SERVER with your IP or Hostname and PATH with your path to MarketSharkHeadless

mkdir build
cd build
cmake ..
cmake --build .
scp cli service root@SERVER:PATH/MarketSharkServer/executable
scp client root@SERVER:PATH/MarketSharkServer/executable/marketshark
