#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>

#define UART_DEVICE_NODE DT_NODELABEL(uart0)

const struct device *uart_dev;

int main(void){
	uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);
	if (!device_is_ready(uart_dev)) {
		printk("UART device not found\n");
		return 0;
	}
    printk("HC-06 initialized. Ready for communication.\n");

    char tx_data[] = "Hello from nRF52833DK!\r\n"; // Add \r to move the cursor back to the beginning of the current line.
    for (int i = 0; i < sizeof(tx_data) - 1; i++) { // i < sizeof(tx_data) - 1 to not include the null terminator \0.
        uart_poll_out(uart_dev, tx_data[i]); // Send data from nRF board to HC-06.
    }

    while (1) {
		char rx_data;
		int ret;
		ret = uart_poll_in(uart_dev, &rx_data);
		if (ret == 0) { // Send data from HC-06 to nRF board.
			printk("Received: %c\n", rx_data);
		} else if (ret == -1) {
			printk("Waiting for data...\n");
		} else {
			printk("Failed to receive data\n");
		}
		
		k_sleep(K_MSEC(1000));
	}
}