# SDCard FATFS

Project nay tong hop tai lieu, schematic, video demo va cac file thu vien C de giao tiep the nho SD Card voi vi dieu khien STM32 thong qua SPI, su dung FatFs de doc/ghi file.

## Muc tieu

- Khoi tao va mount SD Card bang FatFs.
- Doc va ghi file tren the nho SD Card.
- Kiem tra dung luong tong va dung luong con trong.
- Tich hop driver SD Card cap thap voi `user_diskio.c` cua FatFs.
- In trang thai hoat dong qua UART de de debug.

## Cau truc thu muc

```text
SDCard FATFS/
|-- File thu vien/
|   |-- Ket noi thu vien FATFS voi driver phan cung/
|   |   `-- user_diskio.c
|   |-- Thu vien Driver the nho cap thap/
|   |   |-- fatfs_sd.c.c
|   |   `-- fatfs_sd.h.h
|   |-- Thu vien Giao tiep SPI/
|   |   |-- spi.c.c
|   |   `-- spi.h.h
|   |-- Thu vien Giao tiep UART/
|   |   |-- usart.c.c
|   |   `-- usart.h.h
|   |-- Thu vien su dung SD Card/
|   |   |-- sd_card.c.c
|   |   `-- sd_card.h.h
|   `-- main.c/
|       `-- Cac bien va thu vien can them vao main.pdf
|-- Kien thuc tong quat/
|   `-- Kien thuc tong quat.pdf
|-- Schematic/
|   `-- Schematic.pdf
|-- Video demo ket qua chay thuc te/
|   `-- Link_demo_kqua.docx
|-- Video demo mach tich hop/
|   |-- So do mach.pdf
|   |-- code_mach_tong.rar
|   `-- video demo ket qua.pdf
`-- Video huong dan chay/
    `-- Link_hdchay.docx
```

Luu y: Ten thu muc/file trong repo co the co dau tieng Viet. Phan cau truc ben tren da duoc viet khong dau de README hien thi on dinh tren nhieu terminal.

## Thanh phan chinh

### `fatfs_sd`

Driver cap thap de giao tiep SD Card qua SPI. File nay xu ly cac lenh SD/MMC nhu `CMD0`, `CMD8`, `CMD17`, `CMD24`, doc/ghi block 512 byte va cac lenh `ioctl` can thiet cho FatFs.

Mac dinh driver dung:

- SPI: `hspi1`
- Chan CS: `PA4`
- MCU header: `stm32f1xx_hal.h`

### `user_diskio.c`

File cau noi giua FatFs va driver phan cung. Cac ham `USER_initialize`, `USER_read`, `USER_write`, `USER_ioctl` duoc chuyen tiep sang cac ham trong `fatfs_sd`.

### `sd_card`

Thu vien muc cao de thao tac voi SD Card:

- `SD_Init_Mount()`: mount SD Card, tu format neu chua co filesystem.
- `SD_Write_File(char *filename, char *data)`: ghi them du lieu vao cuoi file.
- `SD_Read_File(char *filename)`: doc noi dung file va gui ra UART.
- `SD_Check_Capacity()`: kiem tra tong dung luong va dung luong con trong.

## Yeu cau

### Phan cung

- Vi dieu khien STM32, project hien tai dang dung HAL cho dong STM32F1.
- Module SD Card giao tiep SPI.
- The nho microSD da format FAT/FAT32.
- Ket noi UART de xem log/debug.

### Phan mem

- STM32CubeIDE hoac moi truong build STM32 HAL tuong duong.
- STM32CubeMX neu can cau hinh lai SPI, UART, GPIO va FatFs.
- FatFs middleware duoc bat trong project STM32.

## Ket noi phan cung tham khao

| SD Card Module | STM32 SPI1 |
| --- | --- |
| CS | PA4 |
| SCK | PA5 |
| MISO | PA6 |
| MOSI | PA7 |
| VCC | 3.3V |
| GND | GND |

Luu y: Neu dung chan CS khac, can sua lai macro `SD_CS_PORT` va `SD_CS_PIN` trong driver `fatfs_sd`.

## Cach tich hop vao project STM32

1. Bat SPI, UART va FatFs trong STM32CubeMX/STM32CubeIDE.
2. Cau hinh SPI o che do Master, 8-bit, CPOL Low, CPHA 1 Edge, NSS Software.
3. Them cac file thu vien vao project:
   - `fatfs_sd.c`, `fatfs_sd.h`
   - `sd_card.c`, `sd_card.h`
   - `user_diskio.c`
   - cac file `spi.c/h`, `usart.c/h` neu can tham khao cau hinh
4. Kiem tra lai ten file khi copy vao project. Trong repo nay mot so file dang co duoi mo rong lap lai nhu `.c.c` va `.h.h`; khi dua vao STM32CubeIDE nen doi ve `.c` va `.h`.
5. Dam bao `user_diskio.c` include dung file:

```c
#include "ff_gen_drv.h"
#include "fatfs_sd.h"
```

6. Trong `main.c`, goi cac ham theo thu tu:

```c
MX_GPIO_Init();
MX_SPI1_Init();
MX_USART1_UART_Init();
MX_FATFS_Init();

SD_Init_Mount();
SD_Check_Capacity();
SD_Write_File("test.txt", "Hello SD Card");
SD_Read_File("test.txt");
```

## Cau hinh SPI

Trong file SPI mau, toc do SPI dang duoc dat cham de tang do on dinh khi khoi tao the:

```c
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
```

Sau khi mount thanh cong co the tang toc do SPI neu mach va the nho hoat dong on dinh. Neu gap loi mount/doc/ghi, nen giu toc do cham de debug truoc.

## Debug UART

Thu vien `sd_card.c` gui thong bao trang thai qua UART, vi du:

- mount thanh cong/that bai
- loi ghi file
- noi dung file doc duoc
- dung luong the nho

Hay mo terminal serial dung baudrate da cau hinh trong project de xem log.

## Tai lieu va demo

- `Schematic/Schematic.pdf`: so do mach.
- Thu muc kien thuc tong quat: tai lieu nen ve SD Card/FatFs.
- Thu muc video huong dan chay: link video huong dan chay.
- Thu muc video demo ket qua chay thuc te: link video ket qua thuc te.
- Thu muc video demo mach tich hop: tai lieu va demo cho mach tich hop.

## Ghi chu

- SD Card nen dung nguon 3.3V on dinh.
- Neu module SD Card khong co chuyen muc logic, can dam bao muc tin hieu phu hop voi STM32.
- Khi loi mount, can kiem tra lai chan CS, day SPI, format the va cau hinh FatFs.
- Nen format the nho FAT32 truoc khi thu nghiem.

