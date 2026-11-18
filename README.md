# c908_boot

## 依赖

通过 west 清单拉取（见 `west.yml`）：

- `zephyr` —— linkedsemi fork，含 `lsraptor_evb` 板级与 `lsraptor` SoC
- `LS_SDK` —— `modules/hal/linkedsemi`，SSIv2 flash / IWDG 等 HAL

## 构建

```bash
west init -m http://10.10.20.8:8000/houruiying/c908_boot.git c908_ws
cd c908_ws
west update
west build -b lsraptor_evb c908_boot
```
