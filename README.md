# c908_boot

CMU910 / lsraptor_evb 上运行在 E906 的 bootloader：

- `FLASH_PROGRAM_MODE=1`：把内嵌固件烧进 NOR flash（首次 bring-up）
- `FLASH_PROGRAM_MODE=0`：从 flash 读固件搬进 DDR，释放 C908 复位（正常引导）

## 依赖

通过 west 清单拉取（见 `west.yml`）：

- `zephyr` —— linkedsemi fork，含 `lsraptor_evb` 板级与 `lsraptor` SoC
- `LS_SDK` —— `modules/hal/linkedsemi`，SSIv2 flash / IWDG 等 HAL

## 构建

```bash
west init http://10.10.20.8:8000/houruiying/c908_boot.git c908_ws
cd c908_ws
west update
west build -b lsraptor_evb c908_boot
```

需要 xuantie riscv64 工具链在 `PATH` 中。
