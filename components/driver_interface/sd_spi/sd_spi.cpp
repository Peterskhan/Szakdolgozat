#include "sd_spi.h"

SD_SPI::SD_SPI(uint8_t miso_pin, uint8_t mosi_pin, uint8_t sck_pin, uint8_t cs_pin)
	: DriverInterface(Type::SD_SPI), m_card(nullptr), m_driveName(""), m_isMounted(false), m_cs_pin(cs_pin)
{
	// Driver configuration for SD_SPI transactions
	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	m_host = host;

	// SPI bus configuration
	spi_bus_config_t bus_cfg = {
		.mosi_io_num = mosi_pin,
		.miso_io_num = miso_pin,
		.sclk_io_num = sck_pin,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 4000,
		.flags = 0,
		.intr_flags = 0
	};

	// Initializing SPI bus for SD_SPI transactions
	spi_bus_initialize((spi_host_device_t) m_host.slot, &bus_cfg, 1);
}

SD_SPI::~SD_SPI()
{
	// Unmounting the SD card if mounted
	if(m_isMounted) unmount();

	// Releasing SPI bus resources
	spi_bus_free((spi_host_device_t) m_host.slot);
}

bool SD_SPI::mount(const std::string& driveName)
{
	// Configuration for the FATFS filesystem for mounting the SD card
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
	    .format_if_mount_failed = true,
		.max_files = 10,
		.allocation_unit_size = 16 * 1024
	};

	// SPI configuration for the SD card interface
	sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
	slot_config.gpio_cs = (gpio_num_t) m_cs_pin;
	slot_config.host_id = (spi_host_device_t) m_host.slot;

	// Mounting the SD card to the FATFS filesystem
	return (esp_vfs_fat_sdspi_mount(driveName.c_str(), &m_host, &slot_config, &mount_config, &m_card) == ESP_OK);
}

bool SD_SPI::unmount()
{
	// Unmounting the SD card from the FATFS filesystem
	return (esp_vfs_fat_sdcard_unmount(m_driveName.c_str(), m_card) == ESP_OK);
}


bool SD_SPI::isMounted() const
{
	return m_isMounted;
}


std::string SD_SPI::getDriveName() const
{
	return m_driveName;
}
