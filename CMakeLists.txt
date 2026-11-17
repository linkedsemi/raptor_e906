# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(c908_boot)

# 把固件 bin 内嵌进 bootloader：objcopy 转成 .o，生成 _binary_<文件名>_start/_end/_size
# 符号并放到 .rodata。烧录模式下 flash_write 的数据源就是这些符号（数据 = 固件文件本身）。
set(EMBED_BINS
  c908_boot.bin
  u-boot.dtb
  zero_stage_boot.bin
)

foreach(bin ${EMBED_BINS})
  set(bin_obj ${CMAKE_CURRENT_BINARY_DIR}/${bin}.o)
  # WORKING_DIRECTORY 切到源码目录 + 只用文件名：objcopy 生成的符号名取决于命令行里的
  # 输入路径字符串，绝对路径会把目录也塞进符号名，这里必须用纯文件名得到 _binary_<名>_start。
  add_custom_command(
    OUTPUT ${bin_obj}
    COMMAND ${CMAKE_OBJCOPY}
            -I binary -O elf32-littleriscv -B riscv
            --rename-section .data=.rodata,alloc,load,readonly,data,contents
            ${bin} ${bin_obj}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${bin}
    COMMENT "Embedding ${bin}"
    VERBATIM
  )
  list(APPEND EMBED_OBJS ${bin_obj})
endforeach()

target_sources(app PRIVATE ${EMBED_OBJS})

target_sources(app PRIVATE
  src/main.c
  ddrmc_init/ddrmc_init.c
  ddrmc_init/ddrmc_init_train.c
  ddrmc_init/ddrmc_common.c
  ddrmc_init/ddrmc_ddr4_tmg_cfg.c
)

target_include_directories(app PRIVATE
  ddrmc_init
)
