# SDCard FATFS

Project này tổng hợp tài liệu, schematic, video demo và các file thư viện C để giao tiếp thẻ nhớ SD Card với vi điều khiển STM32 thông qua SPI, sử dụng FatFs để đọc/ghi file.

## Mục Tiêu

- Khởi tạo và mount SD Card bằng FatFs.
- Đọc và ghi file trên thẻ nhớ SD Card.
- Kiểm tra tổng dung lượng và dung lượng còn trống.
- Tích hợp driver SD Card cấp thấp với `user_diskio.c` của FatFs.
- In trạng thái hoạt động qua UART để dễ debug.

## Cấu Trúc Thư Mục

```text
SDCard FATFS/
|-- File thư viện/
|   |-- Kết nối thư viện FATFS với driver phần cứng/
|   |   `-- user_diskio.c
|   |-- Thư viện Driver thẻ nhớ cấp thấp/
|   |   |-- fatfs_sd.c.c
|   |   `-- fatfs_sd.h.h
|   |-- Thư viện Giao tiếp SPI/
|   |   |-- spi.c.c
|   |   `-- spi.h.h
|   |-- Thư viện Giao tiếp UART/
|   |   |-- usart.c.c
|   |   `-- usart.h.h
|   |-- Thư viện sử dụng SD Card/
|   |   |-- sd_card.c.c
|   |   `-- sd_card.h.h
|   `-- main.c/
|       `-- Các biến và thư viện cần thêm vào main.pdf
|-- Kiến thức tổng quát/
|   `-- Kiến thức tổng quát.pdf
|-- Schematic/
|   `-- Schematic.pdf
|-- Video demo kết quả chạy thực tế/
|   `-- Link_demo_kqua.docx
|-- Video demo mạch tích hợp/
|   |-- Sơ đồ mạch.pdf
|   |-- code_mạch_tổng.rar
|   `-- video demo kết quả.pdf
`-- Video hướng dẫn chạy/
    `-- Link_hdchay.docx
```

Lưu ý: Một số tên thư mục/file trong repo có dấu tiếng Việt. Nếu terminal hiển thị lỗi mã hóa, hãy xem trực tiếp trên GitHub hoặc mở bằng trình soạn thảo hỗ trợ UTF-8.

## Thành Phần Chính

### `fatfs_sd`

Driver cấp thấp để giao tiếp SD Card qua SPI. File này xử lý các lệnh SD/MMC như `CMD0`, `CMD8`, `CMD17`, `CMD24`, đọc/ghi block 512 byte và các lệnh `ioctl` cần thiết cho FatFs.

Mặc định driver sử dụng:

- SPI: `hspi1`
- Chân CS: `PA4`
- MCU header: `stm32f1xx_hal.h`

### `user_diskio.c`

File cầu nối giữa FatFs và driver phần cứng. Các hàm `USER_initialize`, `USER_read`, `USER_write`, `USER_ioctl` được chuyển tiếp sang các hàm trong `fatfs_sd`.

### `sd_card`

Thư viện mức cao để thao tác với SD Card:

- `SD_Init_Mount()`: mount SD Card, tự format nếu chưa có filesystem.
- `SD_Write_File(char *filename, char *data)`: ghi thêm dữ liệu vào cuối file.
- `SD_Read_File(char *filename)`: đọc nội dung file và gửi ra UART.
- `SD_Check_Capacity()`: kiểm tra tổng dung lượng và dung lượng còn trống.

## Yêu Cầu

### Phần Cứng

- Vi điều khiển STM32, project hiện tại đang dùng HAL cho dòng STM32F1.
- Module SD Card giao tiếp SPI.
- Thẻ nhớ microSD đã format FAT/FAT32.
- Kết nối UART để xem log/debug.

### Phần Mềm

- STM32CubeIDE hoặc môi trường build STM32 HAL tương đương.
- STM32CubeMX nếu cần cấu hình lại SPI, UART, GPIO và FatFs.
- FatFs middleware được bật trong project STM32.

## Kết Nối Phần Cứng Tham Khảo

| SD Card Module | STM32 SPI1 |
| --- | --- |
| CS | PA4 |
| SCK | PA5 |
| MISO | PA6 |
| MOSI | PA7 |
| VCC | 3.3V |
| GND | GND |

Lưu ý: Nếu dùng chân CS khác, cần sửa lại macro `SD_CS_PORT` và `SD_CS_PIN` trong driver `fatfs_sd`.

## Cách Tích Hợp Vào Project STM32

1. Bật SPI, UART và FatFs trong STM32CubeMX/STM32CubeIDE.
2. Cấu hình SPI ở chế độ Master, 8-bit, CPOL Low, CPHA 1 Edge, NSS Software.
3. Thêm các file thư viện vào project:
   - `fatfs_sd.c`, `fatfs_sd.h`
   - `sd_card.c`, `sd_card.h`
   - `user_diskio.c`
   - các file `spi.c/h`, `usart.c/h` nếu cần tham khảo cấu hình
4. Kiểm tra lại tên file khi copy vào project. Trong repo này một số file đang có đuôi mở rộng lặp lại như `.c.c` và `.h.h`; khi đưa vào STM32CubeIDE nên đổi về `.c` và `.h`.
5. Đảm bảo `user_diskio.c` include đúng file:

```c
#include "ff_gen_drv.h"
#include "fatfs_sd.h"
```

6. Trong `main.c`, gọi các hàm theo thứ tự:

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

## Cấu Hình SPI

Trong file SPI mẫu, tốc độ SPI đang được đặt chậm để tăng độ ổn định khi khởi tạo thẻ:

```c
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
```

Sau khi mount thành công có thể tăng tốc độ SPI nếu mạch và thẻ nhớ hoạt động ổn định. Nếu gặp lỗi mount/đọc/ghi, nên giữ tốc độ chậm để debug trước.

## Debug UART

Thư viện `sd_card.c` gửi thông báo trạng thái qua UART, ví dụ:

- mount thành công/thất bại
- lỗi ghi file
- nội dung file đọc được
- dung lượng thẻ nhớ

Hãy mở terminal serial đúng baudrate đã cấu hình trong project để xem log.

## Tài Liệu Và Demo

- `Schematic/Schematic.pdf`: sơ đồ mạch.
- Thư mục kiến thức tổng quát: tài liệu nền về SD Card/FatFs.
- Thư mục video hướng dẫn chạy: link video hướng dẫn chạy.
- Thư mục video demo kết quả chạy thực tế: link video kết quả thực tế.
- Thư mục video demo mạch tích hợp: tài liệu và demo cho mạch tích hợp.

## Ghi Chú

- SD Card nên dùng nguồn 3.3V ổn định.
- Nếu module SD Card không có chuyển mức logic, cần đảm bảo mức tín hiệu phù hợp với STM32.
- Khi lỗi mount, cần kiểm tra lại chân CS, dây SPI, format thẻ và cấu hình FatFs.
- Nên format thẻ nhớ FAT32 trước khi thử nghiệm.

