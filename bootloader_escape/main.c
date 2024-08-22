/** Based on: https://github.com/espressif/esptool/tree/7cc35e4e4df5568af94e0d1b9d3e737df8a00e7a/test/images/ram_helloworld/source
 *
 */

#define false (0)

extern void ets_printf(const char *s);
extern unsigned int ets_efuse_get_spiconfig(void);
extern void spi_flash_boot_attach(unsigned int, char, char);
extern int ets_run_flash_bootloader(void);

void __attribute__((noreturn)) ram_main()
{
  unsigned int spi_config = ets_efuse_get_spiconfig();
  char spi_legacy_mode = false;
  char spi_opi_mode = false;

  spi_flash_boot_attach(spi_config, spi_legacy_mode, spi_opi_mode);
  int res = ets_run_flash_bootloader();
  if (res != 0) {
    ets_printf("Failed to load application from flash");
  }

  while(1){}
}