#pragma once
#ifndef __INTERNAL_FLASH_BLOCK_DEVICE_H__
#define __INTERNAL_FLASH_BLOCK_DEVICE_H__
#ifdef __cplusplus
extern "C" {
#endif //!<#ifdef __cplusplus
/*************************** C start ********************************/
/*************************** C end ********************************/
#ifdef __cplusplus
}
/*************************** cpp start ********************************/
#include "BlockDevice.h"
#include "mbed.h"
#include "FlashIAP.h"
#include "gsl"
class Internal_flash_block_device : public BlockDevice
{
  public:
    Internal_flash_block_device(uint32_t start, uint32_t size)
        : m_start(start), m_size(size)
    {
    }
    virtual ~Internal_flash_block_device()
    {
        
    }
    virtual int init()
    {
        const auto flash_ret = m_internal_flash.init();
        Ensures(flash_ret == 0);
        const auto flash_start = m_internal_flash.get_flash_start();
        Ensures(flash_start <= m_start);
        const auto flash_size = m_internal_flash.get_flash_size();
        Ensures(flash_start + flash_size >= m_start + m_size);
        const auto page_size = m_internal_flash.get_page_size();
        Ensures(m_size % page_size == 0);
        Ensures((m_start - flash_start) % page_size == 0);
        return BD_ERROR_OK;
    }
    virtual int deinit()
    {
        const auto flash_ret = m_internal_flash.deinit();
        Ensures(flash_ret == 0);
        return BD_ERROR_OK;
    }
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size)
    {
        //printf("read addr %llu, size %llu\r\n", addr, size);        
        Expects(addr + size <= m_size);
        const auto flash_ret = m_internal_flash.read(buffer, m_start + addr, size);
        Ensures(flash_ret == 0);
        return BD_ERROR_OK;
    }
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size)
    {
        printf("program addr %llu, size %llu\r\n", addr, size);        
        Expects(addr + size <= m_size);
        const auto flash_ret = m_internal_flash.program(buffer, m_start +addr, size);
        Ensures(flash_ret == 0);
        return BD_ERROR_OK;
    }

    virtual int erase(bd_addr_t addr, bd_size_t size)
    {
        printf("erase addr %llu, size %llu\r\n", addr, size);        
        Expects(addr + size <= m_size);
        const auto flash_ret = m_internal_flash.erase(m_start + addr, size);
        Ensures(flash_ret == 0);
        return BD_ERROR_OK;
    }
    virtual bd_size_t get_read_size() const
    {
        return m_internal_flash.get_page_size();
    }

    virtual bd_size_t get_program_size() const
    {
        return m_internal_flash.get_page_size();
    }
    virtual bd_size_t size() const
    {
        return m_size;
    }

  private:
    FlashIAP m_internal_flash;
    uint32_t m_start;
    uint32_t m_size;
};
/*************************** cpp end ********************************/
#endif //!<#ifdef __cplusplus
#endif //!< #ifndef __INTERNAL_FLASH_BLOCK_DEVICE_H__