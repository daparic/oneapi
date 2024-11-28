
```
icpx -fsycl main.cpp -o intel-graphics.exe
icpx -fsycl -fsycl-targets=spir64_x86_64 main.cpp -o intel-cpu.exe
icpx -fsycl -fsycl-targets=nvptx64 main.cpp -o nvidia-graphics.exe
```
